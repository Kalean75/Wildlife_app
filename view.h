#ifndef VIEW_H
#define VIEW_H

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
    View(Physics& physics, Renderer& renderer, QWidget *parent = nullptr);
    ~View();

private:
    Ui::View *ui;
};
#endif // VIEW_H
