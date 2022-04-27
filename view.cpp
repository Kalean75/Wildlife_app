#include "view.h"
#include "ui_view.h"

#include <QPainter>
#include <QRegularExpression>

View::View(QWidget *parent) : QMainWindow(parent), ui(new Ui::View)
{
    ui->setupUi(this);
    ui->renderLayout->addWidget(&renderer);
    ui->applicationStack->setCurrentWidget(ui->mainMenu); // Ensure main menu is always presented first regardless of designer state
    helpBox.setWindowTitle("How to Play");
    helpBox.setText(helpBoxWelcomeText);
    physics.setDebugRenderer(renderer);
    // Load all bestiary silhouettes from their corresponding animal images
    QRegularExpression imageNameTest("(.+)Image$");
    for (QLabel* label : ui->bestiary->findChildren<QLabel*>(imageNameTest))
    {
        QSize labelSize(label->width(), label->height());
        QPixmap labelPixmap(labelSize);
        QPainter painter;
        labelPixmap.fill(QColor(0, 0, 0, Renderer::rgbMaxComponent - 1)); // Fill color cannot be fully opaque or the composition blending does not work
        painter.begin(&labelPixmap);
        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        painter.drawPixmap(0, 0, QPixmap::fromImage(QImage(":/" + imageNameTest.match(label->objectName()).captured(1)).scaled(labelSize)));
        label->setPixmap(labelPixmap);
        painter.end();
    }
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
    connect(&quiz, &Quiz::quizFinished, this, &View::endOfQuizPopUp);
    // ResultsPage
    connect(&quiz, &Quiz::sendResults, this, &View::endOfQuizResults);
    // Interface connections
    connect(ui->debugRenderCheckBox, &QCheckBox::stateChanged, &renderer, &Renderer::toggleDebugging);
    connect(ui->startGameButton, &QPushButton::clicked, this, &View::startGameButtonHandler);
    connect(ui->bestiaryButton, &QPushButton::clicked, this, &View::bestiaryButtonPressed);
    connect(ui->bestiaryBackButton, &QPushButton::clicked, this, &View::backButtonPressed);
    connect(ui->gameBackButton, &QPushButton::clicked, this, &View::backButtonPressed);
    connect(ui->helpButton, &QPushButton::clicked, this, &View::helpButtonPressed);
}
//when a new game is started, start on easy difficulty
void View::startGameButtonHandler(){
    currDiff = Quiz::Difficulty::Easy;
    startGame(currDiff);
    ui->difficultyLabel->setText("Level: Easy");
}

QPointF sampleVertex(Terrain::Vertices& vertices)
{
    return vertices.takeAt(random(0, static_cast<int>(vertices.size() - 1)));
}

