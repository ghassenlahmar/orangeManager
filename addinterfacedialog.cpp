#include "addinterfacedialog.h"
#include "ui_addinterfacedialog.h"

#define Q(string) (string).toStdString().c_str()



extern void addrecent(unsigned int vpnId);
extern unsigned int qstringAddressToUint(QString inputAddress);


QString addressRangeAvailable(QString networkIp,QString broadcastIp, QString vpnAddressRange, unsigned int siteId,QString inputVlan,int vlanStart,int vlanEnd)
{
    if(inputVlan!="null")
    {

        if((inputVlan.toInt()>=vlanStart)&&(inputVlan.toInt()<=vlanEnd)){}
        else{return "VLAN outside current VPN VLAN range";}
    }

    unsigned int rangeStart=qstringAddressToUint(networkIp);
    unsigned int rangeEnd=qstringAddressToUint(broadcastIp);

    //bool valid=false;
    //bool available=true;


    IpCalculator calculator(vpnAddressRange);

    unsigned int bufferRangeStart=qstringAddressToUint(calculator.getNetwork());
    unsigned int bufferRangeEnd=qstringAddressToUint(calculator.getBroadcast());

    if ((rangeStart>=bufferRangeStart)&&(rangeEnd<=bufferRangeEnd)){}
    else{return "Range outside current SITE";}


    ////////////////////////////


    QString checkString=QString("select interfaceId from interfaces where (interfaceVlan=%1)AND(siteId=%2)").arg(inputVlan).arg(siteId);
    QSqlQuery checkQry;
    checkQry.prepare(checkString);
    checkQry.exec();

    /*if (!checkQry.next())
    {return "ok";}*/

    if (checkQry.next())
    {
        qDebug()<<"conflicting interface IDs";
        checkQry.next();
        qDebug()<<checkQry.value(0).toUInt();

    return("VLAN is already in use in this site");
    }

    ////////////////////////////

    checkString=QString("select interfaceId from interfaceIpIntervals where (rangeStart <= %2)AND(rangeEnd >= %1)").arg(rangeStart).arg(rangeEnd);
    //qDebug()<<"rangeStart="<<rangeStart;
    checkQry.prepare(checkString);
        checkQry.exec();

/*        if (!checkQry.next())
        {return "ok";}*/

        if (checkQry.next())
        {
            qDebug()<<"conflicting interface ranges";
            qDebug()<<checkQry.value(0).toUInt();
            while (checkQry.next())
            {
                            qDebug()<<checkQry.value(0).toUInt();
                            //QSqlQueryModel interferingVpns;
                          //  QString interferingQrytext=QString;
                                    //add an interfering VPNs viewer
            }
        return("Interfering interface ranges");
        }




    /*
    QSqlQuery getSitesinVpn;
    getSitesinVpn.exec(QString("select siteAddressRange,siteName from sites where vpnId=%1").arg(vpnId));

    QString siteAddress;
    QString interferingSites="";
    while(getSitesinVpn.next())

    {
        siteAddress=getSitesinVpn.value(0).toString();

        IpCalculator calculator(siteAddress);

        bufferRangeStart=qstringAddressToUint(calculator.getNetwork());
        bufferRangeEnd=qstringAddressToUint(calculator.getBroadcast());

        if ((rangeStart<=bufferRangeEnd)&&(rangeEnd>=bufferRangeStart))
            {
                available=false;
                interferingSites=interferingSites+"\n"+getSitesinVpn.value(1).toString();
            }


    }

    if(!available){return("Range inetrfering with sites:"+interferingSites);}

        */

return("ok");

}





bool interfaceIpValidator(QString inputIp)
{
//this function along with line_edit2's validator validates IP addresses
//the line_edit2 validator ensures correct format (no letters, fields<=4)
//this function checks for empty fields or missing fields

//try to integrate this into the validator

    //qDebug()<<"inputIp="<<inputIp;

    QStringList ipFieldsList = inputIp.split(".");

     int n = ipFieldsList.size();

     //qDebug()<<"n="<<n;

     if(n!=4) return false;



     for(int i=0;i<4;i++)
     {
        if(ipFieldsList[i].isEmpty())
            {return false;
           // qDebug()<<"field i="<<i<<"is empty";
        }
     }

     return true;
}


addInterfaceDialog::addInterfaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addInterfaceDialog)
{
    ui->setupUi(this);
}

addInterfaceDialog::~addInterfaceDialog()
{
    delete ui;
}


