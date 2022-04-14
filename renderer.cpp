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
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    renderArea = e->rect();
    for (Entities::RenderBags::iterator i = renderBags.begin(); i != renderBags.end(); ++i)
    {
        int entity = i.key();
        if (physicsBags.contains(entity))
        {
            Entities::RenderBag *renderBag = i.value();
            Entities::PhysicsBag *physicsBag = physicsBags[entity];
            QImage image = images.value(renderBag->imageName);
            painter.save();
            painter.translate(renderArea.center() + QPointF(physicsBag->x, physicsBag->y));
            painter.rotate(physicsBag->angle * radiansToDegrees);
            painter.drawImage(-image.rect().center(), image);
            painter.restore();
        }
    }
    if (debugging)
    {
        debugRenderBuffer = QPixmap(renderArea.size());
        debugRenderBuffer.fill(Qt::transparent);
        emit debugRenderQueued();
        painter.drawPixmap(0, 0, debugRenderBuffer);
    }
    painter.end();
}

void Renderer::mousePressEvent(QMouseEvent *e)
{
    emit mousePressed(e->pos() - renderArea.center());
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

Renderer::~Renderer()
{
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
    QPainter painter(&debugRenderBuffer);
    QPointF points[vertexCount];
    painter.setBrush(QBrush(QColor::fromRgbF(color.r, color.g, color.b, color.a / 2.f)));
    for (int i = 0; i < vertexCount; i++)
    {
        points[i] = QPointF(vertices[i].x * Physics::pixelsPerMeter, vertices[i].y * Physics::pixelsPerMeter);
    }
    painter.save();
    painter.translate(renderArea.center());
    painter.drawPolygon(points, vertexCount);
    painter.restore();
}

void Renderer::DrawSolidCircle(const b2Vec2&, float, const b2Vec2&, const b2Color&)
{
}
