#ifndef CLACKS_H
#define CLACKS_H

#include <QMainWindow>
#include <QStringListModel>
#include <QString>
#include <QStandardItemModel>
#include <QDesktopServices>
#include <QWebEngineView>
#include "addfeeddialog.h"
#include "removedialog.h"
#include "editdialog.h"
#include "feedloader.h"
#include "feed.h"
#include "feedstore.h"

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
    FeedStore feedStore;
    QStandardItemModel *entryModel = 0;
    FeedLoader *feedLoader;
    void databaseConnect();
    void loadFeedList();
    void loadFeedIntoBrowser();
    void displayEntries(Feed feed);

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
    void on_openEntryInBrowser_clicked();
};
#endif // CLACKS_H
