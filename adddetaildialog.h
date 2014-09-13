#ifndef ADDDETAILDIALOG_H
#define ADDDETAILDIALOG_H

#include <QDialog>
#include <QString>
#include <QtSql>
#include <QMessageBox>
#include "databaseoperations.h"


namespace Ui {
class addDetailDialog;
}

class addDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addDetailDialog(QWidget *parent = 0);
    ~addDetailDialog();
    void setInterfaceId(unsigned int inputInterfaceId);

    void setRangeEnd(QString inputRangeEnd);
    void setRangeStart(QString inputRangeStart);
    void setVpnId(unsigned int inputVpnId);
    void setInterfaceName(QString inputInterfaceName);
    void setVpnName(QString inputVpnName);

signals:
    void addedDetail();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    unsigned int vpnId;
    Ui::addDetailDialog *ui;
    unsigned int interfaceId;
    unsigned int rangeStart;
    unsigned int rangeEnd;
    QString interfaceName;
    QString vpnName;
};

#endif // ADDDETAILDIALOG_H
