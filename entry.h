#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QUrl>
#include <QMetaType>

class Entry
{
public:
    Entry() = default;
    ~Entry() = default;
    Entry(const Entry &);
    Entry &operator=(const Entry &) = default;

    Entry(const QString &title, const QString &content, const QUrl &source);
    QString getTitle();
    QString getContent();
    QUrl getSource();

private:
    QString title, content;
    QUrl source;
};

Q_DECLARE_METATYPE(Entry);
#endif // ENTRY_H

