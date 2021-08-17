#include "entry.h"

Entry::Entry(QString title, QString description, QUrl source)
{
    this->title = title;
    this->description = description;
    this->source = source;
}

QString Entry::getTitle()
{
    return title;
}

QString Entry::getDescription()
{
    return description;
}

QUrl Entry::getSource()
{
    return source;
}
