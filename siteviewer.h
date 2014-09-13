#ifndef SITEVIEWER_H
#define SITEVIEWER_H

#include <QWidget>
#include <QtSql>
#include <QString>
#include <QHeaderView>
#include "addinterfacedialog.h"



class vpnTab;

namespace Ui {
class siteViewer;
}

class siteViewer : public QWidget
{
    Q_OBJECT



public:
    explicit siteViewer(QWidget *parent = 0);
    ~siteViewer();
    void settings(unsigned int inputSiteId,QString inputSiteName,QString inputSiteAddressRange, QString inputVlanRange);
//void settings(unsigned int inputSiteId,QString inputSiteName,QString inputSiteAddressRange, QString inputVlanRange,  vpnTab * inputPointer);
    void setVpnId(unsigned int inputVpnId);
    void setVpnName(QString inputVpnName);    

public slots:
    void refreshInterfaces();

signals:
    void interfaceDetail(unsigned int interfaceId,QString interfaceName,QString interfaceAddressRange);

private slots:
    void on_pushButton_4_clicked();
    void on_tableView_2_doubleClicked(const QModelIndex &index);


    void on_pushButton_3_clicked();

    void on_tableView_2_clicked(const QModelIndex &index);

private:
    Ui::siteViewer *ui;
    QSqlQueryModel *interfacesModel;
    unsigned int siteId;
    QString siteName;
    QString siteAddressRange;
    QString vpnName;
    addInterfaceDialog newInterface;
    bool siteIdSelected;
    QModelIndex indexToDelete;
    int vpnVlanStart;
    int vpnVlanEnd;
    unsigned int vpnId;




};

#endif // SITEVIEWER_H
