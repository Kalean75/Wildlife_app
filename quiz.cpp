#include "quiz.h"

Quiz::Quiz(QObject *parent) : QObject(parent)
{
}

void Quiz::startQuiz(Difficulty difficulty)
{
    QVector<Question> pool = quizPoolMap.value(difficulty);
    questions.clear();
    answers.clear();
    for (int i = 0; i < qMin(pool.size(), questionCount); i++)
    {
        questions.append(pool.takeAt(random(0, static_cast<int>(pool.size() - 1))));
    }
    emit questionChanged(quizQuestionLabel());
}

void Quiz::update(Entities::PhysicsBags physicsBags, Entities::QuizBags quizBags)
{
    if (!answer) return;
    // Identify the entity owning the physics bag passed from the world query, then get this entity's quiz bag (which contains its answer data)
    Entities::PhysicsBags::iterator i = physicsBags.begin();
    while (i.value() != answer) i++;
    answers.append(static_cast<Answer>(quizBags.value(i.key())->answerID));
    if (quizInProgress())
    {
        emit questionChanged(quizQuestionLabel());
    }
    else
    {
        int correctAnswers = 0;
        for (int i = 0; i < questions.size(); i++)
        {
            if (answers.at(i) == questions.at(i).second) correctAnswers++;
        }
        emit quizFinished(QString("Quiz grade: %1%").arg(QString::number(correctAnswers / questions.size() * 100)));
    }
    answer = nullptr;
}

void Quiz::queueAnswer(Entities::PhysicsBag* physicsBag)
{
    if (quizInProgress()) answer = physicsBag;
}

bool Quiz::quizInProgress()
{
    return answers.size() < questions.size();
}

QString Quiz::quizQuestionLabel()
{
    return QString("Question (%1/%2): %3").arg(QString::number(answers.size() + 1), QString::number(questions.size()), questions.at(answers.size()).first);
}
