#include "physics.h"

Physics::Physics(QObject *parent)
    : QObject(parent),
      world(new b2World(gravity)),
      timer(new QTimer(this))
{
    timer->setTimerType(Qt::PreciseTimer);
    timer->start(timeStep * msPerSecond);
    connect(timer, &QTimer::timeout, this, &Physics::updateWorld);
    // TODO: Use some sort of factory function for loading, maybe load body data from a file or files, maybe have classes
    // for each kind of entity and assign the class data to the body's userdata
    // Static body
    b2Body* staticBody;
    b2BodyDef staticDef;
    b2PolygonShape staticShape;
    staticDef.position.Set(-200 / pixelsPerMeter, 0);
    staticShape.SetAsBox(400 / pixelsPerMeter, 10 / pixelsPerMeter);
    staticBody = world->CreateBody(&staticDef);
    staticBody->CreateFixture(&staticShape, 0);
    // Dynamic body
    b2Body* dynamicBody;
    b2BodyDef dynamicDef;
    b2PolygonShape dynamicShape;
    b2FixtureDef dynamicFixture;
    dynamicDef.type = b2_dynamicBody;
    dynamicDef.position.Set(0, -100 / pixelsPerMeter);
    dynamicShape.SetAsBox(1 / pixelsPerMeter, 1 / pixelsPerMeter);
    dynamicBody = world->CreateBody(&dynamicDef);
    dynamicFixture.shape = &dynamicShape;
    dynamicFixture.density = 1.0f;
    dynamicFixture.friction = 0.3f;
    dynamicFixture.restitution = 1.0f;
    dynamicBody->CreateFixture(&dynamicFixture);
}

void Physics::updateWorld()
{
    world->Step(timeStep, velocityIterations, positionIterations);
    positions.clear();
    for (b2Body *body = world->GetBodyList(); body; body = body->GetNext())
    {
        if (body->GetType() == b2BodyType::b2_dynamicBody)
        {
            b2Vec2 position = body->GetPosition();
            positions.append(QPointF(position.x * pixelsPerMeter, position.y * pixelsPerMeter));
        }
    }
    emit worldUpdated(positions);
}

Physics::~Physics()
{
    delete timer;
    delete world;
}
