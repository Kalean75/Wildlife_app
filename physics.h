#ifndef PHYSICS_H
#define PHYSICS_H

#include "entities.h"
#include <box2d/box2d.h>

#include <QObject>
#include <QPointF>

class Physics : public QObject
{
    Q_OBJECT
public:
    explicit Physics(QObject *parent = nullptr);
    ~Physics();

public slots:
    void updateWorld(float);
    void createBody(Entities::PhysicsBag*);

private:
    const float pixelsPerMeter = 32.f;
    const int velocityIterations = 6;
    const int positionIterations = 2;
    const b2Vec2 gravity = b2Vec2(0.f, 9.8f);
    b2World *world;
};

#endif // PHYSICS_H
