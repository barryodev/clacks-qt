#ifndef CLACKS_H
#define CLACKS_H

#include <QMainWindow>
#include <QStringListModel>
#include "addfeeddialog.h"
#include "removedialog.h"

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
    RemoveDialog *removeDialog = 0;
    QStringListModel *feedsModel;
    void databaseConnect();
    void loadFeedList();

public slots:
    void receiveAddSlot(QString);
    void receiveRemoveSlot(int);

private slots:
    void on_actionAddNewFeed_triggered();
    void on_actionRemove_Feed_triggered();
};
#endif // CLACKS_H