void addInterfaceDialog::on_pushButton_2_clicked()     //clicking on the "add Interface" button
{

    QString inputIpAddress=ui->lineEdit_2->text();
    bool noSubnetMask=ui->lineEdit_3->text().isEmpty();
    if ( (!interfaceIpValidator(inputIpAddress))  ||  (noSubnetMask) )    {
        QMessageBox::critical(this, tr("Invalid IP address"), tr("Invalid IPv4 address or subnet mask."));}
    //checking if the IP address and mask provided are valid

    else
    {

        QString inputInterfaceName=ui->lineEdit->text();

        QString inputVlan=ui->lineEdit_5->text();

        if (inputVlan=="")
        {
            inputVlan="null";
        }

        QString inputVlanName=ui->lineEdit_4->text();
        QSqlQuery checkInterfaceName;
        checkInterfaceName.prepare(QString("select * from interfaces where UPPER(interfaceName)=UPPER('%1') AND siteId=%2").arg(inputInterfaceName).arg(siteId));

        if(checkInterfaceName.exec())
        {
            if (checkInterfaceName.next())
            {
                QMessageBox::critical(this, tr("Name already exists!"), tr("Cannot create Interface: name is alreay in use in this SITE."));
            }

            else
            {
                QSqlQuery newInterfaceQry;

                QSqlQuery getnextInterfaceId;
                getnextInterfaceId.exec("select nextId from nextId where idName='interfaceId'");

                bool exists=getnextInterfaceId.first();
                //qDebug()<<"exists="<<exists;

                unsigned int nextInterfaceId;
                QString updateInterfaceId;

                if(exists){nextInterfaceId=getnextInterfaceId.value(0).toUInt();
                    updateInterfaceId=QString("update nextId set nextId=%2 Where idName='interfaceId'").arg(nextInterfaceId+1);}
                else{nextInterfaceId=0;updateInterfaceId=QString("insert into nextId values('interfaceId',1)");}
                //si l'entrée interfaceId pour nextId n'existe pas dans la base de données (base de données vide)
                //on le crée au moment d'ajouter la nouvelle interface

                QString inputMask=ui->lineEdit_3->text();
                QString qryText=QString("insert into interfaces(siteId,interfaceId,interfaceName,interfaceAddressRange,interfaceVlan,interfaceVlanName) values(%1,%2,'%3','%4/%5',%6,'%7')").arg(siteId).arg(nextInterfaceId).arg(inputInterfaceName).arg(inputIpAddress).arg(inputMask).arg(inputVlan).arg(inputVlanName);
                //qDebug()<<"qryText="<<qryText;
                //qDebug()<<"siteId="<<siteId;

                newInterfaceQry.prepare(qryText);



                QString rangeValidError;
                rangeValidError=addressRangeAvailable(networkIp, broadcastIp, siteAddressRange, siteId,inputVlan,vlanStart,vlanEnd);

                bool    ok=(rangeValidError=="ok");


                if(ok){

                ok=getnextInterfaceId.exec(updateInterfaceId);}



                if (ok){
                    QString rangeQryText=QString("insert into interfaceIpIntervals values(%1,%2,%3)").arg(rangeStart).arg(rangeEnd).arg(nextInterfaceId);


                    //qDebug()<<"nextINTERFACE ID updated";
                    //this adds the interface ranges
                    ok=getnextInterfaceId.exec(rangeQryText);

                }



                if(ok){
                ok=newInterfaceQry.exec();}

                if(ok)
                {
                    qDebug()<<Q(QString("Successfully added interface %1 (%2\\%3) with VLAN: %4 :%5.").arg(inputInterfaceName, inputIpAddress, inputMask, inputVlanName, inputVlan));
                    addrecent(vpnId);
                    emit addedInterface();
                    QMessageBox::information(this, tr("interface added"), tr("interface added!"));
                    this->close();
                }
                else
                {
                    QString errorMessage="Interface not added. Error: "+QString(newInterfaceQry.lastError().databaseText())+QString(getnextInterfaceId.lastError().databaseText())+rangeValidError;
                    QMessageBox::critical(this, tr("Interface not added"), errorMessage);
                    qDebug()<<errorMessage;
                }
            }
        }

    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Couldn't check INTERFACE availability, check database connection."));
    }
   }
}


void addInterfaceDialog::setSiteId(unsigned int inputSiteId)
{
    siteId=inputSiteId;
    //qDebug()<<"siteId:setsiteId "<<siteId;
}

void addInterfaceDialog::on_pushButton_clicked()
{
    this->close();
}



//this next part handles the subnet calculations
void addInterfaceDialog::on_lineEdit_2_editingFinished()
{
    doSubnetUpdate();
}


void addInterfaceDialog::on_lineEdit_2_textChanged(const QString &arg1)
{
    doSubnetUpdate();
}



void addInterfaceDialog::on_lineEdit_3_editingFinished()
{
    doSubnetUpdate();
}



void addInterfaceDialog::on_lineEdit_3_textChanged(const QString &arg1)
{
    doSubnetUpdate();
}



void addInterfaceDialog::doSubnetUpdate()
{
    IpCalculator calculator(ui->lineEdit_2->text()+'/'+ui->lineEdit_3->text());

    ui->label_15->setText(calculator.getBroadcast());
    broadcastIp=calculator.getBroadcast();
    ui->label_16->setText(calculator.getHostsPerNetwork());
    ui->label_17->setText(calculator.getMaxHost());
    ui->label_14->setText(calculator.getMinHost());
    ui->label_13->setText(calculator.getSubnetMask());
    ui->label_12->setText(calculator.getNetwork());
    networkIp=calculator.getBroadcast();
    rangeStart=qstringAddressToUint(networkIp);
    rangeEnd=qstringAddressToUint(broadcastIp);

    /*QString networkClass = calculator.getNetworkClass();

    if (networkClass == IpCalculator::classUndefined) {
        ui->labelInformation->setText(QString("Undefined Class"));
    } else {
        QString classInformation = QString("Class %1 Network")
            .arg(calculator.getNetworkClass());
        ui->labelInformation->setText(classInformation);
    }*/
}


void addInterfaceDialog::setSiteName(QString inputSiteName)
{
    ui->label_19->setText(inputSiteName);
}


void addInterfaceDialog::setSiteAddressRange(QString inputAddressRange)
{
    ui->label_25->setText(inputAddressRange);
    siteAddressRange=inputAddressRange;
}


void addInterfaceDialog::setSiteVlanStart(int inputVlanStart)
{
    vlanStart=inputVlanStart;
}


void addInterfaceDialog::setSiteVlanEnd(int inputVlanEnd)
{
    vlanEnd=inputVlanEnd;
}

void addInterfaceDialog::setVpnId(unsigned int inputVpnId)
{
    vpnId=inputVpnId;
    //qDebug()<<"vpnId:setvpnId "<<vpnId;
}
