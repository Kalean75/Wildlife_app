#include "renderer.h"
#include "physics.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QDir>

Renderer::Renderer(QWidget *parent) : QWidget(parent)
{
    SetFlags(b2Draw::e_shapeBit); // https://box2d.org/documentation/classb2_draw.html
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(QString("background-color: %1;").arg(theme.value(BG).name()));
    foreach(const QString& imageName, QDir(":").entryList() )
    {
        images.insert(imageName, QImage(":/" + imageName).convertToFormat(QImage::Format_ARGB32_Premultiplied)); // Premultiplied supposed to be more performant
    }
}

void Renderer::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPainterPath path;
    camera.setSize(e->rect().size() / cameraScale);
    painter.scale(cameraScale, cameraScale);
    painter.translate(camera.center());
    for (Entities::PhysicsBags::iterator i = physicsBags.begin(); i != physicsBags.end(); i++)
    {
        int entity = i.key();
        Entities::PhysicsBag *physicsBag = i.value();
        if (renderBags.contains(entity))
        {
            Entities::RenderBag *renderBag = renderBags[entity];
            QImage image = images.value(renderBag->imageName);
            if (!isCulled(QPointF(physicsBag->x, physicsBag->y)))
            {
                painter.save();
                painter.setRenderHints(QPainter::SmoothPixmapTransform);
                painter.translate(physicsBag->x, physicsBag->y);
                painter.rotate(physicsBag->angle * 180 / M_PI);
                painter.scale(renderBag->mirrorX, renderBag->mirrorY);
                painter.drawImage(-image.rect().center(), image);
                painter.restore();
            }
        }
        // May be possible to move render vertex data directly into the render bag, eliminating the need for this extra case
        else if (physicsBag->shapeType == b2Shape::e_edge)
        {
            QPointF vertex1(physicsBag->x, physicsBag->y);
            QPointF vertex2(physicsBag->x1, physicsBag->y1);
            if (!isCulled(vertex1) && !isCulled(vertex2))
            {
                if (path.isEmpty()) path.moveTo(vertex1);
                path.lineTo(vertex2);
            }
        }
    }
    QPainterPath polygon(path);
    if (!path.isEmpty())
    {
        QPointF expand(0, camera.height());
        polygon.lineTo(path.currentPosition() + expand);
        polygon.lineTo(path.elementAt(0) + expand);
        polygon.closeSubpath();
    }
    painter.setRenderHints(QPainter::Antialiasing);
    painter.fillPath(polygon, QBrush(theme.value(Terrain)));
    painter.fillPath(polygon.subtracted(polygon.translated(0, grassWidth + grassBorderWidth)), theme.value(Grass1));
    painter.fillPath(polygon.subtracted(polygon.translated(0, grassWidth)), theme.value(Grass2));
    painter.fillPath(polygon.subtracted(polygon.translated(0, grassBorderWidth)), theme.value(Grass3));
    if (debugging)
    {
        emit debugRenderQueued();
        painter.setRenderHints(QPainter::Antialiasing, false);
        for (const DebugPolygon& polygon : debugPolygons)
        {
            painter.setBrush(QBrush(polygon.color));
            painter.drawPolygon(polygon.vertices.data(), polygon.vertices.size());
        }
        for (const DebugLine& line : debugLines)
        {
            painter.setBrush(QBrush(line.color));
            painter.drawLine(line.vertex1, line.vertex2);
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
    return QColor::fromRgbF(color.r, color.g, color.b, color.a * debugAlphaScale);
}

bool Renderer::isCulled(QPointF vertex)
{
    // If entity size is not taken into account when culling, sprites will "pop in"
    // Tried passing a QRect to the contains method, but it does not work as expected, so an added margin/buffer was used instead as a quick fix
    QRectF viewport(-camera.center(), camera.size());
    viewport.translate(-cullingMargin, -cullingMargin);
    viewport.setSize(viewport.size() + QSize(cullingMargin, cullingMargin) * 2);
    return !viewport.contains(vertex);
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

void Renderer::DrawSegment(const b2Vec2& vertex1, const b2Vec2& vertex2, const b2Color& color)
{
    DebugLine line{};
    line.vertex1 = QPointF(vertex1.x, vertex1.y) * Physics::pixelsPerMeter;
    line.vertex2 = QPointF(vertex2.x, vertex2.y) * Physics::pixelsPerMeter;
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
