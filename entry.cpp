#include "entry.h"

Entry::Entry(const QString &title, const QString &content, const QUrl &source)
{
    this->title = title;
    this->content = content;
    this->source = source;
}

Entry::Entry(const Entry &copyEntry)
{
    this->title = copyEntry.title;
    this->content = copyEntry.content;
    this->source = copyEntry.source;
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
