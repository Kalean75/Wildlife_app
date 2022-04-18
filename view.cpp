#include "view.h"
#include "ui_view.h"

#include <QRandomGenerator>

View::View(QWidget *parent) : QMainWindow(parent), ui(new Ui::View)
{
    ui->setupUi(this);
    ui->renderLayout->addWidget(&renderer);
    // main menu to load first
    ui->applicationStack->setCurrentWidget(ui->mainMenu);
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
    int deer = entities.add();
    Entities::PhysicsBag *deerPhysics = new Entities::PhysicsBag;
    deerPhysics->y = -500.f;
    deerPhysics->w = 256.f;
    deerPhysics->h = 256.f;
    deerPhysics->restitution = 0.5f;
    Entities::RenderBag *deerRender = new Entities::RenderBag;
    deerRender->imageName = "deer";
    Entities::QuizBag *deerQuiz = new Entities::QuizBag;
    deerQuiz->answerID = Quiz::Answer::Deer;
    entities.addPhysics(deer, deerPhysics);
    entities.addRender(deer, deerRender);
    entities.addQuiz(deer, deerQuiz);
    // Terrain generation test
    struct TerrainGenerator
    {
        QVector<QPoint> vertices;
        QVector<QPointF> edgeVertices; // Vertex could be sampled from here to spawn an entity at a random point
        // Parameters can be adjusted for "noisiness"
        int segmentsPerEdge = 10;
        int rangeY = 500;
        int minDx = 750;
        int minDy = 200;
        int rangeDx = 1500;
        int rangeDy = 300;
        void buildVertices(int vertexCount, QPoint startingVertex = QPoint())
        {
            QPoint vertex(startingVertex);
            for (int i = 0; i < vertexCount; i++)
            {
                vertices.append(vertex);
                vertex.rx() += QRandomGenerator::global()->generate() % rangeDx + minDx;
                int testY;
                do
                {
                    testY = vertex.y() + (QRandomGenerator::global()->generate() % rangeDy + minDy) * qCos(M_PI * (i % 2));
                }
                while (qAbs(testY) - (qAbs(startingVertex.y()) - rangeY / 2) > rangeY);
                vertex.setY(testY);
                if (i > 0) buildCurveSegments(i - 1, i);
            }
        }
        void buildCurveSegments(int vertexIndex1, int vertexIndex2)
        {
            QPointF vertex1 = vertices.at(vertexIndex1);
            QPointF vertex2 = vertices.at(vertexIndex2);
            float midpointY = (vertex1 + vertex2).y() / 2.f;
            edgeVertices.append(vertex1);
            for (int i = 0; i < segmentsPerEdge; i++)
            {
                QPointF v;
                v.setX(vertex1.x() + i * (vertex2 - vertex1).x() / segmentsPerEdge);
                v.setY(midpointY - (midpointY - vertex1.y()) * qCos(i * M_PI / segmentsPerEdge));
                edgeVertices.append(v);
            }
        }
    };
    TerrainGenerator generator;
    generator.buildVertices(50, QPoint(-10000, 0));
    for (int i = 1; i < generator.edgeVertices.size(); i++)
    {
        int edge = entities.add();
        Entities::PhysicsBag *edgePhysics = new Entities::PhysicsBag{};
        QPointF v1 = generator.edgeVertices.at(i - 1);
        QPointF v2 = generator.edgeVertices.at(i);
        edgePhysics->x = v1.x();
        edgePhysics->y = v1.y();
        edgePhysics->x1 = v2.x();
        edgePhysics->y1 = v2.y();
        edgePhysics->shapeType = b2Shape::e_edge;
        edgePhysics->bodyType = b2BodyType::b2_staticBody;
        entities.addPhysics(edge, edgePhysics);
    }
    quiz.startQuiz(Quiz::Difficulty::Easy);
    ui->applicationStack->setCurrentWidget(ui->game);
}
void View::beastiaryButtonPressed()
{
    //open bestiary
    ui->applicationStack->setCurrentWidget(ui->bestiary);
}
void View::backButtonPressed()
{
    ui->applicationStack->setCurrentWidget(ui->mainMenu);
}

View::~View()
{
    delete ui;
}



