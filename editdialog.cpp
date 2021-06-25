#include "editdialog.h"
#include "ui_editdialog.h"

#include <QSqlQuery>
#include <QSqlError>


EditDialog::EditDialog(int modelIndex, const QString &feedURL, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
    ui->lineEdit->setText(feedURL);
    this->feedURL = feedURL;
    this->modelIndex = modelIndex;
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::on_buttonBox_accepted()
{
    QSqlQuery query;


    query.prepare("UPDATE feeds SET url = :newURL WHERE url = :oldURL");
    query.bindValue(":newURL", ui->lineEdit->text());
    query.bindValue(":oldURL", feedURL);

    if(!query.exec())
        qWarning() << "RemoveDialog::on_buttonBox_accepted - ERROR: " << query.lastError().text();

    emit sendEditSignal(modelIndex, ui->lineEdit->text());

    close();
}


void EditDialog::on_buttonBox_rejected()
{
    close();
}

