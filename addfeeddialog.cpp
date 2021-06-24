#include "addfeeddialog.h"
#include "ui_addfeeddialog.h"

#include <QSqlQuery>
#include <QSqlError>

AddFeedDialog::AddFeedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFeedDialog)
{
    ui->setupUi(this);
}

AddFeedDialog::~AddFeedDialog()
{
    delete ui;
}

void AddFeedDialog::on_buttonAddFeed_accepted()
{
    QSqlQuery query;

    query.prepare("INSERT INTO feeds (url) VALUES(:url)");
    query.bindValue(":url", ui->feedURLInput->text());

    if(!query.exec())
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();

    emit sendSignal(ui->feedURLInput->text());

    close();
}


void AddFeedDialog::on_buttonAddFeed_rejected()
{
    close();
}

