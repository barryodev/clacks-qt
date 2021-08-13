#include "feedloader.h"

FeedLoader::FeedLoader(QObject *parent) :
    QObject(parent)
{

}

void FeedLoader::downloadFeed(QUrl feedAddress)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(feedAddress));

}

void FeedLoader::replyFinished (QNetworkReply *reply)
{
    QDomDocument doc("mydocument");
    QString result;


    if(reply->error())
    {
        result = QString("Failed to download feed: ") + reply->errorString();
    }
    else
    {
        QLatin1Char space(' ');
        QLatin1String newline("<BR>");

        if (!doc.setContent(reply->readAll())) {
            result = QString("Failed To Parse Xml");
        } else {

            QDomElement topElem = doc.documentElement();
            result += topElem.tagName() + space + topElem.text() + newline;

            QDomNode topTag = topElem.firstChild();
            if(!topTag.isNull() && topTag.hasChildNodes()) {
                QDomNodeList children = topTag.childNodes();

                for(int i = 0; i < children.length() - 1; i++) {
                    QDomElement childElement = children.at(i).toElement();
                    if (!childElement.isNull()) {
                        result += childElement.tagName() + space + childElement.text() + newline;
                    }
                }
            }
        }
    }

    emit sendFeedSignal(result);

    reply->deleteLater();
}
