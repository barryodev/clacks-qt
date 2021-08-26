#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QUrl>

class Entry
{
public:
    Entry(QString title, QString content, QUrl source);
    QString getTitle();
    QString getContent();
    QUrl getSource();

private:
    QString title, content;
    QUrl source;

};

#endif // ENTRY_H
