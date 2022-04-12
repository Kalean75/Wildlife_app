#include "entities.h"
#include "physics.h"

Physics::Physics(QObject *parent) : QObject(parent), world(new b2World(gravity))
{
}

void Physics::updatePhysics()
{
    world->Step(Entities::updateRate, velocityIterations, positionIterations);
    for (b2Body *body = world->GetBodyList(); body; body = body->GetNext())
    {
        Entities::PhysicsBag *bag = loadUserData(body);
        b2Vec2 position = body->GetWorldCenter();
        bag->x = position.x * pixelsPerMeter;
        bag->y = position.y * pixelsPerMeter;
        bag->angle = body->GetAngle();
    }
}

void Physics::removeBody(Entities::PhysicsBag *bag)
{
    if (!bag) return;
    for (b2Body *body = world->GetBodyList(); body; body = body->GetNext())
    {
        if (loadUserData(body) == bag)
        {
            world->DestroyBody(body);
            return;
        }
    }
}

void Physics::addBody(Entities::PhysicsBag *bag)
{
    b2Body* body;
    b2BodyDef bodyDef;
    b2PolygonShape bodyShape;
    b2FixtureDef bodyFixture;
    bodyDef.type = bag->type;
    bodyDef.angle = bag->angle;
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(bag); // https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_loose_ends.html
    bodyDef.position.Set(bag->x / pixelsPerMeter, bag->y / pixelsPerMeter);
    bodyShape.SetAsBox((bag->w / pixelsPerMeter) / 2.f, (bag->h / pixelsPerMeter) / 2.f);
    bodyFixture.shape = &bodyShape;
    bodyFixture.density = bag->density;
    bodyFixture.friction = bag->friction;
    bodyFixture.restitution = bag->restitution;
    body = world->CreateBody(&bodyDef);
    body->CreateFixture(&bodyFixture);
}

Entities::PhysicsBag* Physics::loadUserData(b2Body *body)
{
    return reinterpret_cast<Entities::PhysicsBag*>(body->GetUserData().pointer);
}

Physics::~Physics()
{
    delete world;
}
