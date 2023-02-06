#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}, scribbleArea(new ScribbleArea(this))
{
    setCentralWidget(scribbleArea);

    createActions();
    createMenus();

    setWindowTitle(tr("Delaunay Triangulation"));
    setFixedSize(500, 500);
    //resize(500, 500);
}

/* maybeSave() -> returns true if there is no modification
 * returns false if save is cancelled.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave())
        event->accept();
    else
        event->ignore();
}

/*In the open() slot we first give the user the opportunity to save any modifications to the currently displayed image,
 * before a new image is loaded into the scribble area.
 * Then we ask the user to choose a file and we load the file in the ScribbleArea.
 */
void MainWindow::open()
{
    if(maybeSave())
    {
        QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if(!filename.isEmpty())
            scribbleArea->openImage(filename);
    }
}

/* save() slot is called when the users choose the Save As menu entry, and then choose an entry from the format menu
 * find out which action sent the signal using QObject::sender()
 * Once we have the action, we extract the chosen format using QAction::data()
 */
void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);

}

/* use the penColor() slot to retrieve a new color from the user with a QColorDialog */
void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}

/* To retrieve a new pen width in the penWidth() slot, we use QInputDialog.
 * The QInputDialog class provides a simple convenience dialog to get a single value from the user.
 */
void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"), tr("Scriblle pen width:"), scribbleArea->penWidth(),
                                        1, 50, 1, &ok);

    if(ok)
        scribbleArea->setPenWidth(newWidth);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Scribble"),
            tr("<p>The <b>Scribble</b> example shows how to use QMainWindow as the "
               "base widget for an application, and how to reimplement some of "
               "QWidget's event handlers to receive the events generated for "
               "the application's widgets:</p><p> We reimplement the mouse event "
               "handlers to facilitate drawing, the paint event handler to "
               "update the application and the resize event handler to optimize "
               "the application's appearance. In addition we reimplement the "
               "close event handler to intercept the close events before "
               "terminating the application.</p><p> The example also demonstrates "
               "how to use QPainter to draw an image in real time, as well as "
               "to repaint widgets.</p>"));
}

/* create the actions representing the menu entries and connect them to the appropriate slots */
void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();

    for(const QByteArray &format : imageFormats)
    {
        QString text = tr("%1...").arg(QString::fromLatin1(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, &QAction::triggered, this, &MainWindow::save);
        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, &QAction::triggered, scribbleArea, &ScribbleArea::print);

    exitAct = new QAction(tr("&Exit..."), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, &QAction::triggered, this, &MainWindow::penColor);

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, &QAction::triggered, this, &MainWindow::penWidth);

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, &QAction::triggered, scribbleArea, &ScribbleArea::clearImage);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

/* Add the previously created format actions to the saveAsMenu.
 * Then we add the rest of the actions as well as the saveAsMenu sub-menu to the File, Options and Help menus.
 */
void MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("&Save As"), this);
    for(QAction *action: std::as_const(saveAsActs))
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

/* We check if there are any unsaved changes.
 * If there are any, we use QMessageBox to give the user a warning that the image has been modified
 * and the opportunity to save the modifications
 */
bool MainWindow::maybeSave()
{
    if(scribbleArea->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("This file has been modified\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if(ret == QMessageBox::Save)
            return saveFile("png");
        else if(ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}


bool MainWindow::saveFile(const QByteArray &fileformat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileformat;
    QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath, tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(QString::fromLatin1(fileformat.toUpper()))
                                                    .arg(QString::fromLatin1(fileformat)));
    if(filename.isEmpty())
        return false;
    return scribbleArea->saveImage(filename, fileformat.constData());
}
