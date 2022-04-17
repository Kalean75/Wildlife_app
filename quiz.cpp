#include "quiz.h"

#include <QRandomGenerator>

Quiz::Quiz(QObject *parent) : QObject(parent)
{
}

void Quiz::startQuiz(Difficulty difficulty)
{
    QVector<Question> pool = quizPoolMap.value(difficulty);
    for (int i = 0; i < qMin(pool.size(), questionCount); i++)
    {
        questions.append(pool.takeAt(QRandomGenerator::global()->generate() % pool.size()));
    }
    emit questionChanged(quizQuestionLabel());
}

void Quiz::update(Entities::PhysicsBags physicsBags, Entities::QuizBags quizBags)
{
    if (!answer) return;
    for (Entities::PhysicsBags::iterator i = physicsBags.begin(); i != physicsBags.end(); i++)
    {
        if (i.value() == answer)
        {
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
                emit quizFinished("Quiz grade: " + QString::number(correctAnswers / questions.size() * 100) + "%");
                questions.clear();
                answers.clear();
            }
            break;
        }
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
    QString label = QString("Question (%1/%2): %3").arg(QString::number(answers.size() + 1), QString::number(questions.size()), questions.at(answers.size()).first);
    return label;
}
