#include "view.h"
#include "physics.h"
#include "renderer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    B2_NOT_USED(argc);
    B2_NOT_USED(argv);
    QApplication application(argc, argv);
    Physics physics;
    Renderer renderer;
    View view(physics, renderer);
    view.show();
    return application.exec();
}
