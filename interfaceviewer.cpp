#include "interfaceviewer.h"
#include "ui_interfaceviewer.h"

#define Q(string) (string).toStdString().c_str()



unsigned int countUsedDetails(unsigned int interfaceId)         ///returns the number of used details to display
{                                                               ///against the total available

    QSqlQuery usedDetails;
    usedDetails.exec(QString("select detailId from details where interfaceId=%1").arg(interfaceId));

    unsigned int recCount = 0;
    while( usedDetails.next())
    {
        recCount++;
    }

    //qDebug()<<"recCount="<<recCount;

    return recCount;
}



interfaceViewer::interfaceViewer(QWidget *parent) :             ///initializations
    QWidget(parent),
    ui(new Ui::interfaceViewer)
{
    ui->setupUi(this);

    detailsModel = new QSqlQueryModel;
    QString detailQry=QString("SELECT detailId,detailAddress,localPort,comment FROM details where interfaceId=%1").arg(interfaceId);
    detailsModel->setQuery(detailQry);
    detailsModel->setHeaderData(0, Qt::Horizontal, tr("Detail id"));
    detailsModel->setHeaderData(1, Qt::Horizontal, tr("address"));
    detailsModel->setHeaderData(2, Qt::Horizontal, tr("local port"));
    detailsModel->setHeaderData(3, Qt::Horizontal, tr("comment"));

    ui->tableView_3->setModel(detailsModel);
    ui->tableView_3->setColumnHidden(0,1);

    ui->tableView_3->horizontalHeader()->setDefaultSectionSize(120);
    ui->tableView_3->horizontalHeader()->setStretchLastSection(true);

    connect(&addDetail, SIGNAL(addedDetail()),this, SLOT(refreshDetails()));
}
interfaceViewer::~interfaceViewer()
{
    delete ui;
}

                                                        ///gets the settings, interfaceId etc...
void interfaceViewer::set(unsigned int inputInterfaceId, QString inputInterfaceName, QString inputInterfaceAddressRange)
{
    interfaceId=inputInterfaceId;
    interfaceName=inputInterfaceName;
    interfaceAddressRange=inputInterfaceAddressRange;
    ui->label_7->setText("Platform name "+interfaceName);
    ui->label_8->setText("Address range "+interfaceAddressRange);
    QString detailQry=QString("SELECT detailId,detailAddress,localPort,comment FROM details where interfaceId=%1").arg(interfaceId);
    detailsModel->setQuery(detailQry);
    IpCalculator calculator(interfaceAddressRange);
    ui->label_2->setText(calculator.getHostsPerNetwork());
    ui->label->setText(tr("%1").arg(countUsedDetails(interfaceId)));


}


void interfaceViewer::on_pushButton_clicked()     ///back to interfaces list button
{
    emit back();
    //qDebug()<<"back button pressed";
}


void interfaceViewer::on_pushButton_6_clicked()             ///addDetail button
{
    IpCalculator calculator(interfaceAddressRange);

    addDetail.setRangeStart(calculator.getMinHost());
    addDetail.setRangeEnd(calculator.getMaxHost());

    addDetail.setInterfaceId(interfaceId);
    addDetail.setInterfaceName(interfaceName);

    addDetail.setVpnId(vpnId);
    addDetail.setVpnName(vpnName);

    addDetail.show();
}


void interfaceViewer::refreshDetails()                       ///refreshes the details list
{    
    detailsModel->setQuery(detailsModel->query().lastQuery());
    ui->label->setText(tr("%1").arg(countUsedDetails(interfaceId)));
}


void interfaceViewer::on_tableView_3_clicked(const QModelIndex &index)      ///clicking a detail selects it
{    
    selectedDetailId=detailsModel->data(detailsModel->index(index.row(),0)).toUInt();
}

void interfaceViewer::on_pushButton_5_clicked()                             ///delete detail button
{
    QSqlQuery deleteDetail;
    QString deleteDetailText=QString("delete from details where detailId=%1").arg(selectedDetailId);
    deleteDetail.exec(deleteDetailText);
    refreshDetails();
}


void interfaceViewer::setVpnId(unsigned int inputVpnId)             ///sets the vpnId
{
    vpnId=inputVpnId;    
}

void interfaceViewer::setVpnName(QString inputVpnName)              ///sets the vpnName
{
    vpnName=inputVpnName;
}
