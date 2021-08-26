#include "feed.h"

Feed::Feed()
{

}

void Feed::setEntries(QList<Entry*> entries)
{
    this->entries = entries;
}

QList<Entry*> Feed::getEntries()
{
    return entries;
}

QStringList Feed::getEntryTitles()
{
    QStringList entryTitles;

    for(int x = 0; x < entries.size() - 1; x++) {
        entryTitles.append(entries.at(x)->getTitle());
    }

    return entryTitles;
}

QString Feed::getTitle()
{
    return title;
}

void Feed::setTitle(QString title)
{
    this->title = title;
}

QUrl Feed::getSource()
{
    return source;
}

void Feed::setSource(QUrl source) {
    this->source = source;
}
