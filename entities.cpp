#include "entities.h"

Entities::Entities(QObject *parent) : QObject(parent), timer(new QTimer(this))
{
    timer->setTimerType(Qt::PreciseTimer);
    timer->start(timeStep * 1000);
    connect(timer, &QTimer::timeout, this, &Entities::updateEntities);
}

int Entities::newEntity()
{
    return eid++;
}

void Entities::updateEntities()
{
    emit physicsOutdated(timeStep);
    emit renderOutdated(physicsBags, renderBags);
}

void Entities::addPhysics(int e, PhysicsBag *bag)
{
    if (!physicsBags.contains(e))
    {
        emit addedPhysics(bag);
        physicsBags[e] = bag;
    }
}

void Entities::addRender(int e, RenderBag *bag)
{
    renderBags[e] = bag;
}

Entities::~Entities()
{
}
