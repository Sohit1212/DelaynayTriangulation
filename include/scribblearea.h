#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <vertex.h>
#include <triangle.h>
#include <edge.h>

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    explicit ScribbleArea(QWidget *parent = nullptr);
    bool openImage(const QString &filename);                            // load an image from file
    bool saveImage(const QString &filename, const char *fileFormat);    // store current image in file
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() { return modified; }                              // true if there are unsaved changes to image
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }
    QVector<Triangle> getTri() const { return listTriangle; }

    Vertex circumCenter(const Triangle &t);
    bool inCircumcircle(const Triangle t, Vertex v);
    void triangulate(const Vertex &v);

public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endpoint);
    void drawLine(const QPoint &p1, const QPoint &p2);
    void eraseLine(const QPoint &p1, const QPoint &p2);
    void drawPointAt(const QPoint &point);
    void resizeImage(QImage *image, const QSize &newSize);

    bool modified = false;
    bool scribbling = false;                        // true if the user is pressing left mouse button in the scribble area
    QColor myPenColor = Qt::black;
    int myPenWidth = 3;

    QImage image;                                   // stores the image drawn by the user
    QPoint lastPoint;                               // holds the position of the cursor at the last mouse press or mouse move event.

    QVector<Triangle> listTriangle;
    QVector<Edge> edgeSet;

};

#endif // SCRIBBLEAREA_H
