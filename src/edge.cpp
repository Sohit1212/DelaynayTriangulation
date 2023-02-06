#include "edge.h"
#include <iostream>

Edge::Edge()
{
    this->v1 = Vertex();
    this->v2 = Vertex();
}

Edge::Edge(const Vertex v1, const Vertex v2)
{
    this->v1 = v1;
    this->v2 = v2;
}

bool Edge::operator==(const Edge &e)
{

    std::cout<<"Comaprison: Edge 1 ("<<this->v1.getX()<<","<<this->v1.getY()<<")-->("<<this->v2.getX()<<","<<this->v2.getY()<<")\n" \
       <<"                  Edge 2 ("<<(e.v1).getX()<<","<<(e.v1).getY()<<")-->("<<(e.v2).getX()<<","<<(e.v2).getY()<<")\n";
    if((this->v1 == e.v1 && this->v2 == e.v2) || (this->v1 == e.v2 && this->v2 == e.v1))
    {
        std::cout<<"RETURN TRUE\n";
        return true;
    }
    return false;
}
