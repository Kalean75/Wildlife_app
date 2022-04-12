#ifndef RENDERER_H
#define RENDERER_H

#include "entities.h"

#include <QWidget>

class Renderer : public QWidget
{
    Q_OBJECT
public:
    explicit Renderer(QWidget *parent = nullptr);
    ~Renderer();
    void paintEvent(QPaintEvent *);

public slots:
    void updateRenderer(Entities::PhysicsBags, Entities::RenderBags);

private:
    QMap<QString, QImage*> images;
    // Cannot signal to the paintEvent directly, so bag collections are stored as members for use in paintEvent
    Entities::PhysicsBags physicsBags;
    Entities::RenderBags renderBags;
};

#endif // RENDERER_H
