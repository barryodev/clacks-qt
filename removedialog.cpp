#include "removedialog.h"
#include "ui_removedialog.h"

#include <QSqlQuery>
#include <QSqlError>

RemoveDialog::RemoveDialog(int indexModel, const QString &feedURL, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoveDialog)
{
    ui->setupUi(this);
    this->indexModel = indexModel;
    this->feedURL = feedURL;
    ui->labelFeedURL->setText(feedURL);
}

RemoveDialog::~RemoveDialog()
{
    delete ui;
}

void RemoveDialog::on_buttonBox_accepted()
{
    QSqlQuery query;

    query.prepare("DELETE FROM feeds WHERE url = ?");
    query.addBindValue(feedURL);

    if(!query.exec())
        qWarning() << "RemoveDialog::on_buttonBox_accepted - ERROR: " << query.lastError().text();

    emit sendRemoveSignal(indexModel);
    close();
}


void RemoveDialog::on_buttonBox_rejected()
{
    close();
}

