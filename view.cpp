#include "view.h"
#include "ui_view.h"

View::View(QWidget *parent) : QMainWindow(parent), ui(new Ui::View)
{
    ui->setupUi(this);
    ui->renderLayout->addWidget(&renderer);
    ui->applicationStack->setCurrentWidget(ui->mainMenu); // Ensure main menu is always presented first regardless of designer state
    physics.setDebugRenderer(renderer);
    // Entity event connections
    connect(&entities, &Entities::addedPhysics, &physics, &Physics::addBody);
    connect(&entities, &Entities::removedPhysics, &physics, &Physics::removeBody);
    connect(&entities, &Entities::physicsOutdated, &physics, &Physics::update);
    connect(&entities, &Entities::renderOutdated, &renderer, &Renderer::update);
    connect(&entities, &Entities::quizOutdated, &quiz, &Quiz::update);
    // Renderer connections
    connect(&renderer, &Renderer::mousePressed, &physics, &Physics::queryPoint);
    connect(&renderer, &Renderer::debugRenderQueued, &physics, &Physics::debugRender);
    // Physics connections
    connect(&physics, &Physics::bodyQueried, &quiz, &Quiz::queueAnswer);
    // Quiz connections
    connect(&quiz, &Quiz::questionChanged, ui->quizLabel, &QLabel::setText);
    connect(&quiz, &Quiz::quizFinished, ui->quizLabel, &QLabel::setText);
    // Interface connections
    connect(ui->debugRenderCheckBox, &QCheckBox::stateChanged, &renderer, &Renderer::toggleDebugging);
    connect(ui->startGameButton, &QPushButton::clicked, this, &View::startGameButtonPressed);
    connect(ui->beastiaryButton, &QPushButton::clicked, this, &View::beastiaryButtonPressed);
    connect(ui->beastiaryBackButton, &QPushButton::clicked, this, &View::backButtonPressed);
    connect(ui->gameBackButton, &QPushButton::clicked, this, &View::backButtonPressed);
}

void View::startGameButtonPressed()
{
    // Entity initialization
    // TODO: Move all these hard-coded values to a spawner system, or at least some sort of centralized, well-defined location
    for (int i = -10; i < 60; i++)
    {
        int cloud = entities.add();
        Entities::PhysicsBag *cloudPhysics = new Entities::PhysicsBag;
        Entities::RenderBag *cloudRender = new Entities::RenderBag;
        cloudPhysics->x = i * 1024.f;
        cloudPhysics->y = -300.f;
        cloudPhysics->isSensor = true;
        cloudPhysics->bodyType = b2BodyType::b2_staticBody;
        cloudRender->imageName = QString("cloud%1").arg(QString::number(random(1, 2)));
        entities.addPhysics(cloud, cloudPhysics);
        entities.addRender(cloud, cloudRender);
    }
    int deer = entities.add();
    Entities::PhysicsBag *deerPhysics = new Entities::PhysicsBag;
    Entities::RenderBag *deerRender = new Entities::RenderBag;
    Entities::QuizBag *deerQuiz = new Entities::QuizBag;
    deerPhysics->y = -500.f;
    deerPhysics->w = 256.f;
    deerPhysics->h = 240.f;
    deerPhysics->restitution = 0.5f;
    deerRender->imageName = "deer";
    deerQuiz->answerID = Quiz::Answer::Deer;
    entities.addPhysics(deer, deerPhysics);
    entities.addRender(deer, deerRender);
    entities.addQuiz(deer, deerQuiz);
    // Terrain initialization
    Terrain::Vertices vertices = terrain.buildVertices(50, QPoint(-10000, 200));
    for (int i = 1; i < vertices.size(); i++)
    {
        int edge = entities.add();
        Entities::PhysicsBag *edgePhysics = new Entities::PhysicsBag;
        QPointF v1 = vertices.at(i - 1);
        QPointF v2 = vertices.at(i);
        edgePhysics->x = v1.x();
        edgePhysics->y = v1.y();
        edgePhysics->x1 = v2.x();
        edgePhysics->y1 = v2.y();
        edgePhysics->shapeType = b2Shape::e_edge;
        edgePhysics->bodyType = b2BodyType::b2_staticBody;
        entities.addPhysics(edge, edgePhysics);
    }
    for (int i = 0; i < 100; i++)
    {
        int bush = entities.add();
        int bushIndex = qMax(1, random(0, static_cast<int>(vertices.size() - 1)));
        QPointF bushVertex = vertices.at(bushIndex);
        QPointF previousBushVertex = vertices.at(bushIndex - 1);
        Entities::PhysicsBag *bushPhysics = new Entities::PhysicsBag;
        Entities::RenderBag *bushRender = new Entities::RenderBag;
        if (i % 2 == 0)
        {
            int tree = entities.add();
            QPointF treeVertex = vertices.at(random(0, static_cast<int>(vertices.size() - 1)));
            Entities::PhysicsBag *treePhysics = new Entities::PhysicsBag;
            Entities::RenderBag *treeRender = new Entities::RenderBag;
            treePhysics->x = treeVertex.x();
            treePhysics->y = treeVertex.y() - 250.f;
            treePhysics->isSensor = true;
            treePhysics->bodyType = b2BodyType::b2_staticBody;
            treeRender->imageName = "tree";
            treeRender->mirrorX = std::cos(random(0, 1) * M_PI);
            entities.addPhysics(tree, treePhysics);
            entities.addRender(tree, treeRender);
        }
        bushPhysics->x = bushVertex.x();
        bushPhysics->y = bushVertex.y() - 45.f;
        bushPhysics->angle = std::atan2(bushVertex.y() - previousBushVertex.y(), bushVertex.x() - previousBushVertex.x());;
        bushPhysics->isSensor = true;
        bushPhysics->bodyType = b2BodyType::b2_staticBody;
        bushRender->imageName = "bush";
        bushRender->mirrorX = std::cos(random(0, 1) * M_PI);
        entities.addPhysics(bush, bushPhysics);
        entities.addRender(bush, bushRender);
    }
    // Quiz initialization
    quiz.startQuiz(Quiz::Difficulty::Easy);
    // Switch to game state
    ui->applicationStack->setCurrentWidget(ui->game);
}

void View::beastiaryButtonPressed()
{
    ui->applicationStack->setCurrentWidget(ui->bestiary); // Open bestiary
}

void View::backButtonPressed()
{
    entities.removeAll();
    ui->applicationStack->setCurrentWidget(ui->mainMenu);
}

View::~View()
{
    delete ui;
}
