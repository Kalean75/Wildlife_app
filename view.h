#ifndef VIEW_H
#define VIEW_H

#include "entities.h"
#include "physics.h"
#include "renderer.h"
#include "quiz.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT
public:
    View(QWidget *parent = nullptr);
    ~View();

public slots:
    void startGameButtonPressed();

private slots:
    void on_beastiaryButton_pressed();

private:
    Entities entities;
    Physics physics;
    Renderer renderer;
    Quiz quiz;
    Ui::View *ui;
};

#endif // VIEW_H
