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
    static const int maxRGB = 255;
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
    enum ThemeKey
    {
        BG,
        Terrain,
        Grass1,
        Grass2,
        Grass3
    };
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
    struct DebugCircle
    {
        QPointF center;
        QPointF axis;
        QColor color;
        float radius;
    };
    const int cullingMargin = 500;
    const int grassWidth = 170;
    const int grassBorderWidth = 18;
    const int debugStrokeWidth = 10;
    const float debugAlphaScale = 0.5f;
    const float cameraScaleStep = 0.2f;
    const float minCameraScale = 0.3f;
    const float maxCameraScale = 1.3f;
    float cameraScale = 0.5f;
    bool debugging = false;
    QMap<ThemeKey, QColor> theme =
    {
        {BG, QColor("#cfeffc")},
        {Terrain, QColor("#d26b4c")},
        {Grass1, QColor("#c26246")},
        {Grass2, QColor("#f78d3b")},
        {Grass3, QColor("#ff913c")},
    };
    QMap<QString, QImage> images;
    QVector<DebugPolygon> debugPolygons;
    QVector<DebugLine> debugLines;
    QVector<DebugCircle> debugCircles;
    QRect camera;
    QPoint cameraPanningBuffer;
    // Cannot signal to the paintEvent directly, so bag collections are stored as members for use in paintEvent
    Entities::PhysicsBags physicsBags;
    Entities::RenderBags renderBags;
    QColor parseB2Color(b2Color);
    bool isCulled(QPointF);
};

#endif // RENDERER_H
