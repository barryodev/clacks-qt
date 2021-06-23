#include "clacks.h"
#include "./ui_clacks.h"

Clacks::Clacks(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Clacks)
{
    ui->setupUi(this);
}

Clacks::~Clacks()
{
    delete ui;
    delete feedDialog;
}


void Clacks::on_actionAddNewFeed_triggered()
{
    feedDialog = new AddFeedDialog(this);
    feedDialog->show();
}

