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
    explicit RemoveDialog(int indexModel = -1, const QString &feedURL = nullptr, QWidget *parent = nullptr);
    ~RemoveDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void sendRemoveSignal(int);

private:
    Ui::RemoveDialog *ui;
    int indexModel;
    QString feedURL;
};

#endif // REMOVEDIALOG_H
