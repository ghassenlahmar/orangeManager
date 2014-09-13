#ifndef ADDSITEDIALOG_H
#define ADDSITEDIALOG_H



#include <QtSql>
#include <QMessageBox>
#include <QHostAddress>
#include <QDialog>
#include <QDebug>
#include "ipcalculator.h"
#include "databaseoperations.h"
#include <QTime>


namespace Ui {
class addSiteDialog;
}

class addSiteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addSiteDialog(QWidget *parent = 0);
    ~addSiteDialog();
    void setVpnId(unsigned int inputVpnId);
    void setVpnName(QString inputVpnName);
    void setVpnAddressRange(QString inputAddressRange);

signals:
    void addedSite();

private slots:
    void on_pushButton_2_clicked();
    void on_lineEdit_2_editingFinished();
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_editingFinished();
    void on_lineEdit_3_textChanged(const QString &arg1);


    void on_pushButton_clicked();


private:
    Ui::addSiteDialog *ui;
    void doSubnetUpdate();
    unsigned int vpnId;

    QString networkIp;
    QString broadcastIp;
    QString vpnAddressRange;
    QString vpnName;
    unsigned int rangeStart;
    unsigned int rangeEnd;    
    QTime time;

};

#endif // ADDSITEDIALOG_H
