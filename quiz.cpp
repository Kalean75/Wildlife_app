#include "quiz.h"
#include "QDebug"

Quiz::Quiz(QObject *parent) : QObject(parent)
{
}

void Quiz::startQuiz(Difficulty difficulty)
{
    if(difficulty==Easy)
        questionCount=5;
    else if(difficulty==Medium)
        questionCount=7;
    else
        questionCount=9;
    this->difficulty = difficulty;
    QVector<Question> pool = quizPoolMap.value(difficulty);
    questions.clear();
    answers.clear();
    for (int i = 0; i < questionCount; i++)
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
            if (answers.at(i) == questions.at(i).second) correctAnswers++;//correct answer
            //Tanner's updated code for storing results.
            testQuestions.append(questions.at(i));
            yourAnswers.append(answers.at(i));

        }
            QString percentage = QString("Quiz grade: %1%").arg(QString::number(static_cast<float>(correctAnswers) / questions.size() * 100));
            emit quizFinished(percentage);
            percentages.append(percentage);
            emit sendResults(getResults());
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

QVector<QString> Quiz::getResults(){
    QVector<QString> vector;
    int currentLevel=5;
    bool easyHit=false;
    bool mediumHit=false;
    int questionNumber=1;
    for(int i=0;i<testQuestions.length();i++){
        QString i_string = QString::number(questionNumber);
        vector.append(QString(i_string+". Question: ")+testQuestions[i].first);
        vector.append(QString("Answer: ")+quizAnswer(testQuestions[i].second));
        vector.append("Your Answer:"+quizAnswer(yourAnswers[i]));
        if((questionNumber)%currentLevel==0){
            vector.append(percentages[(i)/currentLevel]);
        }
        if(questionNumber==5 && !easyHit){//reset when Easy is finished
            questionNumber=0;
            easyHit=true;
            currentLevel=7;
        }
        if(questionNumber==7 && !mediumHit){//reset when Easy is finished
            questionNumber=0;
            mediumHit=true;
            currentLevel=9;
        }
        if(questionNumber==9){//reset when Easy is finished
            questionNumber=0;
            easyHit=false;
            mediumHit=false;
            currentLevel=5;
        }
        questionNumber++;
    }
    return vector;
}

QString Quiz::quizAnswer(Answer answer){
    if(answer==Deer)
        return QString("Deer");
    if(answer==Rabbit)
        return QString("Rabbit");
    if(answer==Turtle)
        return QString("Turtle");
    if(answer==Squirrel)
        return QString("Squirrel");
    if(answer==Bear)
        return QString("Bear");
    if(answer==Badger)
        return QString("Badger");
    if(answer==Moose)
        return QString("Moose");
    if(answer==Fox)
        return QString("Fox");
}
void Quiz::clear(){
    testQuestions.clear();
    yourAnswers.clear();
    percentages.clear();
}

