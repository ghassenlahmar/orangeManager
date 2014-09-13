#ifndef NEWVPNGROUPDIALOG_H
#define NEWVPNGROUPDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class newVpnGroupDialog;
}

class newVpnGroupDialog : public QDialog
{
    Q_OBJECT

signals:
    void addedVpnGroup();

public:
    explicit newVpnGroupDialog(QWidget *parent = 0);
    ~newVpnGroupDialog();
    void setUserId(int inputUserId);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::newVpnGroupDialog *ui;
    int userId;
};

#endif // NEWVPNGROUPDIALOG_H
