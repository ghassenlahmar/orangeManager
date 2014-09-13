#include "siteviewer.h"
#include "ui_siteviewer.h"
#include "vpntab.h"

#define Q(string) (string).toStdString().c_str()

///this is the siteviewer widget
///it appears on the right pane of the vpnTab and displays interfaces of a certain site
/////////////////////////////////////////////////////////////////

siteViewer::siteViewer(QWidget *parent) :       ///initializations
    QWidget(parent),
    ui(new Ui::siteViewer)
{
    ui->setupUi(this);

    //this views all interfaces in a given site


    int siteInterfacesNumber;
    siteIdSelected=false;                   //siteIdSelected: if a site ID sin't selected you can't add new interface
    interfacesModel = new QSqlQueryModel;
    QString interfaceQry=QString("SELECT interfaceId,interfaceName,interfaceVlanName,interfaceVlan,interfaceAddressRange FROM interfaces where siteId=%1").arg(siteId);
    interfacesModel->setQuery(interfaceQry);
    interfacesModel->setHeaderData(0, Qt::Horizontal, tr("Interface id"));
    interfacesModel->setHeaderData(1, Qt::Horizontal, tr("Interface"));
    interfacesModel->setHeaderData(2, Qt::Horizontal, tr("Vlan Name"));
    interfacesModel->setHeaderData(3, Qt::Horizontal, tr("Vlan"));
    interfacesModel->setHeaderData(4, Qt::Horizontal, tr("Interface address range"));
    ui->tableView_2->setModel(interfacesModel);
    ui->tableView_2->setColumnHidden(0,1);



    ui->tableView_2->horizontalHeader()->setDefaultSectionSize(120);
    ui->tableView_2->horizontalHeader()->setStretchLastSection(true);
    connect(&newInterface, SIGNAL(addedInterface()),this, SLOT(refreshInterfaces()));
}
siteViewer::~siteViewer()
{
    delete ui;
}


///settings, decides which site will be displayed
void siteViewer::settings(unsigned int inputSiteId,QString inputSiteName,QString inputSiteAddressRange, QString inputVlanRange)
{
        //pointer=inputPointer;

    //ui->tableView_2->setModel(interfacesModel);

    siteId=inputSiteId;
    QString interfaceQry=QString("SELECT interfaceId,interfaceName,interfaceVlanName,interfaceVlan,interfaceAddressRange FROM interfaces where siteId=%1").arg(siteId);
    interfacesModel->setQuery(interfaceQry);
    ui->tableView_2->setColumnHidden(0,1);
    siteName=inputSiteName;
    ui->label_4->setText("Site name :"+siteName);
    siteAddressRange=inputSiteAddressRange;
    ui->label_5->setText("Address range: "+siteAddressRange);
//ui->label_6->setText("Number of interfaces: "+siteInterfacesNumber);
    QStringList vlanStartEnd = inputVlanRange.split("-");

    vpnVlanStart=vlanStartEnd[0].toInt();
    vpnVlanEnd=vlanStartEnd[1].toInt();

    siteIdSelected=true;
}


void siteViewer::on_pushButton_4_clicked()              ///add interface button
{
    if(siteIdSelected)
    {
        newInterface.setSiteId(siteId);
        newInterface.setSiteName(siteName);
        newInterface.setSiteAddressRange(siteAddressRange);
        newInterface.setSiteVlanStart(vpnVlanStart);
        newInterface.setSiteVlanEnd(vpnVlanEnd);
        newInterface.setVpnId(vpnId);
        newInterface.show();
    }

    else
    {
        QMessageBox::critical(this, tr("No site selected!"), tr("You must select a site before creating a new interface."));
    }
}

void siteViewer::refreshInterfaces()                ///slot to refresh the interfaces list
{

    QString interfaceQry=QString("SELECT interfaceId,interfaceName,interfaceVlanName,interfaceVlan,interfaceAddressRange FROM interfaces where siteId=%1").arg(siteId);
    interfacesModel->setQuery(interfaceQry);

    //qDebug()<<"interface list refreshed";

}


///doubleclicking on an interface
///sends a detail to vpntab to change the pane
void siteViewer::on_tableView_2_doubleClicked(const QModelIndex &index)
{
    unsigned int interfaceId;
    QString interfaceName,interfaceAddressRange;
    interfaceId=interfacesModel->data(interfacesModel->index(index.row(),0)).toInt();
    interfaceName=interfacesModel->data(interfacesModel->index(index.row(),1)).toString();
    interfaceAddressRange=interfacesModel->data(interfacesModel->index(index.row(),4)).toString();
    //siteInterfacesNumber=siteModel->data(siteModel->index(index.row(),0)).toInt();
    //qDebug()<<"interfaceId="<<interfaceId;
    emit interfaceDetail(interfaceId,interfaceName,interfaceAddressRange);

}

void siteViewer::on_tableView_2_clicked(const QModelIndex &index) ///selects an interface to delete
{
    indexToDelete=index;
}

///add a debug message
void siteViewer::on_pushButton_3_clicked()          ///delete interface button
{
    unsigned int interfaceId;
    QSqlQuery deleteQry;
    interfaceId=interfacesModel->data(interfacesModel->index(indexToDelete.row(),0)).toInt();
    QString qryText=QString("delete from interfaces where interfaceId=%1").arg(interfaceId);

    deleteQry.exec(qryText);
    qryText=QString("delete from interfaceIpIntervals where interfaceId=%1").arg(interfaceId);

    deleteQry.exec(qryText);
    refreshInterfaces();
}

void siteViewer::setVpnId(unsigned int inputVpnId)          ///sets the current vpnId
{
    vpnId=inputVpnId;
    //qDebug()<<"vpnId:setvpnId "<<vpnId;
}

void siteViewer::setVpnName(QString inputVpnName)           ///sets the current vpnName
{
    vpnName=inputVpnName;
}
