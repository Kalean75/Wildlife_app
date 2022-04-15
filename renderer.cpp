#include "renderer.h"
#include "physics.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDir>

Renderer::Renderer(QWidget *parent) : QWidget(parent)
{
    SetFlags(b2Draw::e_shapeBit); // https://box2d.org/documentation/classb2_draw.html
    foreach(const QString& imageName, QDir(":").entryList() )
    {
        images.insert(imageName, QImage(":/" + imageName));
    }
}

void Renderer::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    camera.setSize(e->rect().size() / cameraScale);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    painter.scale(cameraScale, cameraScale);
    painter.translate(camera.center());
    for (Entities::RenderBags::iterator i = renderBags.begin(); i != renderBags.end(); ++i)
    {
        int entity = i.key();
        if (physicsBags.contains(entity))
        {
            Entities::RenderBag *renderBag = i.value();
            Entities::PhysicsBag *physicsBag = physicsBags[entity];
            QImage image = images.value(renderBag->imageName);
            painter.save();
            painter.translate(physicsBag->x, physicsBag->y);
            painter.rotate(qRadiansToDegrees(physicsBag->angle));
            painter.drawImage(-image.rect().center(), image);
            painter.restore();
        }
    }
    if (debugging)
    {
        emit debugRenderQueued();
        for (const DebugPolygon& polygon : debugPolygons)
        {
            painter.setBrush(QBrush(polygon.color));
            painter.drawPolygon(polygon.vertices.data(), polygon.vertices.size());
        }
        debugPolygons.clear();
    }
    painter.end();
}

void Renderer::mousePressEvent(QMouseEvent *e)
{
    emit mousePressed(e->pos() / cameraScale - camera.center());
    cameraPanningBuffer = e->pos();
}

void Renderer::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        camera.translate((e->pos() - cameraPanningBuffer) / cameraScale);
        cameraPanningBuffer = e->pos();
        repaint(); // Purely for smoothness while panning; remove if performance becomes an issue
    }
}

void Renderer::wheelEvent(QWheelEvent *e)
{
    float wheelDelta = QVector2D(e->angleDelta()).normalized().y();
    cameraScale = std::min(std::max(cameraScale + cameraScaleStep * wheelDelta, minCameraScale), maxCameraScale);
}

void Renderer::update(Entities::PhysicsBags newPhysicsBags, Entities::RenderBags newRenderBags)
{
    physicsBags = newPhysicsBags;
    renderBags = newRenderBags;
    repaint();
}

void Renderer::toggleDebugging(bool isDebugging)
{
    debugging = isDebugging;
}

// Box2D debug drawing functions inherited from b2Draw class
// These are purely for rendering Box2D bodies and aren't to be used for anything else

void Renderer::DrawPolygon(const b2Vec2*, int, const b2Color&)
{
}

void Renderer::DrawCircle(const b2Vec2&, float, const b2Color&)
{
}

void Renderer::DrawPoint(const b2Vec2&, float, const b2Color&)
{
}

void Renderer::DrawSegment(const b2Vec2&, const b2Vec2&, const b2Color&)
{
}

void Renderer::DrawTransform(const b2Transform&)
{
}

void Renderer::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color& color)
{
    QVector<QPointF> copiedVertices;
    for (int i = 0; i < vertexCount; i++)
    {
        copiedVertices.append(QPointF(vertices[i].x * Physics::pixelsPerMeter, vertices[i].y * Physics::pixelsPerMeter));
    }
    debugPolygons.append(DebugPolygon{copiedVertices, QColor::fromRgbF(color.r, color.g, color.b, color.a / 2.f)});
}

void Renderer::DrawSolidCircle(const b2Vec2&, float, const b2Vec2&, const b2Color&)
{
}
