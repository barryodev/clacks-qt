#ifndef FEED_H
#define FEED_H
#include <QList>
#include <QUrl>
#include <QStringList>
#include "entry.h"

class Feed
{
public:
    Feed();
    void setEntries(QList<Entry*>);
    QList<Entry*> getEntries();
    QStringList getEntryTitles();
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
