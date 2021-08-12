#include "feedloader.h"

FeedLoader::FeedLoader(QObject *parent) :
    QObject(parent)
{

}

void FeedLoader::downloadFeed(QUrl feedAddress)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(feedAddress));
}

void FeedLoader::replyFinished (QNetworkReply *reply)
{
    QDomDocument doc("mydocument");

    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }
    else
    {
        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << QSslSocket::sslLibraryBuildVersionString();

        if (!doc.setContent(reply->readAll())) {
            qDebug() << "Failed To Parse Xml";
            return;
        } else {

            QDomElement docElem = doc.documentElement();

            QDomNode topTag = docElem.firstChild();
            if(!topTag.isNull() && topTag.hasChildNodes()) {
                QDomNodeList children = topTag.childNodes();

                for(int i = 0; i < children.length() - 1; i++) {
                    QDomElement childElement = children.at(i).toElement();
                    if (!childElement.isNull()) {
                        qDebug() << childElement.tagName() << " " << childElement.text();
                    }
                }
            }
        }
    }

    reply->deleteLater();
}
