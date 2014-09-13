#include "vpntab.h"
#include "ui_vpntab.h"
#include "qheaderview.h"
#include <QHeaderView>

#define Q(string) (string).toStdString().c_str()

///these are the contents that appear in a VPN tab after double clicking on a VPN from the main list
///it is composed of two parts, a site list on the left, and a right pane that changes into an
///interface list (siteViewer class) or a detail list(interfaceViewer class) depending on what you do
////////////////////////////////////////////////////////////////////////////////////



///initializations
///added a stacked widget to implement the site and interface viewer
///setting up the view and queries
vpnTab::vpnTab(QString inputVpnName,QString vpnDescription,QString inputAddressRange,QString inputVlanRange,unsigned int inputVpnId):ui(new Ui::vpnTab)
{
    ui->setupUi(this);

    ui->stackedWidget->insertWidget(0,&viewSite);
    ui->stackedWidget->setCurrentIndex(0);
    //I've used a stacked widget to implement the site and interface viewer
    //I change between them by setting the index depending on where the user clicks

    vpnId=inputVpnId;
    vpnName=inputVpnName;
    addressRange=inputAddressRange;
    vlanRange=inputVlanRange;
    ui->label->setText("VPN name: "+vpnName);
    ui->label_2->setText("Description: "+vpnDescription);
    ui->label_3->setText("Address range: "+addressRange);
    ui->label_4->setText("VLAN: "+vlanRange);
    //initializations


    siteModel = new QSqlQueryModel;
    QString qry=QString("SELECT siteId,siteName,siteAddressRange FROM sites where vpnId=%1").arg(vpnId);
    siteModel->setQuery(qry);
    siteModel->setHeaderData(0, Qt::Horizontal, tr("Site id"));
    siteModel->setHeaderData(1, Qt::Horizontal, tr("Site name"));
    siteModel->setHeaderData(2, Qt::Horizontal, tr("Address range"));
    ui->tableView->setModel(siteModel);
    ui->tableView->setColumnHidden(0,1);

    ui->tableView->horizontalHeader()->setDefaultSectionSize(120);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    //fetching the site list and sizing up elements

    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,2);


    //connecting signals

    connect(&viewSite, SIGNAL(interfaceDetail(uint,QString,QString)),this, SLOT(detailRequested(uint,QString,QString)));
    //signal to view the details of an interface after double click

    connect(&viewInterface, SIGNAL(back()),this, SLOT(backToInterfaces()));
    //signal to switch back to the interfaces list from the details list

    connect(&addSite, SIGNAL(addedSite()),this, SLOT(refreshSite()));
    connect(&del, SIGNAL(deleted()),this, SLOT(refreshSite()));
    connect(&del, SIGNAL(deleted()),&viewSite, SLOT(refreshInterfaces()));
    //signal to refresh lists after deletion of site/interface or successfully adding a site

}
vpnTab::~vpnTab()
{
    delete ui;
}



void vpnTab::on_tableView_clicked(const QModelIndex &index)    ///clicking on a site from the list
{
    //clicking on the table
    //caches the current element info for use with the add/delete buttons


    //int siteInterfacesNumber;

    siteId=siteModel->data(siteModel->index(index.row(),0)).toInt();
    siteName=siteModel->data(siteModel->index(index.row(),1)).toString();
    siteAddressRange=siteModel->data(siteModel->index(index.row(),2)).toString();
    //siteInterfacesNumber=siteModel->data(siteModel->index(index.row(),0)).toInt();
    //qDebug()<<"siteId="<<siteId;

    viewSite.settings(siteId,siteName,siteAddressRange,vlanRange);
    viewSite.setVpnId(vpnId);
    viewSite.setVpnName(vpnName);

}


void vpnTab::on_pushButton_clicked()            ///add site button
{
    addSite.setVpnId(vpnId);
    addSite.setVpnName(vpnName);
    addSite.setVpnAddressRange(addressRange);
    addSite.show();
}


void vpnTab::refreshSite()          ///slot to refresh the sites list, called after adding/deleting sites
{
    QString qry=QString("SELECT siteId,siteName,siteAddressRange FROM sites where vpnId=%1").arg(vpnId);
    siteModel->setQuery(qry);

    //qDebug()<<"SITE list refreshed";
}



///slot to switch the right pane to detail view
/// activated when doubleclicking on a detail from the interface view
void vpnTab::detailRequested(unsigned int inputInterfaceId,QString inputInterfaceName, QString inputInterfaceAddressRange)
{
    //I delete the previously used detail widget before adding a new one
    ui->stackedWidget->removeWidget(ui->stackedWidget->widget(1));
    ui->stackedWidget->insertWidget(1,&viewInterface);
    viewInterface.set(inputInterfaceId,inputInterfaceName,inputInterfaceAddressRange);
    viewInterface.setVpnId(vpnId);
    viewInterface.setVpnName(vpnName);
    ui->stackedWidget->setCurrentIndex(1);
}


void vpnTab::backToInterfaces()     ///back button, returns from detailViewer to interface viewer
{
    ui->stackedWidget->setCurrentIndex(0);

}


void vpnTab::on_pushButton_2_clicked()  ///delete site button
{
    del.settings("Site",siteId,siteName);
    del.show();
}


unsigned int vpnTab::getVpnId()         ///returns the vpnId
{
    return (vpnId);
}

QString vpnTab::getVpnName()         ///returns the vpnName
{
    return (vpnName);
}

QString vpnTab::getVpnAddressRange()         ///returns the vpnName
{
    return (addressRange);
}

QString vpnTab::getVpnVlanRange()         ///returns the VLAN Range
{
    return (vlanRange);
}

///the next ones are unused, I was trying to implement a pointer
///to avoid passing all the values to subClasses but ran into compiler problems

void vpnTab::setPointer(vpnTab* inputPointer)
{
    //pointer=inputPointer;
}

QString vpnTab::getSiteAddressRange()         ///returns the site Address Range
{
    return (siteAddressRange);
}

unsigned int vpnTab::getSiteId()         ///returns the site id
{
    return (siteId);
}

QString vpnTab::getSiteName()         ///returns the site Name
{
    return (siteName);
}
