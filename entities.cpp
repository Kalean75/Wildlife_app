#include "entities.h"

Entities::Entities(QObject *parent)
    : QObject(parent),
      renderTimer(new QTimer()),
      frameTimer(new QElapsedTimer())
{
    renderTimer->setTimerType(Qt::PreciseTimer);
    renderTimer->start(renderRate * msPerSecond);
    frameTimer->start();
    connect(renderTimer, &QTimer::timeout, this, &Entities::render);
    update();
}

int Entities::add()
{
    return eid++;
}

void Entities::update()
{
    int updateMsDelta = updateRate * msPerSecond;
    accumulator += frameTimer->restart();
    // Fixed timestep ensures that on average, simulation updates every fixed time interval regardless of framerate
    while (accumulator >= updateMsDelta)
    {
        emit physicsOutdated();
        accumulator -= updateMsDelta;
    }
    QTimer::singleShot(updateMsDelta - accumulator, this, &Entities::update);
}

void Entities::render()
{
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
    delete renderTimer;
    delete frameTimer;
}
