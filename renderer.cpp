#include "renderer.h"

#include <QPainter>
#include <QPaintEvent>

Renderer::Renderer(QWidget *parent) : QWidget(parent)
{
    images.insert("deer", new QImage(":/deer"));
}

void Renderer::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    Entities::RenderBags::iterator i;
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    for (i = renderBags.begin(); i != renderBags.end(); ++i)
    {
        int entity = i.key();
        if (physicsBags.contains(entity))
        {
            Entities::RenderBag *renderBag = i.value();
            Entities::PhysicsBag *physicsBag = physicsBags[entity];
            QImage *image = images[renderBag->imageName];
            QRect paintArea = e->rect();
            painter.save();
            painter.translate(paintArea.center() + QPointF(physicsBag->x, physicsBag->y));
            painter.rotate(physicsBag->angle * radiansToDegrees);
            painter.drawImage(-image->rect().center(), *image);
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
    QMap<QString, QImage*>::iterator i;
    for (i = images.begin(); i != images.end(); ++i)
    {
        delete i.value();
    }
}
