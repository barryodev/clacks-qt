#include "clacks.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("ClacksQT");
    Clacks w;
    w.show();
    return a.exec();
}
