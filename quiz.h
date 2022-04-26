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
        Question("This animal hops around instead of walking", Rabbit)
    };
    QVector<Question> mediumQuizPool =
    {
        Question("This animal can jump up to 10 feet high.", Deer),
        Question("This animal is Cold blooded", Turtle),
        Question("This animal's name comes from the greek word skiouros, which means shadow-tail", Squirrel),
        Question("This animal's feet are considered a sign of good luck", Rabbit)
    };
    QVector<Question> hardQuizPool =
    {
        Question("This animal is believed to be colorblind and see objects at a lower resolution than humans do.", Deer),
        Question("The oldest known individual of this animal reached 187", Turtle),
        Question("This animal was introduced by humans to Australia", Squirrel),
        Question("This animal's was first domesticated by ancient Rome", Rabbit)
    };
    QMap<Difficulty, QVector<Question>> quizPoolMap =
    {
        {Easy, easyQuizPool},
        {Medium, mediumQuizPool},
        {Hard, hardQuizPool}
    };
    Entities::PhysicsBag *answer = nullptr;
    QVector<Question> questions;
    QVector<Answer> answers;
    bool quizInProgress();
    QString quizQuestionLabel();
};

#endif // QUIZ_H
