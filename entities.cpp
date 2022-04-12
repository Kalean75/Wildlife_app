#include "entities.h"

Entities::Entities(QObject *parent) : QObject(parent), timer(new QTimer(this))
{
    timer->setTimerType(Qt::PreciseTimer);
    timer->start(updateRate * 1000);
    connect(timer, &QTimer::timeout, this, &Entities::update);
}

int Entities::add()
{
    return eid++;
}

void Entities::update()
{
    emit physicsOutdated();
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
    if (!renderBags.contains(e)) renderBags[e] = bag;
}

void Entities::remove(int e)
{
    Entities::PhysicsBag *physicsBag = physicsBags.value(e, nullptr);
    Entities::RenderBag *renderBag = renderBags.value(e, nullptr);
    emit removedPhysics(physicsBag);
    delete physicsBag;
    delete renderBag;
    physicsBags.remove(e);
    renderBags.remove(e);
}

void Entities::removeAll()
{
    for (int i = 0; i < eid; i++)
    {
        remove(i);
    }
    eid = 0;
}

Entities::~Entities()
{
    removeAll();
}
