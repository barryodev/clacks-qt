#include "feed.h"

Feed::Feed()
{
    this->entries = QList<Entry*>(20);
}

void Feed::addEntry(Entry* newEntry)
{
    this->entries.append(newEntry);
}

QList<Entry*> Feed::getEntries()
{
    return entries;
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
