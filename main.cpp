#include "view.h"
#include "entities.h"
#include "physics.h"
#include "renderer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    B2_NOT_USED(argc);
    B2_NOT_USED(argv);
    QApplication application(argc, argv);
    Entities entities;
    Physics physics;
    Renderer renderer;
    View view(entities, physics, renderer);
    view.show();
    return application.exec();
}
