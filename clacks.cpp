#include "clacks.h"
#include "./ui_clacks.h"

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>
#include <QAbstractItemView>
#include <QMessageBox>

Clacks::Clacks(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Clacks)
{
    ui->setupUi(this);
    feedLoader = new FeedLoader(this);
    ui->entryWebView->setHtml("");
    ui->entryWebView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->entryWebView->resize(500,500);
    ui->entryWebView->show();


    databaseConnect();
    loadFeedList();
}

void Clacks::databaseConnect() {
    const QString DRIVER("QSQLITE");
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir(dataPath);
    if (!dir.exists())
        dir.mkpath(dataPath);

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);

        db.setDatabaseName(dataPath + "/feeds.db");

        if(!db.open()) {
            qWarning() << "Clacks::databaseConnect - ERROR: " << db.lastError().text();
        }
    }
    else {
        qWarning() << "Clacks::databaseConnect - ERROR: no driver " << DRIVER << " available";
    }

    QSqlQuery query("CREATE TABLE IF NOT EXISTS feeds (id INTEGER PRIMARY KEY, url TEXT)");
    if(!query.isActive())
        qWarning() << "Clacks::databaseConnect - ERROR: " << query.lastError().text();

    query.finish();
}

void Clacks::loadFeedList() {
    QStringList list;

    QSqlQuery query("SELECT url FROM feeds");
    while (query.next()) {
        list.append(query.value(0).toString());
    }

    for(int i = 0; i < list.count(); i++) {
        feedStore.addFeed(i, list.at(i));
    }

    for(int i = 0; i < list.count(); i++) {
        feedLoader->downloadFeed(QUrl(list.at(i)));
        connect(feedLoader, SIGNAL(sendFeedSignal(bool,QString,Feed)), this, SLOT(receiveFeedLoaded(bool,QString,Feed)));
    }

    ui->feedsList->setModel(feedStore.getModel());
    ui->feedsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}



Clacks::~Clacks()
{
    {
        QSqlDatabase db = QSqlDatabase::database();
        db.close();
    }
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );

    delete ui;
    delete entryModel;
    feedLoader->deleteLater();
}

void Clacks::on_actionAddNewFeed_triggered()
{
    addFeedDialog = new AddFeedDialog(this);
    connect(addFeedDialog, SIGNAL(sendAddSignal(QString)), this, SLOT(receiveAddSlot(QString)));
    addFeedDialog->setAttribute(Qt::WA_DeleteOnClose);
    addFeedDialog->show();
}

void Clacks::receiveAddSlot(QString feedURL)
{
    feedStore.addFeed(feedURL);

    feedLoader->downloadFeed(QUrl(feedURL));
    connect(feedLoader, SIGNAL(sendFeedSignal(bool,QString,Feed)), this, SLOT(receiveFeedLoaded(bool,QString,Feed)));
}

void Clacks::on_actionRemove_Feed_triggered()
{
    if(ui->feedsList->selectionModel()->selectedIndexes().size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("Click on a feed in the list that you want to remove");
        msgBox.exec();
    } else {

        removeDialog = new RemoveDialog(ui->feedsList->currentIndex().row(), feedStore.getSource(ui->feedsList->currentIndex()), this);
        connect(removeDialog, SIGNAL(sendRemoveSignal(int)), this, SLOT(receiveRemoveSlot(int)));
        removeDialog->setAttribute(Qt::WA_DeleteOnClose);
        removeDialog->show();
    }
}


void Clacks::receiveRemoveSlot(int removedIndex)
{
    feedStore.removeFeed(removedIndex);
}


void Clacks::on_actionEdit_Feed_triggered()
{
    if(ui->feedsList->selectionModel()->selectedIndexes().size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("Click on a feed in the list that you want to edit");
        msgBox.exec();
    } else {
        editDialog = new EditDialog(ui->feedsList->currentIndex().row(), feedStore.getSource(ui->feedsList->currentIndex()), this);
        connect(editDialog, SIGNAL(sendEditSignal(int,QString)), this, SLOT(receiveEditSlot(int,QString)));
        editDialog->setAttribute(Qt::WA_DeleteOnClose);
        editDialog->show();
    }
}

void Clacks::receiveEditSlot(int index, QString feedURL)
{
    feedStore.addFeed(index, feedURL);

    feedLoader->downloadFeed(QUrl(feedURL));
    connect(feedLoader, SIGNAL(sendFeedSignal(bool,QString,Feed)), this, SLOT(receiveFeedLoaded(bool,QString,Feed)));
}


void Clacks::displayEntries(Feed feed)
{
    if(entryModel != nullptr) {
        delete entryModel;
    }

    entryModel = new QStandardItemModel();

    QList<Entry*> entries = feed.getEntries();

    for(int i = 0; i < entries.count(); i++) {
        QVariant variantWrapper = QVariant::fromValue(*entries.at(i));

        QStandardItem *item = new QStandardItem(entries.at(i)->getTitle());
        item->setData(variantWrapper, Qt::UserRole);

        entryModel->setItem(i, item);
    }

    ui->entryList->setModel(entryModel);
    connect(ui->entryList, &QListView::clicked, this, &Clacks::entryClicked);

    if(entries.count() > 1) {
        ui->entryWebView->setHtml(entries.at(0)->getContent());
        ui->entryList->setCurrentIndex(entryModel->index(0,0));
    }
}

void Clacks::on_feedsList_clicked(const QModelIndex &index)
{
    ui->entryWebView->setHtml("");
    Feed feed = feedStore.getFeed(index);
    displayEntries(feed);
}

void Clacks::receiveFeedLoaded(bool error, QString errorString, Feed feed) {
    if(error) {
        feed.setTitle("Error: " + feed.getSource().toString());
        Entry *entry = new Entry(errorString, "", QUrl(""));
        QList<Entry*> list;
        list.append(entry);
        feed.setEntries(list);
    }
    int indexReplaced = feedStore.updateFeed(feed);
    if(indexReplaced == 0) {
        displayEntries(feed);
    }
}


void Clacks::entryClicked(const QModelIndex &index)
{
    QStandardItem *item = entryModel->itemFromIndex(index);

    if(item != nullptr) {
        QVariant variantWrapper = item->data(Qt::UserRole);
        Entry clickedEntry = variantWrapper.value<Entry>();
        ui->entryWebView->setHtml(clickedEntry.getContent());
    }
}


void Clacks::on_openEntryInBrowser_clicked()
{
    if(entryModel != nullptr) {
        QModelIndexList localSelectedIndexes = ui->entryList->selectionModel()->selectedIndexes();
        if(localSelectedIndexes.count() == 1) {
            QStandardItem *item = entryModel->itemFromIndex(localSelectedIndexes.first());
            QVariant variantWrapper = item->data(Qt::UserRole);
            Entry clickedEntry = variantWrapper.value<Entry>();
            QDesktopServices::openUrl(clickedEntry.getSource());
        }
    }
}


void Clacks::on_actionRefresh_Feeds_triggered()
{
    feedStore.resetFeedStore();
    if(entryModel != nullptr) {
        entryModel->clear();
    }
    ui->entryWebView->setHtml("");
    loadFeedList();
}

