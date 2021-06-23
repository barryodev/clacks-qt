#ifndef CLACKS_H
#define CLACKS_H

#include <QMainWindow>
#include "addfeeddialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Clacks; }
QT_END_NAMESPACE

class Clacks : public QMainWindow
{
    Q_OBJECT

public:
    Clacks(QWidget *parent = nullptr);
    ~Clacks();

private slots:
    void on_actionAddNewFeed_triggered();

    void on_pushButton_clicked();

private:
    Ui::Clacks *ui;
    AddFeedDialog *feedDialog = 0;

    void databaseConnect();
};
#endif // CLACKS_H