//creates animals in view
void View::createAnimalEntities(Terrain::Vertices vertices)
{
    //deer
    int deer = entities.add();
    QPointF deerPos = sampleVertex(vertices);
    Entities::PhysicsBag *deerPhysics = new Entities::PhysicsBag;
    Entities::RenderBag *deerRender = new Entities::RenderBag;
    Entities::QuizBag *deerQuiz = new Entities::QuizBag;
    deerPhysics->w = 256.f;
    deerPhysics->h = 230.f;
    deerPhysics->x = deerPos.x();
    deerPhysics->y = deerPos.y();
    deerRender->imageName = "deer";
    deerQuiz->answerID = Quiz::Answer::Deer;
    entities.addPhysics(deer, deerPhysics);
    entities.addRender(deer, deerRender);
    entities.addQuiz(deer, deerQuiz);

    //turtle
    int turtle = entities.add();
    QPointF turtlePos = sampleVertex(vertices);
    Entities::PhysicsBag *turtlePhysics = new Entities::PhysicsBag;
    Entities::RenderBag *turtleRender = new Entities::RenderBag;
    Entities::QuizBag *turtleQuiz = new Entities::QuizBag;
    turtlePhysics->w = 128.f;
    turtlePhysics->h = 75.f;
    turtlePhysics->x = turtlePos.x();
    turtlePhysics->y = turtlePos.y();
    turtleRender->imageName = "turtle";
    turtleRender->scaleX = 0.5f;
    turtleRender->scaleY = 0.5f;
    turtleQuiz->answerID = Quiz::Answer::Turtle;
    entities.addPhysics(turtle, turtlePhysics);
    entities.addRender(turtle, turtleRender);
    entities.addQuiz(turtle, turtleQuiz);

    //squirrel
    int squirrel = entities.add();
    QPointF squirrelPos = sampleVertex(vertices);
    Entities::PhysicsBag *squirrelPhysics = new Entities::PhysicsBag;
    Entities::RenderBag *squirrelRender = new Entities::RenderBag;
    Entities::QuizBag *squirrelQuiz = new Entities::QuizBag;
    squirrelPhysics->w = 128.f;
    squirrelPhysics->h = 103.f;
    squirrelPhysics->x = squirrelPos.x();
    squirrelPhysics->y = squirrelPos.y();
    squirrelRender->imageName = "squirrel";
    squirrelRender->scaleX = 0.5f;
    squirrelRender->scaleY = 0.5f;
    squirrelQuiz->answerID = Quiz::Answer::Squirrel;
    entities.addPhysics(squirrel, squirrelPhysics);
    entities.addRender(squirrel, squirrelRender);
    entities.addQuiz(squirrel, squirrelQuiz);

    //rabbit
    int rabbit = entities.add();
    QPointF rabbitPos = sampleVertex(vertices);
    Entities::PhysicsBag *rabbitPhysics = new Entities::PhysicsBag;
    Entities::RenderBag *rabbitRender = new Entities::RenderBag;
    Entities::QuizBag *rabbitQuiz = new Entities::QuizBag;
    rabbitPhysics->w = 128.f;
    rabbitPhysics->h = 123.f;
    rabbitPhysics->x = rabbitPos.x();
    rabbitPhysics->y = rabbitPos.y();
    rabbitRender->imageName = "rabbit";
    rabbitRender->scaleX = 0.5f;
    rabbitRender->scaleY = 0.5f;
    rabbitQuiz->answerID = Quiz::Answer::Rabbit;
    entities.addPhysics(rabbit, rabbitPhysics);
    entities.addRender(rabbit, rabbitRender);
    entities.addQuiz(rabbit, rabbitQuiz);

    //bear
    int bear = entities.add();
    QPointF bearPos = sampleVertex(vertices);
    Entities::PhysicsBag *bearPhysics = new Entities::PhysicsBag;
    Entities::RenderBag *bearRender = new Entities::RenderBag;
    Entities::QuizBag *bearQuiz = new Entities::QuizBag;
    bearPhysics->w = 384.f;
    bearPhysics->h = 205.f;
    bearPhysics->x = bearPos.x();
    bearPhysics->y = bearPos.y();
    bearRender->imageName = "bear";
    bearRender->scaleX = 0.75f;
    bearRender->scaleY = 0.75f;
    bearQuiz->answerID = Quiz::Answer::Bear;
    entities.addPhysics(bear, bearPhysics);
    entities.addRender(bear, bearRender);
    entities.addQuiz(bear, bearQuiz);

    //badger
    int badger = entities.add();
    QPointF badgerPos = sampleVertex(vertices);
    Entities::PhysicsBag *badgerPhysics = new Entities::PhysicsBag;
    Entities::RenderBag *badgerRender = new Entities::RenderBag;
    Entities::QuizBag *badgerQuiz = new Entities::QuizBag;
    badgerPhysics->w = 128.f;
    badgerPhysics->h = 59.f;
    badgerPhysics->x = badgerPos.x();
    badgerPhysics->y = badgerPos.y();
    badgerRender->imageName = "badger";
    badgerRender->scaleX = 0.5f;
    badgerRender->scaleY = 0.5f;
    badgerQuiz->answerID = Quiz::Answer::Badger;
    entities.addPhysics(badger, badgerPhysics);
    entities.addRender(badger, badgerRender);
    entities.addQuiz(badger, badgerQuiz);

    //moose
    int moose = entities.add();
    QPointF moosePos = sampleVertex(vertices);
    Entities::PhysicsBag * moosePhysics = new Entities::PhysicsBag;
    Entities::RenderBag * mooseRender = new Entities::RenderBag;
    Entities::QuizBag * mooseQuiz = new Entities::QuizBag;
    moosePhysics->w = 384.f;
    moosePhysics->h = 324.f;
    moosePhysics->x = moosePos.x();
    moosePhysics->y = moosePos.y();
    mooseRender->imageName = "moose";
    mooseRender->scaleX = 0.75f;
    mooseRender->scaleY = 0.75f;
    mooseQuiz->answerID = Quiz::Answer::Moose;
    entities.addPhysics(moose, moosePhysics);
    entities.addRender(moose, mooseRender);
    entities.addQuiz(moose, mooseQuiz);

    //fox
    int fox = entities.add();
    QPointF foxPos = sampleVertex(vertices);
    Entities::PhysicsBag * foxPhysics = new Entities::PhysicsBag;
    Entities::RenderBag * foxRender = new Entities::RenderBag;
    Entities::QuizBag * foxQuiz = new Entities::QuizBag;
    foxPhysics->w = 256.f;
    foxPhysics->h = 230.f;
    foxPhysics->x = foxPos.x();
    foxPhysics->y = foxPos.y();
    foxRender->imageName = "fox";
    foxQuiz->answerID = Quiz::Answer::Fox;
    entities.addPhysics(fox, foxPhysics);
    entities.addRender(fox, foxRender);
    entities.addQuiz(fox, foxQuiz);

    //wolf
    int wolf = entities.add();
    QPointF wolfPos = sampleVertex(vertices);
    Entities::PhysicsBag * wolfPhysics = new Entities::PhysicsBag;
    Entities::RenderBag * wolfRender = new Entities::RenderBag;
    Entities::QuizBag * wolfQuiz = new Entities::QuizBag;
    wolfPhysics->w = 256.f;
    wolfPhysics->h = 251.f;
    wolfPhysics->x = wolfPos.x();
    wolfPhysics->y = wolfPos.y();
    wolfRender->imageName = "wolf";
    wolfQuiz->answerID = Quiz::Answer::Wolf;
    entities.addPhysics(wolf, wolfPhysics);
    entities.addRender(wolf, wolfRender);
    entities.addQuiz(wolf, wolfQuiz);
}
//starts a new game at a set difficulty
void View::startGame(Quiz::Difficulty difficulty)
{
    // Entity initialization
    // TODO: Move all these hard-coded values to a spawner system, or at least some sort of centralized, well-defined location
    for (int i = -10; i < 60; i++)
    {
        int cloud = entities.add();
        Entities::PhysicsBag *cloudPhysics = new Entities::PhysicsBag;
        Entities::RenderBag *cloudRender = new Entities::RenderBag;
        cloudPhysics->x = i * 1024.f;
        cloudPhysics->y = -400.f;
        cloudPhysics->bodyType = b2BodyType::b2_staticBody;
        cloudRender->imageName = QString("cloud%1").arg(QString::number(random(1, 2)));
        entities.addPhysics(cloud, cloudPhysics);
        entities.addRender(cloud, cloudRender);
    }
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
        edgePhysics->categoryBits = Entities::Category::Ground;
        edgePhysics->maskBits = -1; // Everything collides with ground by default, so ground has to also collide with everything (-1 mask bits)
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
            treePhysics->bodyType = b2BodyType::b2_staticBody;
            treeRender->imageName = "tree";
            treeRender->scaleX = std::cos(random(0, 1) * M_PI);
            entities.addPhysics(tree, treePhysics);
            entities.addRender(tree, treeRender);
        }
        bushPhysics->x = bushVertex.x();
        bushPhysics->y = bushVertex.y() - 45.f;
        bushPhysics->angle = std::atan2(bushVertex.y() - previousBushVertex.y(), bushVertex.x() - previousBushVertex.x());
        bushPhysics->bodyType = b2BodyType::b2_staticBody;
        bushRender->imageName = "bush";
        bushRender->scaleX = std::cos(random(0, 1) * M_PI);
        entities.addPhysics(bush, bushPhysics);
        entities.addRender(bush, bushRender);
    }
    Terrain::Vertices prunedVertices;
    for (int i = 0; i < vertices.size(); i++)
    {
        if (i % 4 == 0) prunedVertices.append(vertices.at(i) - QPointF(0, 500));
    }
    prunedVertices.remove(0);
    prunedVertices.remove(prunedVertices.size() - 1);
    createAnimalEntities(prunedVertices);
    // Quiz initialization
    quiz.startQuiz(difficulty);
    // Switch to game state
    ui->applicationStack->setCurrentWidget(ui->game);
    // Show help box at the beginning of the first quiz
    if (!helpBoxShown)
    {
        helpBox.exec();
        helpBoxShown = true;
    }
}

