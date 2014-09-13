#ifndef ADDVPNDIALOG_H
#define ADDVPNDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>
#include <QHostAddress>
#include "ipcalculator.h"
#include "databaseoperations.h"


namespace Ui {
class addVpnDialog;
}

class addVpnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addVpnDialog(QWidget *parent = 0);
    ~addVpnDialog();

signals:
    void addedVpn();

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_2_editingFinished();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_lineEdit_3_editingFinished();

    void on_lineEdit_3_textChanged(const QString &arg1);

private:
    Ui::addVpnDialog *ui;
    void doSubnetUpdate();
    QString networkIp;
    QString broadcastIp;
    unsigned int rangeStart;
    unsigned int rangeEnd;

};

#endif // ADDVPNDIALOG_H
