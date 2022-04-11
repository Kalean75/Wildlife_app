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
        float x;
        float y;
        float w;
        float h;
        float density = 1.0f;
        float friction = 1.0f;
        float restitution = 0;
        float angle = 0;
        b2BodyType type = b2_dynamicBody;
    };
    struct RenderBag
    {
        QString imageName;
    };
    int newEntity();
    void addPhysics(int, PhysicsBag*);
    void addRender(int, RenderBag*);

public slots:
    void updateEntities();

signals:
    void addedPhysics(Entities::PhysicsBag*);
    void physicsOutdated(float);
    void renderOutdated(QMap<int, Entities::PhysicsBag*>, QMap<int, Entities::RenderBag*>);

private:
    const float timeStep = 1 / 60.0f;
    int eid = 0;
    QTimer *timer;
    QMap<int, PhysicsBag*> physicsBags;
    QMap<int, RenderBag*> renderBags;
};

#endif // ENTITIES_H
