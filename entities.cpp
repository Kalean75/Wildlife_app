#include "entities.h"

Entities::Entities(QObject *parent) : QObject(parent), frameTimer(new QElapsedTimer())
{
    frameTimer->start();
    update();
}

int Entities::add()
{
    return eid++;
}

void Entities::update()
{
    accumulator += frameTimer->restart();
    // Fixed timestep ensures that on average, simulation updates every fixed time interval regardless of framerate
    while (accumulator >= updateMsDelta)
    {
        emit physicsOutdated();
        accumulator -= updateMsDelta;
    }
    emit quizOutdated(physicsBags, quizBags);
    emit renderOutdated(physicsBags, renderBags);
    QTimer::singleShot(updateMsDelta - accumulator, this, &Entities::update);
}

void Entities::addPhysics(int e, PhysicsBag *physicsBag)
{
    if (!physicsBags.contains(e))
    {
        emit addedPhysics(physicsBag);
        physicsBags[e] = physicsBag;
    }
}

void Entities::addRender(int e, RenderBag *renderBag)
{
    if (!renderBags.contains(e)) renderBags[e] = renderBag;
}

void Entities::addQuiz(int e, QuizBag *quizBag)
{
    if (!quizBags.contains(e)) quizBags[e] = quizBag;
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
    delete frameTimer;
}
