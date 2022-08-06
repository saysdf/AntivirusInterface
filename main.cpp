#include <QApplication>
#include "handler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Handler h;
    h.startProgramm();
    return a.exec();
}
