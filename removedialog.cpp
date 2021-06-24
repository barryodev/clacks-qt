#include "removedialog.h"
#include "ui_removedialog.h"

#include <QSqlQuery>
#include <QSqlError>

RemoveDialog::RemoveDialog(QWidget *parent, QStringListModel *feedsModel) :
    QDialog(parent),
    ui(new Ui::RemoveDialog)
{
    ui->setupUi(this);
    ui->removeComboBox->setModel(feedsModel);
}

RemoveDialog::~RemoveDialog()
{
    delete ui;
}

void RemoveDialog::on_buttonBox_accepted()
{
    QSqlQuery query;

    query.prepare("DELETE FROM feeds WHERE url = ?");
    query.addBindValue(ui->removeComboBox->currentText());

    if(!query.exec())
        qWarning() << "RemoveDialog::on_buttonBox_accepted - ERROR: " << query.lastError().text();

    emit sendRemoveSignal(ui->removeComboBox->currentIndex());
    close();
}


void RemoveDialog::on_buttonBox_rejected()
{
    close();
}

