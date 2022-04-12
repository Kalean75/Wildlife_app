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
    QMap<int, Entities::RenderBag*>::iterator i;
    for (i = renderBags.begin(); i != renderBags.end(); ++i)
    {
        int entity = i.key();
        if (physicsBags.contains(entity))
        {
            Entities::RenderBag *renderBag = i.value();
            Entities::PhysicsBag *physicsBag = physicsBags[entity];
            QImage *image = images[renderBag->imageName];
            QRect paintArea = e->rect();
            painter.drawImage(paintArea.center() + QPointF(physicsBag->x, physicsBag->y) - image->rect().center(), *image);
        }
    }
    painter.end();
}

void Renderer::updateRenderer(QMap<int, Entities::PhysicsBag*> newPhysicsBags, QMap<int, Entities::RenderBag*> newRenderBags)
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
