#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"

class Edge
{
private:
    Vertex v1;
    Vertex v2;

public:
    Edge();
    Edge(const Vertex v1, const Vertex v2);
    bool operator==(const Edge &e);

    Vertex getV1() { return v1; }
    Vertex getV2() { return v2; }

};

#endif // EDGE_H
