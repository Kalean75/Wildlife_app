#ifndef ENTITIES_H
#define ENTITIES_H

#include <box2d/box2d.h>

#include <QObject>
#include <QMap>
#include <QTimer>
#include <QElapsedTimer>

class Entities : public QObject
{
    Q_OBJECT
public:
    explicit Entities(QObject *parent = nullptr);
    ~Entities();
    struct PhysicsBag
    {
        float x = 0.f;
        float y = 0.f;
        float w = 1.f;
        float h = 1.f;
        float angle = 0.f;
        float density = 1.f;
        float friction = 1.f;
        float restitution = 0.f;
        float linearDamping = 0.f;
        float angularDamping = 0.5f;
        b2BodyType type = b2_dynamicBody;
    };
    struct RenderBag
    {
        QString imageName = "";
    };
    typedef QMap<int, PhysicsBag*> PhysicsBags;
    typedef QMap<int, RenderBag*> RenderBags;
    static constexpr float updateRate = 1.f / 120.f;
    int add();
    void addPhysics(int, PhysicsBag*);
    void addRender(int, RenderBag*);
    void remove(int);
    void removeAll();

signals:
    void addedPhysics(Entities::PhysicsBag*);
    void removedPhysics(Entities::PhysicsBag*);
    void physicsOutdated();
    void renderOutdated(Entities::PhysicsBags, Entities::RenderBags);

private:
    const int updateMsDelta = updateRate * 1000;
    int eid = 0;
    int accumulator = 0;
    QElapsedTimer *frameTimer;
    PhysicsBags physicsBags;
    RenderBags renderBags;
    void update();
};

#endif // ENTITIES_H
