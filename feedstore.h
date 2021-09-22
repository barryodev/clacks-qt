#ifndef FEEDSTORE_H
#define FEEDSTORE_H
#include <QStandardItemModel>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include "feed.h"

class FeedStore
{
public:
    FeedStore();
    ~FeedStore();
    void addFeed(QString source);
    void addFeed(int index, QString source);
    void addFeed(const QModelIndex &index, QString title, Feed feed);
    void removeFeed(int index);
    int updateFeed(Feed feed);
    void resetFeedStore();
    QString getSource(const QModelIndex &index);
    Feed getFeed(const QModelIndex &index);
    QStandardItemModel* getModel();

private:
    QStandardItemModel *feedModel = 0;
    QMutex mutex;
};

#endif // FEEDSTORE_H
