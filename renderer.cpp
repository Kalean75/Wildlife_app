#include "renderer.h"

#include <QPainter>
#include <QPaintEvent>

Renderer::Renderer(QWidget *parent)
    : QWidget(parent),
      image(new QImage(":/res/deer.png"))
{
}

void Renderer::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QRect paintArea = e->rect();
    for (const QPointF& position : positions)
    {
        painter.drawImage(paintArea.center() + position - image->rect().center(), *image);
    }
    painter.end();
}

void Renderer::repaint(const QVector<QPointF>& newPositions)
{
    positions = newPositions;
    update();
}

Renderer::~Renderer()
{
    delete image;
}
