#ifndef TERRAIN_H
#define TERRAIN_H

#include "entities.h"

#include <QVector>
#include <QPoint>

class Terrain
{
public:
    typedef QVector<QPointF> Vertices;
    Vertices buildVertices(int, QPoint);
    void buildCurveSegments(int, int);
private:
    // Parameters can be adjusted for "noisiness"
    int segmentsPerEdge = 10;
    int rangeY = 500;
    int minDx = 750;
    int minDy = 200;
    int rangeDx = 1500;
    int rangeDy = 300;
    Vertices vertices;
    Vertices edgeVertices; // Vertex could be sampled from here to spawn an entity at a random point
};

#endif // TERRAIN_H
