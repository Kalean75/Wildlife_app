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
    static constexpr float pixelsPerMeter = 128.f;
    void setDebugRenderer(b2Draw&);

public slots:
    void update();
    void addBody(Entities::PhysicsBag*);
    void removeBody(Entities::PhysicsBag*);
    void queryPoint(QPoint);
    void debugRender();

private:
    struct WorldQuery : b2QueryCallback
    {
        b2Body *body;
        bool ReportFixture(b2Fixture *fixture)
        {
            body = fixture->GetBody();
            return false;
        }
    };
    const int velocityIterations = 8;
    const int positionIterations = 3;
    const b2Vec2 gravity = b2Vec2(0.f, 9.8f);
    b2World *world;
    WorldQuery *worldQuery = new WorldQuery{};
    Entities::PhysicsBag* loadUserData(b2Body*);
};

#endif // PHYSICS_H
