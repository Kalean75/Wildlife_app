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
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
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
    void toggleDebugging(bool);

signals:
    void mousePressed(QPoint);
    void debugRenderQueued();

private:
    struct DebugPolygon
    {
        QVector<QPointF> vertices;
        QColor color;
    };
    struct DebugLine
    {
        QPointF vertex1;
        QPointF vertex2;
        QColor color;
    };
    const int cullingMargin = 250;
    const float debugAlphaScale = 0.5f;
    const float cameraScaleStep = 0.25f;
    const float minCameraScale = 0.5f;
    const float maxCameraScale = 1.5f;
    float cameraScale = 1.f;
    bool debugging = false;
    QMap<QString, QImage> images;
    QVector<DebugPolygon> debugPolygons;
    QVector<DebugLine> debugLines;
    QRect camera;
    QPoint cameraPanningBuffer;
    // Cannot signal to the paintEvent directly, so bag collections are stored as members for use in paintEvent
    Entities::PhysicsBags physicsBags;
    Entities::RenderBags renderBags;
    QColor parseB2Color(b2Color);
    bool isCulled(QPointF);
    bool isCulled(QVector<QPointF>);
};

#endif // RENDERER_H
