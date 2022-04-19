#include "terrain.h"

Terrain::Vertices Terrain::buildVertices(int vertexCount, QPoint startingVertex = QPoint())
{
    QPoint vertex(startingVertex);
    vertices.clear();
    edgeVertices.clear();
    for (int i = 0; i < vertexCount; i++)
    {
        vertices.append(vertex);
        vertex.rx() += random(minDx, rangeDx);
        int testY;
        do
        {
            testY = vertex.y() + random(minDy, rangeDy) * std::cos(M_PI * (i % 2));
        }
        while (qAbs(testY) - (qAbs(startingVertex.y()) - rangeY / 2) > rangeY);
        vertex.setY(testY);
        if (i > 0) buildCurveSegments(i - 1, i);
    }
    return edgeVertices;
}

void Terrain::buildCurveSegments(int vertexIndex1, int vertexIndex2)
{
    QPointF vertex1 = vertices.at(vertexIndex1);
    QPointF vertex2 = vertices.at(vertexIndex2);
    float midpointY = (vertex1 + vertex2).y() / 2.f;
    for (int i = 0; i <= segmentsPerEdge; i++)
    {
        QPointF vertex;
        vertex.setX(vertex1.x() + i * (vertex2 - vertex1).x() / segmentsPerEdge);
        vertex.setY(midpointY - (midpointY - vertex1.y()) * std::cos(i * M_PI / segmentsPerEdge));
        edgeVertices.append(vertex);
    }
}
