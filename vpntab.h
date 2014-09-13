#ifndef VPNTAB_H
#define VPNTAB_H

#include <QWidget>
#include <QtSql>
#include <QString>
#include <QHeaderView>
#include "addsitedialog.h"
#include "siteviewer.h"
#include "interfaceviewer.h"
#include "confirmdeletedialog.h"




namespace Ui {
class vpnTab;
}

class vpnTab : public QWidget
{
    Q_OBJECT

public:
    /*explicit vpnTab(QWidget *parent = 0);*/
    vpnTab(QString vpnName,QString vpnDescription,QString addressRange, QString inputVlanRange, unsigned int inputvpnId);
    ~vpnTab();
    unsigned int getVpnId();
    void setPointer(vpnTab* inputPointer);
    QString getVpnName();
    QString getVpnAddressRange();
    QString getVpnVlanRange();
    QString getSiteAddressRange();
    unsigned int getSiteId();
    QString getSiteName();


public slots:
    void refreshSite();

private slots:
    //void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void detailRequested(unsigned int inputInterfaceId,QString inputInterfaceName, QString inputInterfaceAddressRange);
    void backToInterfaces();
    void on_tableView_clicked(const QModelIndex &index);
    void on_pushButton_2_clicked();

private:
    unsigned int vpnId;
    QString vpnName;
    QString addressRange;
    QString vlanRange;

    Ui::vpnTab *ui;
    QSqlQueryModel *siteModel;

    addSiteDialog addSite;
    siteViewer viewSite;
    interfaceViewer viewInterface;

    unsigned int siteId;
    QString siteName;
    QString siteAddressRange;

    confirmDeleteDialog del;    
};

#endif // VPNTAB_H
