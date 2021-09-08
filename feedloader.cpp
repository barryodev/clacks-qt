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
    newFeed.setSource(reply->url());

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
                newFeed.setEntries(parseEntries(doc));
            }  catch (std::runtime_error) {
                error = true;
                errorMessage = QString("Failed To Parse Xml");
            }
        }
    }

    emit sendFeedSignal(error, errorMessage, newFeed);

    reply->deleteLater();
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
    case rss: case rss_rdf:
    {
        QDomElement channelNode = findChildElementByTag("channel", rootElement);
        QDomElement titleNode = findChildElementByTag("title", channelNode);
        title = titleNode.text();
        if(QString::compare(title, "") == 0) {
            QDomElement titleNode = findChildElementByTag("dc:title", channelNode);
            title = titleNode.text();
        }
        break;
    }
    }
    if(QString::compare(title, "") == 0){
        throw std::runtime_error("Failed to Parse Xml");
    }

    return title;

}

QList<Entry*> FeedLoader::parseEntries(QDomDocument xmlFeed)
{
    QList<Entry*> allEntries = QList<Entry*>();
    switch (getFeedType(xmlFeed)) {
    case atom:
    {
        QDomNodeList entryList = xmlFeed.elementsByTagName("entry");
        for(int x = 0; x < entryList.size(); x++) {
            QDomNode currentNode = entryList.item(x);
            if(currentNode.isElement() && !currentNode.isNull()) {

              QString title = findChildElementByTag("title", currentNode.toElement()).text();
              QString content = findChildElementByTag("content", currentNode.toElement()).text();
              QDomElement linkElement = findChildElementByTag("link", currentNode.toElement());
              QString source = linkElement.attribute("href");

              Entry* newEntry = new Entry(title, content, QUrl(source));
              allEntries.append(newEntry);
            }
        }
        break;
    }
    case rss: case rss_rdf:
    {
        QDomNodeList itemList = xmlFeed.elementsByTagName("item");
        for(int x = 0; x < itemList.size(); x++) {
            QDomNode currentNode = itemList.item(x);
            if(currentNode.isElement() && !currentNode.isNull()) {
                QString title = findChildElementByTag("title", currentNode.toElement()).text();
                if(QString::compare(title, "") == 0) {
                    title = findChildElementByTag("dc:title", currentNode.toElement()).text();
                }

                QString content = findChildElementByTag("content:encoded", currentNode.toElement()).text();

                if(QString::compare(content, "") == 0) {
                    content = findChildElementByTag("description", currentNode.toElement()).text();

                    if(QString::compare(content, "") == 0) {
                        content = findChildElementByTag("dc:description", currentNode.toElement()).text();
                    }
                }

                QString source = findChildElementByTag("link", currentNode.toElement()).text();

                Entry* newEntry = new Entry(title, content, QUrl(source));
                allEntries.append(newEntry);
            }
        }
        break;
    }

    }

    return allEntries;
}

FeedType FeedLoader::getFeedType(QDomDocument xmlFeed)
{
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
