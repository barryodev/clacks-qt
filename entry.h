#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QUrl>

class Entry
{
public:
    Entry(QString title, QString description, QUrl source);
    QString getTitle();
    QString getDescription();
    QUrl getSource();

private:
    QString title, description;
    QUrl source;

};

#endif // ENTRY_H
