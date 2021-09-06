#ifndef CLACKS_H
#define CLACKS_H

#include <QMainWindow>
#include <QStringListModel>
#include <QMap>
#include <QString>
#include <QStandardItemModel>
#include "addfeeddialog.h"
#include "removedialog.h"
#include "editdialog.h"
#include "feedloader.h"
#include "feed.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Clacks; }
QT_END_NAMESPACE

class Clacks : public QMainWindow
{
    Q_OBJECT

public:
    Clacks(QWidget *parent = nullptr);
    ~Clacks();

private:
    Ui::Clacks *ui;
    AddFeedDialog *addFeedDialog = 0;
    RemoveDialog *removeDialog = 0;
    EditDialog *editDialog = 0;
    QStringListModel *feedsModel;
    QStandardItemModel *entryModel = 0;
    FeedLoader *feedLoader;
    QMap<QString, Feed> allFeeds;
    void databaseConnect();
    void loadFeedList();
    void loadFeedIntoBrowser();

public slots:
    void receiveAddSlot(QString);
    void receiveRemoveSlot(int);
    void receiveEditSlot(int, QString);
    void receiveFeedLoaded(bool, QString, Feed);
    void entryClicked(const QModelIndex &index);

private slots:
    void on_feedsList_clicked(const QModelIndex &index);
    void on_actionAddNewFeed_triggered();
    void on_actionRemove_Feed_triggered();
    void on_actionEdit_Feed_triggered();
};
#endif // CLACKS_H
