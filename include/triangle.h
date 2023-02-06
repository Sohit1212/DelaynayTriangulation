#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vertex.h"
#include "line.h"
#include "edge.h"
#include <QtMath>
#include <QVector>
#include <QSet>
#include <QSetIterator>

class Triangle
{
private:
    Vertex v0;
    Vertex v1;
    Vertex v2;

public:
    Triangle();
    Triangle(const Vertex v0, const Vertex v1, const Vertex v2);
    bool operator==(const Triangle &t);

    Vertex getV0() const { return v0; }
    Vertex getV1() const { return v1; }
    Vertex getV2() const { return v2; }
};

#endif // TRIANGLE_H
