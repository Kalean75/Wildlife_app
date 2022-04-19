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
        Entities::PhysicsBag *physicsBag = loadUserData(body);
        if (body->GetType() == b2BodyType::b2_dynamicBody)
        {
            b2Shape *bodyShape = body->GetFixtureList()->GetShape();
            switch (bodyShape->GetType())
            {
            case b2Shape::e_edge:
            {
                b2EdgeShape *edge = static_cast<b2EdgeShape*>(bodyShape);
                b2Vec2 vertex1 = edge->m_vertex1;
                b2Vec2 vertex2 = edge->m_vertex2;
                physicsBag->x = vertex1.x * pixelsPerMeter;
                physicsBag->y = vertex1.y * pixelsPerMeter;
                physicsBag->x1 = vertex2.x * pixelsPerMeter;
                physicsBag->y1 = vertex2.y * pixelsPerMeter;
                break;
            }
            default:
                b2Vec2 bodyPosition = body->GetWorldCenter();
                physicsBag->x = bodyPosition.x * pixelsPerMeter;
                physicsBag->y = bodyPosition.y * pixelsPerMeter;
            }
        }
        physicsBag->angle = body->GetAngle();
    }
}

void Physics::addBody(Entities::PhysicsBag *physicsBag)
{
    b2Vec2 bodyPosition(physicsBag->x / pixelsPerMeter, physicsBag->y / pixelsPerMeter);
    b2BodyDef bodyDef;
    b2FixtureDef bodyFixture;
    b2CircleShape circleShape;
    b2PolygonShape polygonShape;
    b2EdgeShape edgeShape;
    bodyDef.type = physicsBag->bodyType;
    bodyDef.angle = physicsBag->angle;
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(physicsBag); // https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_loose_ends.html
    bodyDef.linearDamping = physicsBag->linearDamping;
    bodyDef.angularDamping = physicsBag->angularDamping;
    bodyFixture.isSensor = physicsBag->isSensor;
    bodyFixture.density = physicsBag->density;
    bodyFixture.friction = physicsBag->friction;
    bodyFixture.restitution = physicsBag->restitution;
    // Cases enclosed in a scope to allow bodyFixture.shape to be assigned
    switch (physicsBag->shapeType)
    {
    case b2Shape::e_circle:
    {
        circleShape.m_radius = physicsBag->r / pixelsPerMeter;
        circleShape.m_p = bodyPosition;
        bodyFixture.shape = &circleShape;
        break;
    }
    case b2Shape::e_polygon:
    {
        polygonShape.SetAsBox(physicsBag->w / pixelsPerMeter / 2.f, physicsBag->h / pixelsPerMeter / 2.f);
        bodyDef.position = bodyPosition;
        bodyFixture.shape = &polygonShape;
        break;
    }
    case b2Shape::e_edge:
    {
        edgeShape.SetTwoSided(bodyPosition, b2Vec2(physicsBag->x1 / pixelsPerMeter, physicsBag->y1 / pixelsPerMeter));
        bodyFixture.shape = &edgeShape;
        break;
    }
    default:
        break;
    }
    world->CreateBody(&bodyDef)->CreateFixture(&bodyFixture);
}

void Physics::removeBody(Entities::PhysicsBag *physicsBag)
{
    if (!physicsBag) return;
    for (b2Body *body = world->GetBodyList(); body; body = body->GetNext())
    {
        if (loadUserData(body) == physicsBag)
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
    if (body && body->GetType() == b2_dynamicBody)
    {
        body->SetLinearVelocity(b2Vec2(0, -5.f)); // Ad hoc "jump impulse on click" implementation
        emit bodyQueried(loadUserData(body));
    }
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