void View::bestiaryButtonPressed()
{
    ui->applicationStack->setCurrentWidget(ui->bestiary); // Open bestiary
}

void View::backButtonPressed()
{
    entities.removeAll();
    ui->applicationStack->setCurrentWidget(ui->mainMenu);
}

void View::helpButtonPressed(){
    //set the text to the intructions text instead of the welcome text
    helpBox.setText(helpBoxButtonText);
    helpBox.exec();
}
void View::iterateDifficulty(){
    if(currDiff == Quiz::Difficulty::Easy){
        currDiff = Quiz::Difficulty::Medium;
        ui->difficultyLabel->setText("Level: Medium");
    }
    else if(currDiff == Quiz::Difficulty::Medium){
        currDiff = Quiz::Difficulty::Hard;
        ui->difficultyLabel->setText("Level: Hard");
    }
}
void View::endOfQuizPopUp(QString result){
    QMessageBox EndOfQuizBox;
    // Do not delete method, this will create a duplicate window everytime a quiz is created.
    EndOfQuizBox.setWindowTitle("Quiz Has Finished");
    EndOfQuizBox.setText("You have answered all the questions for this level of the quiz!");
    EndOfQuizBox.setInformativeText("You can study the beastiary, go back to the main menu, or continue to the next level. \n" + result);
    QPushButton* retryButton;
    QPushButton* nextLevelButton;
    // if the last level then give option to retry quiz, if not, give option to go to next level.
    if(currDiff == Quiz::Difficulty::Hard){
       retryButton = EndOfQuizBox.addButton(tr("Retry"), QMessageBox::ActionRole);
    }
    else {
       nextLevelButton = EndOfQuizBox.addButton(tr("Next Level"), QMessageBox::ActionRole);
    }
    QPushButton* backToMainMenuButton = EndOfQuizBox.addButton(tr("Main Menu"), QMessageBox::NoRole);
    QPushButton* studyBeastiaryButton = EndOfQuizBox.addButton(tr("Beastiary"), QMessageBox::NoRole);
    QPushButton* resultsPageButton = EndOfQuizBox.addButton(tr("Results"),QMessageBox::NoRole);

    EndOfQuizBox.exec();
    if (EndOfQuizBox.clickedButton() == nextLevelButton) {
        entities.removeAll();
        // next level
        iterateDifficulty();
        startGame(currDiff);
    } else if (EndOfQuizBox.clickedButton() == backToMainMenuButton) {
        // back to menu
        backButtonPressed();
    }
    else if (EndOfQuizBox.clickedButton() == studyBeastiaryButton) {
        // go to beastiary.
        entities.removeAll();
        ui->applicationStack->setCurrentWidget(ui->bestiary);
    }
    else if (EndOfQuizBox.clickedButton() == resultsPageButton) {
        // go to beastiary.
        entities.removeAll();
        ui->applicationStack->setCurrentWidget(ui->resultsPage);
    }
    else if (EndOfQuizBox.clickedButton() == retryButton) {
        entities.removeAll();
        //start over
        currDiff = Quiz::Difficulty::Easy;
        startGame(currDiff);
        ui->difficultyLabel->setText("Level: Easy");
    }


}
void View::endOfQuizResults(QVector<QString> results){
    ui->ResultsLabel->setText(" ");
    for(QString result : results)
        ui->ResultsLabel->setText(ui->ResultsLabel->text()+"\n"+result);
}

View::~View()
{
    delete ui;
}

//When the results page push button is clicked.
void View::on_pushButton_clicked()
{
    entities.removeAll();
    ui->applicationStack->setCurrentWidget(ui->mainMenu);
}

