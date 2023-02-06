#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

class Vertex
{
private:
    double x;
    double y;
public:
    Vertex();
    Vertex(const double &x, const double &y);
    void setCoordinates(const double &x, const double &y);
    bool operator==(const Vertex &v);
    Vertex sum(const Vertex &v);
    Vertex diff(const Vertex &v);
    Vertex dot(const Vertex &v);

    double getX() const { return x; }
    double getY() const { return y; }
};

#endif // VERTEX_H
