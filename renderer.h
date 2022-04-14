#ifndef RENDERER_H
#define RENDERER_H

#include "entities.h"
#include <box2d/box2d.h>

#include <QWidget>

class Renderer : public QWidget, public b2Draw
{
    Q_OBJECT
public:
    explicit Renderer(QWidget *parent = nullptr);
    ~Renderer();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    // Box2D debug drawing functions
    void DrawPolygon(const b2Vec2*, int, const b2Color&);
    void DrawCircle(const b2Vec2&, float, const b2Color&);
    void DrawPoint(const b2Vec2&, float, const b2Color&);
    void DrawSegment(const b2Vec2&, const b2Vec2&, const b2Color&);
    void DrawTransform(const b2Transform&);
    void DrawSolidPolygon(const b2Vec2*, int, const b2Color&);
    void DrawSolidCircle(const b2Vec2&, float, const b2Vec2&, const b2Color&);

public slots:
    void update(Entities::PhysicsBags, Entities::RenderBags);

signals:
    void mousePressed(QPoint);
    void debugRenderQueued();

private:
    const float radiansToDegrees = 180.f / M_PI;
    QMap<QString, QImage> images;
    QRect renderArea;
    QPixmap debugRenderBuffer;
    // Cannot signal to the paintEvent directly, so bag collections are stored as members for use in paintEvent
    Entities::PhysicsBags physicsBags;
    Entities::RenderBags renderBags;
};

#endif // RENDERER_H
