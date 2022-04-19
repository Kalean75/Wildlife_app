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
        Deer
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
        Question("This animal has 310 degrees of vision, almost twice that of a human.", Deer)
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
