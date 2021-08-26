#ifndef FEEDLOADER_H
#define FEEDLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomNodeList>
#include <stdexcept>
#include "feed.h"
#include "entry.h"

enum FeedType { atom, rss, rss_rdf };

class FeedLoader: public QObject
{
    Q_OBJECT
public:
    FeedLoader(QObject *parent = 0);
    void downloadFeed(QUrl feedAddress);

signals:
    void sendFeedSignal(bool, QString, Feed);

public slots:
    void replyFinished (QNetworkReply *reply);

private:
    QString parseTitle(QDomDocument);
    QList<Entry*> parseEntries(QDomDocument);
    FeedType getFeedType(QDomDocument xmlFeed);
    QNetworkAccessManager *manager;
    QDomElement findChildElementByTag(QString tag, QDomElement elementToSearch);
};

#endif // FEEDLOADER_H
