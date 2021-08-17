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
    QDomDocument doc("feedXml");
    bool error = false;
    QString errorMessage;
    Feed newFeed;


    if(reply->error())
    {
        error = true;
        errorMessage = QString("Failed to download feed: ") + reply->errorString();
    }
    else
    {

        if (!doc.setContent(reply->readAll())) {
            error = true;
            errorMessage = QString("Failed To Parse Xml");
        } else {
            errorMessage = "";
            try {
                QString title = parseTitle(doc);
                newFeed.setTitle(title);
            }  catch (std::runtime_error) {
                error = true;
                errorMessage = QString("Failed To Parse Xml");
            }
        }
    }

    emit sendFeedSignal(error, errorMessage, newFeed);

    reply->deleteLater();
}

QDomElement FeedLoader::findChildElementByTag(QString tagName, QDomElement rootElement)
{
    QDomElement result;

    for(QDomNode childNode = rootElement.firstChild(); !childNode.isNull(); childNode = childNode.nextSibling())
    {
        if (childNode.isElement()) {
            QDomElement childAsElement = childNode.toElement();
            if(QString::compare(childAsElement.tagName(), tagName, Qt::CaseInsensitive) == 0) {
                result = childAsElement;
                break;
            }
        }
    }
    return result;
}

QString FeedLoader::parseTitle(QDomDocument xmlFeed)
{
    QString title = "";
    QDomElement rootElement = xmlFeed.documentElement();
    switch (getFeedType(xmlFeed)) {
    case atom:
    {
        QDomElement titleNode = findChildElementByTag("title", rootElement);
        title = titleNode.text();
        break;
    }
    case rss:
    {
        QDomElement channelNode = findChildElementByTag("channel", rootElement);
        QDomElement titleNode = findChildElementByTag("title", channelNode);
        title = titleNode.text();
        if(QString::compare(title, "") == 0){
            QDomElement titleNode = findChildElementByTag("dc:title", channelNode);
            title = titleNode.text();
        }
        break;
    }
    case rss_rdf:
        QDomElement channelNode = findChildElementByTag("channel", rootElement);
        QDomElement titleNode = findChildElementByTag("title", channelNode);
        title = titleNode.text();
        if(QString::compare(title, "") == 0){
            QDomElement titleNode = findChildElementByTag("dc:title", channelNode);
            title = titleNode.text();
        }
        break;
    }
    if(QString::compare(title, "") == 0){
        throw std::runtime_error("Failed to Parse Xml");
    }

    return title;

}

FeedType FeedLoader::getFeedType(QDomDocument xmlFeed) {
    QDomElement rootElement = xmlFeed.documentElement();
    if(rootElement.isNull()) {
        throw std::runtime_error("Failed to Parse Xml");
    }

    if(rootElement.tagName() == "feed") {
        //atom
        return atom;
    } else if(rootElement.tagName() == "rss") {
        //rss 0.91+
        return rss;
    } else if(rootElement.tagName() == "rdf:RDF") {
        //rss 0.90
        return rss_rdf;
    } else {
        //top element not recognised
        throw std::runtime_error("Failed to Parse Xml");
    }
}
