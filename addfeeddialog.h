#ifndef ADDFEEDDIALOG_H
#define ADDFEEDDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class AddFeedDialog;
}

class AddFeedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFeedDialog(QWidget *parent = nullptr);
    ~AddFeedDialog();

private slots:
    void on_buttonAddFeed_accepted();

    void on_buttonAddFeed_rejected();

signals:
    void sendAddSignal(QString);

private:
    Ui::AddFeedDialog *ui;
};

#endif // ADDFEEDDIALOG_H
