#include "clacks.h"

#include <QApplication>
#include "feedloader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("ClacksQT");
    Clacks w;
    w.show();

    FeedLoader fl;
    fl.downloadFeed(QUrl("https://www.example.com"));

    return a.exec();
}
