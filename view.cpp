#include "view.h"
#include "ui_view.h"

View::View(Entities& entities, Physics& physics, Renderer& renderer, QWidget *parent) : QMainWindow(parent), ui(new Ui::View)
{
    ui->setupUi(this);
    ui->rootLayout->addWidget(&renderer);
    // Entity event connections
    connect(&entities, &Entities::addedPhysics, &physics, &Physics::addBody);
    connect(&entities, &Entities::removedPhysics, &physics, &Physics::removeBody);
    connect(&entities, &Entities::physicsOutdated, &physics, &Physics::updatePhysics);
    connect(&entities, &Entities::renderOutdated, &renderer, &Renderer::updateRenderer);
    // Entity initialization
    int ground = entities.add();
    int deer = entities.add();
    Entities::PhysicsBag *groundPhysics = new Entities::PhysicsBag{.y = 200.f, .w = 1000.f, .type = b2BodyType::b2_staticBody};
    Entities::PhysicsBag *deerPhysics = new Entities::PhysicsBag{.y = -100.f, .w = 256.f, .h = 256.f, .restitution = 1.f};
    Entities::RenderBag *deerRender = new Entities::RenderBag{.imageName = "deer"};
    entities.addPhysics(ground, groundPhysics);
    entities.addPhysics(deer, deerPhysics);
    entities.addRender(deer, deerRender);
}

View::~View()
{
    delete ui;
}
