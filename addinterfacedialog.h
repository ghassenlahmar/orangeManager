#ifndef ADDINTERFACEDIALOG_H
#define ADDINTERFACEDIALOG_H

#include <QtSql>
#include <QMessageBox>
#include <QHostAddress>
#include <QDialog>
#include <QDebug>
#include "ipcalculator.h"
#include "databaseoperations.h"


namespace Ui {
class addInterfaceDialog;
}

class addInterfaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addInterfaceDialog(QWidget *parent = 0);
    ~addInterfaceDialog();
    unsigned int siteId;
    void setSiteId(unsigned int inputSiteId);
    void setSiteName(QString inputVpnName);
    void setSiteAddressRange(QString inputAddressRange);
    void setSiteVlanStart(int inputVlanStart);
    void setSiteVlanEnd(int inputVlanEnd);
    void setVpnId(unsigned int inputVpnId);

signals:
    void addedInterface();

private slots:
    void on_pushButton_2_clicked();
    void on_lineEdit_2_editingFinished();
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_editingFinished();
    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::addInterfaceDialog *ui;
    void doSubnetUpdate();
    QString siteAddressRange;

    unsigned int vpnId;

    QString networkIp;
    QString broadcastIp;
    unsigned int rangeStart;
    unsigned int rangeEnd;
    int vlanStart;
    int vlanEnd;


};

#endif // ADDINTERFACEDIALOG_H
