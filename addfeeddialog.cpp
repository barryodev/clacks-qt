#include "addfeeddialog.h"
#include "ui_addfeeddialog.h"

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
    accept();
}


void AddFeedDialog::on_buttonAddFeed_rejected()
{
    reject();
}

