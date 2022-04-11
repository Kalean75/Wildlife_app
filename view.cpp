#include "view.h"
#include "ui_view.h"

View::View(Entities& entities, Physics& physics, Renderer& renderer, QWidget *parent) : QMainWindow(parent), ui(new Ui::View)
{
    ui->setupUi(this);
    ui->rootLayout->addWidget(&renderer);
    // Entity event connections
    connect(&entities, &Entities::addedPhysics, &physics, &Physics::createBody);
    connect(&entities, &Entities::physicsOutdated, &physics, &Physics::updateWorld);
    connect(&entities, &Entities::renderOutdated, &renderer, &Renderer::rerender);
    // Entity initialization
    int ground = entities.newEntity();
    int deer = entities.newEntity();
    Entities::PhysicsBag *groundPhysics = new Entities::PhysicsBag{.x = -200, .y = 0, .w = 400, .h = 10, .type = b2BodyType::b2_staticBody};
    Entities::PhysicsBag *deerPhysics = new Entities::PhysicsBag{.x = 0, .y = -100, .w = 1, .h = 1, .restitution = 1.0f};
    Entities::RenderBag *deerRender = new Entities::RenderBag{.imageName = "deer"};
    entities.addPhysics(ground, groundPhysics);
    entities.addPhysics(deer, deerPhysics);
    entities.addRender(deer, deerRender);
}

View::~View()
{
    delete ui;
}
