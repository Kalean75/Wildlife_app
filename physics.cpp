#include "entities.h"
#include "physics.h"

Physics::Physics(QObject *parent) : QObject(parent), world(new b2World(gravity))
{
}

void Physics::update()
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

void Physics::addBody(Entities::PhysicsBag *bag)
{
    b2Body *body;
    b2BodyDef bodyDef;
    b2PolygonShape bodyShape;
    b2FixtureDef bodyFixture;
    bodyDef.type = bag->type;
    bodyDef.angle = bag->angle;
    bodyDef.position.Set(bag->x / pixelsPerMeter, bag->y / pixelsPerMeter);
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(bag); // https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_loose_ends.html
    bodyDef.linearDamping = bag->linearDamping;
    bodyDef.angularDamping = bag->angularDamping;
    bodyShape.SetAsBox((bag->w / pixelsPerMeter) / 2.f, (bag->h / pixelsPerMeter) / 2.f);
    bodyFixture.shape = &bodyShape;
    bodyFixture.density = bag->density;
    bodyFixture.friction = bag->friction;
    bodyFixture.restitution = bag->restitution;
    body = world->CreateBody(&bodyDef);
    body->CreateFixture(&bodyFixture);
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

void Physics::queryPoint(QPoint point)
{
    b2AABB bounds;
    bounds.upperBound = b2Vec2(point.x() / pixelsPerMeter, point.y() / pixelsPerMeter);
    bounds.lowerBound = b2Vec2((point.x() + 1) / pixelsPerMeter, (point.y() + 1) / pixelsPerMeter);
    worldQuery->body = nullptr;
    world->QueryAABB(worldQuery, bounds);
    // TODO: Maybe emit a signal to some steering system for body movement
    if (worldQuery->body) worldQuery->body->SetLinearVelocity(b2Vec2(0, -5.f)); // Ad hoc "jump impulse on click" implementation
}

void Physics::setDebugRenderer(b2Draw& renderer)
{
    world->SetDebugDraw(&renderer);
}

void Physics::debugRender()
{
    world->DebugDraw();
}

Entities::PhysicsBag* Physics::loadUserData(b2Body *body)
{
    return reinterpret_cast<Entities::PhysicsBag*>(body->GetUserData().pointer);
}

Physics::~Physics()
{
    delete world;
    delete worldQuery;
}
