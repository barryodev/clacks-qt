#include "clacks.h"
#include "./ui_clacks.h"

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>
#include <QAbstractItemView>

Clacks::Clacks(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Clacks)
{
    ui->setupUi(this);
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
    delete feedDialog;
    delete feedsModel;
}

void Clacks::on_actionAddNewFeed_triggered()
{
    feedDialog = new AddFeedDialog(this);
    connect(feedDialog, SIGNAL(sendSignal(QString)), this, SLOT(receiveSlot(QString)));
    feedDialog->setAttribute(Qt::WA_DeleteOnClose);
    feedDialog->show();
}

void Clacks::receiveSlot(QString feedURL)
{
    if(feedsModel) {
        if(feedsModel->insertRow(feedsModel->rowCount())) {
            QModelIndex index = feedsModel->index(feedsModel->rowCount() - 1, 0);
            feedsModel->setData(index, feedURL);
        }
    }
}
