#ifndef REMOVEDIALOG_H
#define REMOVEDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class RemoveDialog;
}

class RemoveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveDialog(QWidget *parent = nullptr, QStringListModel *feedsModel = nullptr);
    ~RemoveDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void sendRemoveSignal(int);

private:
    Ui::RemoveDialog *ui;
};

#endif // REMOVEDIALOG_H
