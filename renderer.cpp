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
    painter.scale(cameraScale, cameraScale);
    painter.translate(camera.center());
    for (Entities::PhysicsBags::iterator i = physicsBags.begin(); i != physicsBags.end(); i++)
    {
        int entity = i.key();
        if (renderBags.contains(entity))
        {
            Entities::PhysicsBag *physicsBag = i.value();
            Entities::RenderBag *renderBag = renderBags[entity];
            QImage image = images.value(renderBag->imageName);
            painter.save();
            painter.setRenderHints(QPainter::SmoothPixmapTransform);
            painter.translate(physicsBag->x, physicsBag->y);
            painter.rotate(qRadiansToDegrees(physicsBag->angle));
            painter.drawImage(-image.rect().center(), image);
            painter.restore();
        }
    }
    emit debugRenderQueued();
    // Edge vertex data is already collected for the debug view, so it makes more sense to reuse that instead of recreating it from the physics bag
    for (const DebugLine& line : debugLines)
    {
        QPointF edgeDepth(0, camera.height());
        QVector<QPointF> vertices = {line.v1, line.v2, line.v2 + edgeDepth, line.v1 + edgeDepth};
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing);
        painter.setBrush(QBrush(Qt::lightGray));
        painter.setPen(QPen(painter.brush(), 2.f)); // Increased stroke width to eliminate polygon gaps on lower zoom scales
        painter.drawPolygon(vertices.data(), vertices.size());
        painter.restore();
    }
    if (debugging)
    {
        for (const DebugPolygon& polygon : debugPolygons)
        {
            painter.setBrush(QBrush(polygon.color));
            painter.drawPolygon(polygon.vertices.data(), polygon.vertices.size());
        }
        for (const DebugLine& line : debugLines)
        {
            painter.setBrush(QBrush(line.color));
            painter.drawLine(line.v1, line.v2);
        }
    }
    debugPolygons.clear();
    debugLines.clear();
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

QColor Renderer::parseB2Color(b2Color color)
{
    return QColor::fromRgbF(color.r, color.g, color.b, color.a / 2.f);
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

void Renderer::DrawSegment(const b2Vec2& v1, const b2Vec2& v2, const b2Color& color)
{
    DebugLine line{};
    line.v1 = QPointF(v1.x, v1.y) * Physics::pixelsPerMeter;
    line.v2 = QPointF(v2.x, v2.y) * Physics::pixelsPerMeter;
    line.color = parseB2Color(color);
    debugLines.append(line);
}

void Renderer::DrawTransform(const b2Transform&)
{
}

void Renderer::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color& color)
{
    QVector<QPointF> copiedVertices;
    for (int i = 0; i < vertexCount; i++)
    {
        copiedVertices.append(QPointF(vertices[i].x, vertices[i].y) * Physics::pixelsPerMeter);
    }
    debugPolygons.append(DebugPolygon{copiedVertices, parseB2Color(color)});
}

void Renderer::DrawSolidCircle(const b2Vec2&, float, const b2Vec2&, const b2Color&)
{
}
