#ifndef ENTITIES_H
#define ENTITIES_H

#include <box2d/box2d.h>

#include <QObject>
#include <QMap>
#include <QTimer>
#include <QElapsedTimer>
#include <random>

// Utility function for uniform random put here for convenience
template<typename T>
T random(T min, T max)
{
    std::random_device seed;
    std::mt19937 generator(seed());
    std::uniform_int_distribution<T> dist(min, max);
    return dist(generator);
}

class Entities : public QObject
{
    Q_OBJECT
public:
    explicit Entities(QObject *parent = nullptr);
    ~Entities();
    // https://www.iforce2d.net/b2dtut/collision-filtering
    // Note that category bits must be powers of two
    enum Category
    {
        Environment = 0x0001,
        Ground = 0x0002
    };
    struct PhysicsBag
    {
        float x = 0.f;
        float y = 0.f;
        // x1 and y1 used for edge shapes
        float x1 = 0.f;
        float y1 = 0.f;
        // r used for circle shapes
        float r = 1.f;
        float w = 1.f;
        float h = 1.f;
        float angle = 0.f;
        float density = 1.f;
        float friction = 1.f;
        float restitution = 0.f;
        float linearDamping = 0.f;
        float angularDamping = 50.f;
        bool isSensor = false;
        short categoryBits = Category::Environment;
        short maskBits = Category::Ground;
        b2Shape::Type shapeType = b2Shape::e_polygon;
        b2BodyType bodyType = b2_dynamicBody;
    };
    struct RenderBag
    {
        QString imageName = "";
        float scaleX = 1.f;
        float scaleY = 1.f;
    };
    struct QuizBag
    {
        int answerID = 0;
    };
    typedef QMap<int, PhysicsBag*> PhysicsBags;
    typedef QMap<int, RenderBag*> RenderBags;
    typedef QMap<int, QuizBag*> QuizBags;
    static constexpr float updateRate = 1.f / 165.f;
    int add();
    void addPhysics(int, PhysicsBag*);
    void addRender(int, RenderBag*);
    void addQuiz(int, QuizBag*);
    void remove(int);
    void removeAll();

signals:
    void addedPhysics(Entities::PhysicsBag*);
    void removedPhysics(Entities::PhysicsBag*);
    void physicsOutdated();
    void renderOutdated(Entities::PhysicsBags, Entities::RenderBags);
    void quizOutdated(Entities::PhysicsBags, Entities::QuizBags);

private:
    const int updateMsDelta = updateRate * 1000;
    int eid = 0;
    int accumulator = 0;
    QElapsedTimer *frameTimer;
    PhysicsBags physicsBags;
    RenderBags renderBags;
    QuizBags quizBags;
    void update();
};

#endif // ENTITIES_H
