#include "feedstore.h"

FeedStore::FeedStore()
{
    feedModel = new QStandardItemModel();
}

FeedStore::~FeedStore()
{
    delete feedModel;
}

void FeedStore::addFeed(QString source)
{
    QMutexLocker locker(&mutex);
    QStandardItem *item = new QStandardItem(source);

    Feed feed;
    feed.setSource(QUrl(source));
    QVariant variantWrapper = QVariant::fromValue(feed);
    item->setData(variantWrapper, Qt::UserRole);

    int rowCount = feedModel->rowCount();
    feedModel->setItem(rowCount, item);
}

void FeedStore::addFeed(int index, QString source)
{
    QMutexLocker locker(&mutex);
    QStandardItem *item = new QStandardItem(source);

    Feed feed;
    feed.setSource(QUrl(source));
    QVariant variantWrapper = QVariant::fromValue(feed);
    item->setData(variantWrapper, Qt::UserRole);

    feedModel->setItem(index, item);
}


void FeedStore::addFeed(const QModelIndex &index, QString title, Feed feed)
{
    QMutexLocker locker(&mutex);
    QStandardItem *item = feedModel->itemFromIndex(index);
    item->setText(title);
    QVariant variantWrapper = QVariant::fromValue(feed);
    item->setData(variantWrapper, Qt::UserRole);

    feedModel->setItem(index.row(), item);
}

void FeedStore::removeFeed(int index)
{
    QMutexLocker locker(&mutex);
    feedModel->removeRows(index, 1);
}

int FeedStore::updateFeed(Feed feed)
{
    QMutexLocker locker(&mutex);
    int indexReplaced = -1;

    for(int i = 0; i < feedModel->rowCount(); i++) {
        QStandardItem *item = feedModel->item(i);
        QVariant variantWrapper = item->data(Qt::UserRole);
        Feed currentFeed = variantWrapper.value<Feed>();
        if(QString::compare(currentFeed.getSource().toString(), feed.getSource().toString()) == 0) {
            QStandardItem *newItem = new QStandardItem(feed.getTitle());
            QVariant variantWrapper = QVariant::fromValue(feed);
            newItem->setData(variantWrapper, Qt::UserRole);
            feedModel->setItem(i, newItem);
            indexReplaced = i;
            break;
        }
    }

    return indexReplaced;
}

void FeedStore::resetFeedStore()
{
     QMutexLocker locker(&mutex);
     feedModel->clear();

}

QString FeedStore::getSource(const QModelIndex &index)
{
    QMutexLocker locker(&mutex);
    QStandardItem *item =feedModel->itemFromIndex(index);
    QVariant variantWrapper = item->data(Qt::UserRole);
    Feed feed = variantWrapper.value<Feed>();
    return feed.getSource().toString();
}

Feed FeedStore::getFeed(const QModelIndex &index)
{
    QMutexLocker locker(&mutex);
    QStandardItem *item = feedModel->itemFromIndex(index);
    QVariant variantWrapper = item->data(Qt::UserRole);
    Feed returnedFeed = variantWrapper.value<Feed>();
    return returnedFeed;

}

QStandardItemModel *FeedStore::getModel()
{
    return feedModel;
}
