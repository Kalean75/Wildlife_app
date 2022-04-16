#include "entities.h"
#include "physics.h"

Physics::Physics(QObject *parent) : QObject(parent), world(new b2World(gravity)), worldQuery(new WorldQuery{})
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
    b2Vec2 bodyPosition(bag->x / pixelsPerMeter, bag->y / pixelsPerMeter);
    b2BodyDef bodyDef;
    b2FixtureDef bodyFixture;
    b2CircleShape circleShape;
    b2PolygonShape polygonShape;
    b2EdgeShape edgeShape;
    bodyDef.type = bag->bodyType;
    bodyDef.angle = bag->angle;
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(bag); // https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_loose_ends.html
    bodyDef.linearDamping = bag->linearDamping;
    bodyDef.angularDamping = bag->angularDamping;
    bodyFixture.density = bag->density;
    bodyFixture.friction = bag->friction;
    bodyFixture.restitution = bag->restitution;
    // Cases enclosed in a scope to allow bodyFixture.shape to be assigned
    switch (bag->shapeType)
    {
    case b2Shape::e_circle:
    {
        circleShape.m_radius = bag->r / pixelsPerMeter;
        circleShape.m_p = bodyPosition;
        bodyFixture.shape = &circleShape;
        break;
    }
    case b2Shape::e_polygon:
    {
        polygonShape.SetAsBox((bag->w / pixelsPerMeter) / 2.f, (bag->h / pixelsPerMeter) / 2.f);
        bodyDef.position = bodyPosition;
        bodyFixture.shape = &polygonShape;
        break;
    }
    case b2Shape::e_edge:
    {
        edgeShape.SetTwoSided(bodyPosition, b2Vec2(bag->x1 / pixelsPerMeter, bag->y1 / pixelsPerMeter));
        bodyFixture.shape = &edgeShape;
        break;
    }
    default:
        break;
    }
    world->CreateBody(&bodyDef)->CreateFixture(&bodyFixture);
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
    // TODO: Some sort of heuristic for determining an animal aside from just "is dynamic body"
    b2Body *body = worldQuery->body;
    if (body && body->GetType() == b2_dynamicBody) body->SetLinearVelocity(b2Vec2(0, -5.f)); // Ad hoc "jump impulse on click" implementation
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
