#include "scribblearea.h"
#include <QVector>
#include <QMouseEvent>
#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QtPrintSupport>

#include <iostream>

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget{parent}
{
    double initialSize = 10000;
    QPoint p0 = QPoint(-initialSize, -initialSize);
    QPoint p1 = QPoint(initialSize, -initialSize);
    QPoint p2 = QPoint(0, initialSize);

    drawPointAt(p0);
    drawPointAt(p1);
    drawPointAt(p2);

    Vertex v0 = Vertex(p0.x(), p0.y());
    Vertex v1 = Vertex(p1.x(), p1.y());
    Vertex v2 = Vertex(p2.x(), p2.y());

    Triangle superT = Triangle(v0, v1, v2);
    listTriangle.push_back(superT);

    setAttribute(Qt::WA_StaticContents);    //contents are static and rooted to top-left corner

}

bool ScribbleArea::openImage(const QString &filename)
{
    QImage loadedImage;
    if(!loadedImage.load(filename))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);

    image = loadedImage;
    modified = false;
    update();

    return true;
}

bool ScribbleArea::saveImage(const QString &filename, const char *fileFormat)
{
    QImage visibleImage = image;

    resizeImage(&visibleImage, size());

    if(visibleImage.save(filename, fileFormat))
    {
        modified = false;
        return true;
    }

    return false;
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()
{
   image.fill(qRgb(255, 255, 255));
   modified = true;
   update();
}

/* Given a Triangle, we have its 3 vertices
 * Find the Perpendicular bisector for two of the edges
 * Then calculate the point of intersection of the bisetors
 */
Vertex ScribbleArea::circumCenter(const Triangle &t)
{
    Vertex v0 = t.getV0();
    Vertex v1 = t.getV1();
    Vertex v2 = t.getV2();

    Line l1 = Line::bisector(v0, v1);
    Line l2 = Line::bisector(v1, v2);

    Vertex intersect = l1.intersection(l2);

    return intersect;
}

bool ScribbleArea::inCircumcircle(const Triangle t, Vertex v)
{
    Vertex cCenter = circumCenter(t);
    Vertex v0 = t.getV0();

    double radius = sqrt(pow(v0.getX() - cCenter.getX() ,2)+pow(v0.getY() - cCenter.getY() ,2));
    double dist = sqrt(pow(v.getX() - cCenter.getX() ,2)+pow(v.getY() -cCenter.getY() ,2));

    if(dist < radius)
        return true;
    return false;
}

void ScribbleArea::triangulate(const Vertex &v)
{
    std::cout<<"Inside triangulate\n";
    QVector<Edge> edgeList;
    int numTri = listTriangle.size();
    std::cout<<numTri<<std::endl;
    QVector<Triangle>::iterator it = listTriangle.begin();
    while(it != listTriangle.end())
    {
        if(listTriangle.size() == 0) break;
        std::cout<<"Inside For loop\n";
        if(inCircumcircle(*it, v))
        {
            std::cout<<"Inside inCircumcircle\n";
            edgeList.push_back(Edge((*it).getV0(),(*it).getV1()));
            edgeList.push_back(Edge((*it).getV1(),(*it).getV2()));
            edgeList.push_back(Edge((*it).getV2(),(*it).getV0()));
            it = listTriangle.erase(it);
        }
        else it++;
    }

    int siz = edgeList.size();

    for(int i=0;i<siz;i++)
    {
        std::cout<<"Checking one edge: ("<<edgeList[i].getV1().getX()<<","<<edgeList[i].getV1().getY() \
                <<")<--->("<<edgeList[i].getV2().getX()<<","<<edgeList[i].getV2().getY()<<")"<<std::endl;
        QPoint p1 = QPoint(edgeList[i].getV1().getX(), edgeList[i].getV1().getY());
        QPoint p2 = QPoint(edgeList[i].getV2().getX(), edgeList[i].getV2().getY());
        eraseLine(p1, p2);
        bool flag = true;
        for(int j=0;j<siz;j++)
        {
            if(i != j && edgeList[i] == edgeList[j])
            {
                flag = false;
                break;
            }
        }
        if(flag)
        {
            //std::cout<<"Added one edge: ("<<edgeList[i].getV1().getX()<<","<<edgeList[i].getV1().getY() \
                    <<")<--->("<<edgeList[i].getV2().getX()<<","<<edgeList[i].getV2().getY()<<")"<<std::endl;
            edgeSet.push_back(edgeList[i]);
        }
    }
}


/* mousePressEvent -> records position of the press event if left button is pressed
 * and marks scribbling ON
 */
void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
        drawPointAt(lastPoint);
        Vertex v = Vertex(lastPoint.x(), lastPoint.y());
        std::cout<<"Point Clicked: "<<v.getX()<<" "<<v.getY()<<std::endl;
        triangulate(v);
        //std::cout<<"Point Clicked: "<<v.getX()<<" "<<v.getY()<<std::endl;
        QPoint p = QPoint(v.getX(), v.getY());
        for(Edge e : qAsConst(edgeSet))
        {
            Vertex v1 = e.getV1();
            Vertex v2 = e.getV2();
            Triangle t = Triangle(v, v1, v2);
            listTriangle.push_back(t);
            QPoint p1 = QPoint(v1.getX(), v1.getY());
            QPoint p2 = QPoint(v2.getX(), v2.getY());

            drawLine(p, p1);
            drawLine(p, p2);
            drawLine(p1, p2);


        }

        edgeSet.clear();
        //scribbling = true;
    }
}

