#ifndef ENTITIES_H
#define ENTITIES_H

#include <box2d/box2d.h>

#include <QObject>
#include <QMap>
#include <QTimer>

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
        b2BodyType type = b2_dynamicBody;
    };
    struct RenderBag
    {
        QString imageName = "";
    };
    typedef QMap<int, PhysicsBag*> PhysicsBags;
    typedef QMap<int, RenderBag*> RenderBags;
    int newEntity();
    void addPhysics(int, PhysicsBag*);
    void addRender(int, RenderBag*);

public slots:
    void updateEntities();

signals:
    void addedPhysics(Entities::PhysicsBag*);
    void physicsOutdated(float);
    void renderOutdated(Entities::PhysicsBags, Entities::RenderBags);

private:
    const float timeStep = 1.f / 60.f;
    int eid = 0;
    QTimer *timer;
    PhysicsBags physicsBags;
    RenderBags renderBags;
};

#endif // ENTITIES_H
