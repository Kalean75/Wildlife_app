#ifndef PHYSICS_H
#define PHYSICS_H

#include <box2d/box2d.h>

#include <QObject>
#include <QTimer>
#include <QPointF>

class Physics : public QObject
{
    Q_OBJECT
public:
    explicit Physics(QObject *parent = nullptr);
    ~Physics();

public slots:
    void updateWorld();

signals:
    void worldUpdated(const QVector<QPointF>&);

private:
    const int msPerSecond = 1000;
    const int velocityIterations = 6;
    const int positionIterations = 2;
    const float timeStep = 1 / 60.0f;
    const float pixelsPerMeter = 32.0f;
    const b2Vec2 gravity = b2Vec2(0, 9.8f);
    b2World *world;
    QTimer *timer;
    QVector<QPointF> positions;
};

#endif // PHYSICS_H
