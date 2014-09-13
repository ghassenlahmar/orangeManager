#include "confirmdeletedialog.h"
#include "ui_confirmdeletedialog.h"

#define Q(string) (string).toStdString().c_str()

///this function displays a delete confirmation dialog
///and takes care of deleting the entries
///////////////////////////////////////////////////////////////////////////////////

confirmDeleteDialog::confirmDeleteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::confirmDeleteDialog)
{
    ui->setupUi(this);
}



void deleteSite(unsigned int id)                              ///deletes all interfaces associated with a site
{
    QSqlQuery siteDeleteQry;
    QString deleteQryText=QString("select interfaceId from interfaces where siteId=%1").arg(id);
    siteDeleteQry.exec(deleteQryText);


    QSqlQuery interfaceDeleteQry;
    while(siteDeleteQry.next()){
        deleteQryText=QString("delete from details where interfaceId=%1").arg(siteDeleteQry.value(0).toUInt());
        interfaceDeleteQry.exec(deleteQryText);

        deleteQryText=QString("delete from interfaces where interfaceId=%1").arg(siteDeleteQry.value(0).toUInt());
        interfaceDeleteQry.exec(deleteQryText);

        deleteQryText=QString("delete from interfaceIpIntervals where interfaceId=%1").arg(siteDeleteQry.value(0).toUInt());
        interfaceDeleteQry.exec(deleteQryText);


    }
    deleteQryText=QString("delete from sites where siteId=%1").arg(id);
    siteDeleteQry.exec(deleteQryText);
}




confirmDeleteDialog::~confirmDeleteDialog()
{
    delete ui;
}

                                                                ///initializations
void confirmDeleteDialog::settings(QString inputElementType,unsigned int inputId,QString inputElementName)
{
    id=inputId;
    elementType=inputElementType;
    elementName=inputElementName;
    ui->label_2->setText(elementType+" "+elementName);

    if (elementType=="VpnGroup")
    {
        ui->label_3->setText("The associated VPNs won't be deleted");
    }
    else
    {
        ui->label_3->setText("This will cause the loss of all associated sub-elements");
    }
}


void confirmDeleteDialog::on_pushButton_clicked()       ///confirm button
{


    QString deleteQryText;
    QSqlQuery deleteQry;

    if (elementType=="Vpn")        ///for every VPN it deletes all associated sites
    {                              ///the deleteSite function deletes all interfaces for a given site
                                   ///this won't be necessary when I implement the isDeletedFlag for history viewing

        deleteQryText=QString("select siteId from sites where vpnId=%1").arg(id);
        deleteQry.exec(deleteQryText);

            while(deleteQry.next()){
                deleteSite(deleteQry.value(0).toUInt());
            }

            deleteQryText=QString("delete from VPNs where vpnId=%1").arg(id);
            deleteQry.exec(deleteQryText);

            deleteQryText=QString("delete from vpnVlanRanges where vpnId=%1").arg(id);
            deleteQry.exec(deleteQryText);

            deleteQryText=QString("delete from vpnIpIntervals where vpnId=%1").arg(id);
            deleteQry.exec(deleteQryText);
            emit deleted();

    }

    else if (elementType=="Site"){
        deleteSite(id);
        emit deleted();
    }

    else if (elementType=="VpnGroup")
    {
        deleteQryText=QString("delete from vpnGroups where groupId=%1").arg(id);
        deleteQry.exec(deleteQryText);
        emit deletedGroup();
    }

    qDebug()<<Q("Successfully deleted "+elementType+" "+elementName+".");
    this->close();
}

void confirmDeleteDialog::on_pushButton_2_clicked()     ///cancel button
{
    this->close();
}
