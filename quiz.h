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
        Squirrel,
        Bear,
        Badger,
        Moose,
        Fox

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
    void sendResults(QVector<QString>);

private:
    typedef QPair<QString, Answer> Question;
    const int questionCount = 10;
    QVector<Question> easyQuizPool =
    {
        Question("This animal has 310 degrees of vision, almost twice that of a human.", Deer),
        Question("This animal Has a hard shell developed mainly from their ribs.", Turtle),
        Question("This animal is cold blooded", Turtle),
        Question("This animal is commonly found in trees", Squirrel),
        Question("This animal hops around instead of walking", Rabbit),
        Question("This animal is nocturnal and tends to live in burrows", Badger),
         Question("Most commonly found in North America and parts of Mexico", Bear)
    };
    QVector<Question> mediumQuizPool =
    {
        Question("This animal can jump up to 10 feet high.", Deer),
        Question("This animal can have a homeland range which can span 30 miles", Deer),
        Question("This animal is Cold blooded", Turtle),
        Question("This animal is closely related to birds", Turtle),
        Question("This animal's name comes from the greek word skiouros, which means shadow-tail", Squirrel),
        Question("This animal is most closely related to beaver and mice", Squirrel),
        Question("Has longer hind limbs than fore-limbs", Squirrel),
        Question("This animal's feet are considered a sign of good luck", Rabbit),
        Question("Can grow up to 20 to 34 inches long from head to tail", Badger),
         Question("This animal Can grow to up to 6ft, and 600 lbs", Bear)
    };
    QVector<Question> hardQuizPool =
    {
        Question("This animal is believed to be colorblind and see objects at a lower resolution than humans do.", Deer),
        Question("The oldest known individual of this animal reached 187 years old", Turtle),
        Question("This animal belongs to the order testudines", Turtle),
        Question("This animal ranges in size from 10cm and 172g to 2.7 m and 500kg in size", Turtle),
        Question("This animal was introduced by humans to Australia", Squirrel),
        Question("This animal's was first domesticated by ancient Rome", Rabbit),
        Question("This animal has a max speed of 25 mph", Rabbit),
        Question("This animal typically lives for about 10 years", Badger),
         Question("This animal Is a member of the family ursidae", Bear)
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
    //Added by Tanner 4/23/2022 2:36
    Difficulty difficulty;
    QVector<Question> testQuestions;
    QVector<Answer> yourAnswers;
    QVector<QString> percentages;
    QVector<QString> getResults();
    QString quizAnswer(Answer answer);
};

#endif // QUIZ_H