/* mouseMoveEvent -> if the left button is pressed and mouse if moved
 * call private function drawLineTo on the current positions
 *
void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if((event->button() & Qt::LeftButton) && scribbling)
    {
        drawLineTo(event->pos());
    }
}
*/
/* mouseReleaseEvent -> if the left button is released
 * call private function drawLineTo on the current positions
 * and mark scribbling as false
 *
void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && scribbling)
    {
        drawLineTo(event->pos());
        scribbling = false;
    }
}
*/
/* start a paintEvent
 * create a QPainter for the scribble area, and draw the image.
 *
 * Q. why we don't just draw directly onto the widget instead of drawing in a QImage
 * and copying the QImage onto screen in paintEvent() ?
 *
 * 1. Window system does not remember our image, and hence cannot redraw it in a new paintEvent
 * 2. Qt normally doesn't allow us to paint outside of paintEvent()
 * 3. QImage is guaranteed to use 8-bit for each color channel (red, green, blue, and alpha)
 */
void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}
/* When user starts application, resizeEvent is generated
 * Initial image is slightly larger than mainWindow
 */
void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if(width() > image.width() || height() > image.height())
    {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

/* we draw a line from the point where the mouse was located when the last mouse press or mouse move occurred,
 * we set modified to true, we generate a repaint event, and we update lastPoint
 * so that next time drawLineTo() is called, we continue drawing from where we left.

void ScribbleArea::drawLineTo(const QPoint &endpoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endpoint);
    modified = true;

    int rad = (myPenWidth/2) + 2;
    update(QRect(lastPoint, endpoint).normalized().adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endpoint;
}
*/

void ScribbleArea::eraseLine(const QPoint &p1, const QPoint &p2)
{
    QPainter painter(&image);
    painter.setPen(QPen(Qt::white, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(p1, p2);

    modified = true;

    int rad = (myPenWidth/2) + 2;
    update(QRect(p1, p2).normalized().adjusted(-rad, -rad, +rad, +rad));

}

void ScribbleArea::drawLine(const QPoint &p1, const QPoint &p2)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(p1, p2);

    modified = true;

    int rad = (myPenWidth/2) + 2;
    update(QRect(p1, p2).normalized().adjusted(-rad, -rad, +rad, +rad));

}

void ScribbleArea::drawPointAt(const QPoint &point)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPoint(point);

    modified = true;

    int rad = (myPenWidth/2) + 2;
    update(QRect(point, point).normalized().adjusted(-rad, -rad, +rad, +rad));
}

/* Create a new QImage with the right size, fill it with white,
 * and draw the old image onto it using QPainter
 */
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if(image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);

    painter.drawImage(QPoint(0,0), *image);
    *image = newImage;
}

/* Construct a high resolution QPrinter object for the required output format,
 * using a QPrintDialog to ask the user to specify a page size and
 * indicate how the output should be formatted on the page.
 */
void ScribbleArea::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);

    //If the dialog is accepted
    if(printDialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif  // QT_CONFIG(printdialog)
}
