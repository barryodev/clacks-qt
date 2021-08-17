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
#include <QLatin1String>
#include <QLatin1Char>
#include <stdexcept>
#include "feed.h"

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
    FeedType getFeedType(QDomDocument xmlFeed);
    QNetworkAccessManager *manager;
    QDomElement findChildElementByTag(QString tag, QDomElement elementToSearch);
};

#endif // FEEDLOADER_H
