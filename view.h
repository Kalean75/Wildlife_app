#ifndef VIEW_H
#define VIEW_H

#include "entities.h"
#include "physics.h"
#include "renderer.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT
public:
    View(Entities& entities, Physics& physics, Renderer& renderer, QWidget *parent = nullptr);
    ~View();

private:
    Ui::View *ui;
    void startGame();
    Renderer renderer;
    Physics physics;
    Entities entities;
};

#endif // VIEW_H
