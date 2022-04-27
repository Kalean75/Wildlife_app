#ifndef VIEW_H
#define VIEW_H

#include "entities.h"
#include "physics.h"
#include "renderer.h"
#include "quiz.h"
#include "terrain.h"

#include <QMainWindow>
#include <QMessageBox>

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
    void startGameButtonHandler();
    void startGame(Quiz::Difficulty difficulty);
    void bestiaryButtonPressed();
    void backButtonPressed();
    void helpButtonPressed();
    void endOfQuizPopUp(QString result);
    void endOfQuizResults(QVector<QString> information);


protected:
    void createAnimals();

    void createAnimalEntities(Terrain::Vertices);

private slots:
    void on_pushButton_clicked();

private:
    const QString helpBoxWelcomeText = QString
    (
        "Welcome!\n\nWildlife Education is a quiz based app designed to help users identify animals in their natural environment. "
        "The quiz is played as follows: A question or a fun fact will appear at the top of the screen. "
        "You will need to find the animal that statement is describing and click on the animal. "
        "The quicker you find and click on the animal, the more points you get!"
    );
    // make the text not say welcome when the help button is pressed.
    const QString helpBoxButtonText = QString
    (
        "Instructions:\n\n"
        "The quiz is played as follows: A question or a fun fact will appear at the top of the screen. "
        "You will need to find the animal that statement is describing and click on the animal."
        "The quicker you find and click on the animal, the more points you get!"
    );
    bool helpBoxShown = false;
    QMessageBox helpBox;
    Entities entities;
    Physics physics;
    Renderer renderer;
    Quiz quiz;
    Terrain terrain;
    Quiz::Difficulty currDiff;
    void iterateDifficulty();
    Ui::View *ui;
};

#endif // VIEW_H
