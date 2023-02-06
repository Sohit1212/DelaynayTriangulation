#ifndef LINE_H
#define LINE_H

#include "vertex.h"

class Line
{
private:
    double a;
    double b;
    double c;
public:
    Line();
    Line(const double &a, const double &b, const double &c);
    double slope();
    static Line bisector(Vertex &v1, Vertex &v2);
    Vertex intersection(const Line l);
};

#endif // LINE_H
