#include "line.h"

Line::Line()
{
    this->a = 0;
    this->b = 0;
    this->c = 0;
}

Line::Line(const double &a, const double &b, const double &c)
{
    this->a = a;
    this->b = b;
    this->c = c;
}

double Line::slope()
{
    return -(this->b)/this->a;
}

/* Given two vertices P, Q
 * Line PQ => ax + by = c;
 * a = (yq-yp), b = (xp-cq)
 * Substitute values of vertex P or Q to obtain C
 *
 * Perpendicular bisector of PQ => PB(pq)
 * d = -bx + ay;
 * put x == (xp+xq)/2;
 *     y == (yp+yq)/2;
 *
 * Calculate the value of d
 */
Line Line::bisector(Vertex &v1, Vertex &v2)
{
    double midx = (v1.getX() + v2.getX()) / 2.0;
    double midy = (v1.getY() + v2.getY()) / 2.0;

    double a = v1.getY() - v2.getY();
    double b = v2.getX() - v1.getX();
    //double c = a*this->x + b*this->y;         // the value of c is not required here

    double d = -b*midx + a*midy;

    Line res = Line(-b, a, d);
    return res;
}

Vertex Line::intersection(Line l)
{
    double determinant = (this->a*l.b - this->b*l.a);

    if(determinant == 0)
    {
        return Vertex();
    }
    else
    {
        double x = (this->c*l.b - this->b*l.c)/determinant;
        double y = (this->a*l.c- this->c*l.a)/determinant;

        return Vertex(x,y);
    }
}
