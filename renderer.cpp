#include "renderer.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDir>

Renderer::Renderer(QWidget *parent) : QWidget(parent)
{
    foreach(const QString& imageName, QDir(":").entryList() )
    {
        images.insert(imageName, QImage(":/" + imageName));
    }
}

void Renderer::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    for (Entities::RenderBags::iterator i = renderBags.begin(); i != renderBags.end(); ++i)
    {
        int entity = i.key();
        if (physicsBags.contains(entity))
        {
            Entities::RenderBag *renderBag = i.value();
            Entities::PhysicsBag *physicsBag = physicsBags[entity];
            QImage image = images.value(renderBag->imageName);
            QRect paintArea = e->rect();
            painter.save();
            painter.translate(paintArea.center() + QPointF(physicsBag->x, physicsBag->y));
            painter.rotate(physicsBag->angle * radiansToDegrees);
            painter.drawImage(-image.rect().center(), image);
            painter.restore();
        }
    }
    painter.end();
}

void Renderer::updateRenderer(Entities::PhysicsBags newPhysicsBags, Entities::RenderBags newRenderBags)
{
    physicsBags = newPhysicsBags;
    renderBags = newRenderBags;
    update();
}

Renderer::~Renderer()
{
}
