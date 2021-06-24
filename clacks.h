#ifndef CLACKS_H
#define CLACKS_H

#include <QMainWindow>
#include <QStringListModel>
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

private:
    Ui::Clacks *ui;
    AddFeedDialog *feedDialog = 0;
    QStringListModel *feedsModel;
    void databaseConnect();
    void loadFeedList();

public slots:
    void receiveSlot(QString);

private slots:
    void on_actionAddNewFeed_triggered();

};
#endif // CLACKS_H
