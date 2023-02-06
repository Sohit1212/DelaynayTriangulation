#include "vertex.h"


Vertex::Vertex()
{
    this->x = -1.0;
    this->y = -1.0;
}

Vertex::Vertex(const double &x, const double &y)
{
    this->x = x;
    this->y = y;
}
void Vertex::setCoordinates(const double &x, const double &y)
{
    this->x = x;
    this->y = y;
}

bool Vertex::operator==(const Vertex &v)
{
    if(this->x == v.x && this->y == v.y)
        return true;
    return false;
}

Vertex Vertex::sum(const Vertex &v)
{
    Vertex newV = Vertex();
    newV.x = this->x + v.x;
    newV.y = this->y + v.y;

    return newV;
}

Vertex Vertex::diff(const Vertex &v)
{
    Vertex newV = Vertex();
    newV.x = this->x - v.x;
    newV.y = this->y - v.y;

    return newV;
}

Vertex Vertex::dot(const Vertex &v)
{
    Vertex newV = Vertex();
    newV.x = this->x * v.x;
    newV.y = this->y * v.y;

    return newV;
}
