#ifndef QUIZ_H
#define QUIZ_H

#include "entities.h"

#include <QObject>
#include <QVector>

class Quiz : public QObject
{
    Q_OBJECT
public:
    explicit Quiz(QObject *parent = nullptr);
    enum Answer
    {
        Deer,
        Turtle,
        Rabbit,
        Squirrel

    };
    enum Difficulty
    {
        Easy,
        Medium,
        Hard
    };
    void startQuiz(Difficulty);

public slots:
    void update(Entities::PhysicsBags, Entities::QuizBags);
    void queueAnswer(Entities::PhysicsBag*);

signals:
    void questionChanged(QString);
    void quizFinished(QString);

private:
    typedef QPair<QString, Answer> Question;
    const int questionCount = 10;
    QVector<Question> easyQuizPool =
    {
        Question("This animal has 310 degrees of vision, almost twice that of a human.", Deer),
        Question("This animal Has a hard shell developed mainly from their ribs.", Turtle),
        Question("This animal is commonly found in trees", Squirrel),
        Question("This animal's feet are considered a sign of good luck", Rabbit)
    };
    QMap<Difficulty, QVector<Question>> quizPoolMap =
    {
        {Easy, easyQuizPool}
    };
    Entities::PhysicsBag *answer = nullptr;
    QVector<Question> questions;
    QVector<Answer> answers;
    bool quizInProgress();
    QString quizQuestionLabel();
};

#endif // QUIZ_H
