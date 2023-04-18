#include "GooeyDeadDaque.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GooeyDeadDaque w;
    w.show();
    return a.exec();
}
