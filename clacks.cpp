#include "clacks.h"
#include "./ui_clacks.h"

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>

Clacks::Clacks(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Clacks)
{
    ui->setupUi(this);
    databaseConnect();
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

Clacks::~Clacks()
{
    {
        QSqlDatabase db = QSqlDatabase::database();
        db.close();
    }
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );

    delete ui;
    delete feedDialog;
}


void Clacks::on_actionAddNewFeed_triggered()
{
    feedDialog = new AddFeedDialog(this);
    feedDialog->show();
}


void Clacks::on_pushButton_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT url FROM feeds ORDER BY ROWID DESC LIMIT 1");

    if(!query.exec()) {
        qWarning() << "MainWindow::on_pushButton_clicked - ERROR: " << query.lastError().text();
    }
    if(query.first()){
        ui->databaseResult->setText(query.value(0).toString());
    }
    query.finish();

}

