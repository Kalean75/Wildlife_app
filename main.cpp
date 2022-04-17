#include "view.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    B2_NOT_USED(argc);
    B2_NOT_USED(argv);
    QApplication application(argc, argv);
    View view;
    view.show();
    return application.exec();
}
