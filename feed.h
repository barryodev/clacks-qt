#ifndef FEED_H
#define FEED_H
#include <QList>
#include <QUrl>
#include "Entry.h"

class Feed
{
public:
    Feed();
    void addEntry(Entry* newEntry);
    QList<Entry*> getEntries();
    QString getTitle();
    void setTitle(QString title);
    QUrl getSource();
    void setSource(QUrl source);


private:
    QList<Entry*> entries;
    QString title;
    QUrl source;
};

#endif // FEED_H
