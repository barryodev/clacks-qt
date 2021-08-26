#include "entry.h"

Entry::Entry(QString title, QString content, QUrl source)
{
    this->title = title;
    this->content = content;
    this->source = source;
}

QString Entry::getTitle()
{
    return title;
}

QString Entry::getContent()
{
    return content;
}

QUrl Entry::getSource()
{
    return source;
}
