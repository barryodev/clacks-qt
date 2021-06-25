#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(int modelIndex = -1, const QString &feedURL = nullptr, QWidget *parent = nullptr);
    ~EditDialog();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void sendEditSignal(int, QString);

private:
    Ui::EditDialog *ui;
    QString feedURL;
    int modelIndex;
};

#endif // EDITDIALOG_H
