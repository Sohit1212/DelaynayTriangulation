#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scribblearea.h"
#include <QCloseEvent>
#include <QFileDialog>
#include <QAction>
#include <QInputDialog>
#include <QColorDialog>
#include <QImageWriter>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QApplication>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

/* open(), save(), penColor() and penWidth() slots correspond to menu entries */
private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();

private:
    void createActions();
    void createMenus();
    bool maybeSave();                               // check if there are any unsaved changes
    bool saveFile(const QByteArray &fileformat);

    ScribbleArea *scribbleArea;
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *printAct;
    QAction *clearScreenAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

signals:

};

#endif // MAINWINDOW_H
