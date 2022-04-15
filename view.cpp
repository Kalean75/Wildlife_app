#include "view.h"
#include "ui_view.h"

View::View(Entities& entities, Physics& physics, Renderer& renderer, QWidget *parent) : QMainWindow(parent), ui(new Ui::View)
{
    ui->setupUi(this);
    ui->renderLayout->addWidget(&renderer);
    physics.setDebugRenderer(renderer);
    // Entity event connections
    connect(&entities, &Entities::addedPhysics, &physics, &Physics::addBody);
    connect(&entities, &Entities::removedPhysics, &physics, &Physics::removeBody);
    connect(&entities, &Entities::physicsOutdated, &physics, &Physics::update);
    connect(&entities, &Entities::renderOutdated, &renderer, &Renderer::update);
    // Renderer connections
    connect(&renderer, &Renderer::mousePressed, &physics, &Physics::queryPoint);
    connect(&renderer, &Renderer::debugRenderQueued, &physics, &Physics::debugRender);
    // Interface connections
    connect(ui->debugRenderCheckbox, &QCheckBox::stateChanged, &renderer, &Renderer::toggleDebugging);
    // Entity initialization
    int ground = entities.add();
    int deer = entities.add();
    Entities::PhysicsBag *groundPhysics = new Entities::PhysicsBag{};
    groundPhysics->y = 300.f;
    groundPhysics->w = 5000.f;
    groundPhysics->h = 20.f;
    groundPhysics->type = b2BodyType::b2_staticBody;
    Entities::PhysicsBag *deerPhysics = new Entities::PhysicsBag{};
    deerPhysics->y = -300.f;
    deerPhysics->w = 256.f;
    deerPhysics->h = 256.f;
    deerPhysics->restitution = 0.5f;
    Entities::RenderBag *deerRender = new Entities::RenderBag{};
    deerRender->imageName = "deer";
    entities.addPhysics(ground, groundPhysics);
    entities.addPhysics(deer, deerPhysics);
    entities.addRender(deer, deerRender);
}

View::~View()
{
    delete ui;
}
