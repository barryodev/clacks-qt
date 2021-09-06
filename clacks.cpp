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
    ui->entryText->setReadOnly(true);
    ui->entryText->setOpenExternalLinks(true);
    ui->entryText->setTextInteractionFlags(ui->entryText->textInteractionFlags() | Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard);
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

void Clacks::loadFeedList(){
    feedsModel = new QStringListModel(this);

    QStringList list;

    QSqlQuery query("SELECT url FROM feeds");
    while (query.next()) {
        list.append(query.value(0).toString());
    }

    feedsModel->setStringList(list);
    ui->feedsList->setModel(feedsModel);
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
    delete feedsModel;
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
    if(feedsModel) {
        if(feedsModel->insertRow(feedsModel->rowCount())) {
            QModelIndex index = feedsModel->index(feedsModel->rowCount() - 1, 0);
            feedsModel->setData(index, feedURL);
        }
    }
}

void Clacks::on_actionRemove_Feed_triggered()
{
    if(ui->feedsList->selectionModel()->selectedIndexes().size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("Click on a feed in the list that you want to remove");
        msgBox.exec();
    } else {
        removeDialog = new RemoveDialog(ui->feedsList->currentIndex().row(), feedsModel->data(ui->feedsList->currentIndex()).toString(), this);
        connect(removeDialog, SIGNAL(sendRemoveSignal(int)), this, SLOT(receiveRemoveSlot(int)));
        removeDialog->setAttribute(Qt::WA_DeleteOnClose);
        removeDialog->show();
    }
}


void Clacks::receiveRemoveSlot(int removedIndex)
{
    feedsModel->removeRow(removedIndex);
}


void Clacks::on_actionEdit_Feed_triggered()
{
    if(ui->feedsList->selectionModel()->selectedIndexes().size() == 0) {
        QMessageBox msgBox;
        msgBox.setText("Click on a feed in the list that you want to edit");
        msgBox.exec();
    } else {
        editDialog = new EditDialog(ui->feedsList->currentIndex().row(), feedsModel->data(ui->feedsList->currentIndex()).toString(), this);
        connect(editDialog, SIGNAL(sendEditSignal(int,QString)), this, SLOT(receiveEditSlot(int,QString)));
        editDialog->setAttribute(Qt::WA_DeleteOnClose);
        editDialog->show();
    }
}

void Clacks::receiveEditSlot(int index, QString feedURL)
{
    feedsModel->setData(feedsModel->index(index, 0), feedURL);
}


void Clacks::on_feedsList_clicked(const QModelIndex &index)
{

    feedLoader->downloadFeed(QUrl(feedsModel->data(ui->feedsList->currentIndex()).toString()));

    connect(feedLoader, SIGNAL(sendFeedSignal(bool,QString,Feed)), this, SLOT(receiveFeedLoaded(bool,QString,Feed)));

    ui->entryText->setText(feedsModel->data(ui->feedsList->currentIndex()).toString());

}

void Clacks::receiveFeedLoaded(bool error, QString errorString, Feed feed) {
    if(error) {
        ui->entryText->setHtml(errorString);
    } else {
        ui->entryText->setHtml(feed.getTitle());

        if(entryModel) {
            delete entryModel;
        }

        entryModel = new QStandardItemModel();

        QList entries = feed.getEntries();

        for(int i = 0; i < entries.count(); i++) {
            QVariant variantWrapper = QVariant::fromValue(*entries.at(i));

            QStandardItem *item = new QStandardItem(entries.at(i)->getTitle());
            item->setData(variantWrapper, Qt::UserRole);

            entryModel->setItem(i, item);
        }

        ui->entryList->setModel(entryModel);
        connect(ui->entryList, &QListView::clicked, this, &Clacks::entryClicked);

    }
}

void Clacks::entryClicked(const QModelIndex &index)
{
    QStandardItem *item = entryModel->itemFromIndex(index);

    if(item != nullptr) {
        QVariant variantWrapper = item->data(Qt::UserRole);
        Entry clickedEntry = variantWrapper.value<Entry>();

        ui->entryText->setHtml(clickedEntry.getContent());
    }
}

