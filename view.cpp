#include "view.h"
#include "ui_view.h"

View::View(Physics& physics, Renderer& renderer, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::View)
{
    ui->setupUi(this);
    ui->rootLayout->addWidget(&renderer);
    connect(&physics, &Physics::worldUpdated, &renderer, &Renderer::repaint);
}

View::~View()
{
    delete ui;
}
