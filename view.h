#ifndef VIEW_H
#define VIEW_H

#include "entities.h"
#include "physics.h"
#include "renderer.h"
#include "quiz.h"
#include "terrain.h"
#include <QMessageBox>

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
    void beastiaryButtonPressed();
    void backButtonPressed();
    void helpButtonPressed();

private:
    Entities entities;
    Physics physics;
    Renderer renderer;
    Quiz quiz;
    Terrain terrain;
    QMessageBox helpBox;
    void showHelpBox();
    bool helpBoxShown;
    Ui::View *ui;
};

#endif // VIEW_H
