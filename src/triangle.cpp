#include "triangle.h"
#include <iostream>

Triangle::Triangle()
{
    this->v0 = Vertex();
    this->v1 = Vertex();
    this->v2 = Vertex();
}

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2)
{
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;

}

bool Triangle::operator ==(const Triangle &t)
{
    if(this->v0 == t.v0 && this->v1 == t.v1 && this->v2 == t.v2)
        return true;
    return false;
}

