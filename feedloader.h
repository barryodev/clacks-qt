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

class FeedLoader: public QObject
{
    Q_OBJECT
public:
    FeedLoader(QObject *parent = 0);
    void downloadFeed(QUrl feedAddress);

signals:
    void sendFeedSignal(QString);

public slots:
    void replyFinished (QNetworkReply *reply);

private:
   QNetworkAccessManager *manager;
};

#endif // FEEDLOADER_H
