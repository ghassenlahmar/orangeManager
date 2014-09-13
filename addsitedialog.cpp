#include "addsitedialog.h"
#include "ui_addsitedialog.h"

#define Q(string) (string).toStdString().c_str()

extern void addrecent(unsigned int vpnId);
extern unsigned int qstringAddressToUint(QString inputAddress);
extern bool ipValidator(QString inputIp);


QString addressRangeValidAvailable(QString networkIp,QString broadcastIp, QString vpnAddressRange, unsigned int vpnId)
{
    unsigned int rangeStart=qstringAddressToUint(networkIp);
    unsigned int rangeEnd=qstringAddressToUint(broadcastIp);

    //bool valid=false;
    bool available=true;


    IpCalculator calculator(vpnAddressRange);

    unsigned int bufferRangeStart=qstringAddressToUint(calculator.getNetwork());
    unsigned int bufferRangeEnd=qstringAddressToUint(calculator.getBroadcast());

    if ((rangeStart>=bufferRangeStart)&&(rangeEnd<=bufferRangeEnd)){}
    else{return "Range outside current VPN";}


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
                interferingSites=interferingSites+" "+getSitesinVpn.value(1).toString();
            }


    }

    if(!available){return("Range already used in sites:"+interferingSites+".");}

    return("ok");

}



/*bool siteIpValidator(QString inputIp)
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
            {return false;//qDebug()<<"field i="<<i<<"is empty";
        }
     }

     return true;
}*/




addSiteDialog::addSiteDialog(QWidget *parent) :
    QDialog(parent),ui(new Ui::addSiteDialog)
{
    ui->setupUi(this);

    //IP address field validator
    QString Octet = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QString Mask = "(?:[0-2]?[0-9]|3[0-2])";

    ui->lineEdit_2->setValidator(new QRegExpValidator(
                                     QRegExp("^" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + "$"), this));
    ui->lineEdit_3->setValidator(new QRegExpValidator(
                                     QRegExp("^" + Mask +  "\\." + Mask + "$"), this));




}

addSiteDialog::~addSiteDialog()
{
    delete ui;
}


//this next part handles the subnet calculations


void addSiteDialog::on_lineEdit_2_textChanged(const QString &arg1)
{
    doSubnetUpdate();
}

void addSiteDialog::on_lineEdit_2_editingFinished()
{
    doSubnetUpdate();
}




void addSiteDialog::on_lineEdit_3_editingFinished()
{
    doSubnetUpdate();
}



void addSiteDialog::on_lineEdit_3_textChanged(const QString &arg1)
{
    doSubnetUpdate();
}




void addSiteDialog::doSubnetUpdate()
{
    IpCalculator calculator(ui->lineEdit_2->text()+'/'+ui->lineEdit_3->text());

    ui->label_15->setText(calculator.getBroadcast());
    broadcastIp=calculator.getBroadcast();

    ui->label_16->setText(calculator.getHostsPerNetwork());
    ui->label_17->setText(calculator.getMaxHost());
    ui->label_14->setText(calculator.getMinHost());
    ui->label_13->setText(calculator.getSubnetMask());

    ui->label_12->setText(calculator.getNetwork());
    networkIp=calculator.getNetwork();

    /*QString networkClass = calculator.getNetworkClass();

    if (networkClass == IpCalculator::classUndefined) {
        ui->labelInformation->setText(QString("Undefined Class"));
    } else {
        QString classInformation = QString("Class %1 Network")
            .arg(calculator.getNetworkClass());
        ui->labelInformation->setText(classInformation);
    }*/
}




void addSiteDialog::on_pushButton_2_clicked()     //clicking on the "add site" button
{

    QString inputIpAddress=ui->lineEdit_2->text();
    bool noSubnetMask=ui->lineEdit_3->text().isEmpty();

    if ( (!ipValidator(inputIpAddress))  ||  (noSubnetMask) )    {
        QMessageBox::critical(this, tr("Invalid IP address"), tr("Invalid IPv4 address or subnet mask."));}
    //checking if the IP address and mask provided are valid

    else
    {

        QString inputSiteName=ui->lineEdit->text();
        QSqlQuery checkSiteName;
        checkSiteName.prepare(QString("select * from sites where UPPER(siteName)=UPPER('%1') AND vpnId=%2").arg(inputSiteName).arg(vpnId));

        if(checkSiteName.exec())
        {
            if (checkSiteName.next())
            {
                QMessageBox::critical(this, tr("Name already exists!"), tr("Cannot create SITE: name is alreay in use in this VPN."));
                qDebug()<<Q(QString("Site name %1 already used in vpn %2.").arg(inputSiteName,vpnName));
            }

            else
            {
                QSqlQuery newSiteQry;

                QSqlQuery getNextSiteId;
                getNextSiteId.exec("select nextId from nextId where idName='siteId'");

                bool exists=getNextSiteId.first();
                //qDebug()<<"exists="<<exists;

                unsigned int nextSiteId;
                QString updateSiteId;

                if(exists){nextSiteId=getNextSiteId.value(0).toUInt();
                    updateSiteId=QString("update nextId set nextId=%2 Where idName='siteId'").arg(nextSiteId+1);}
                else{nextSiteId=0;updateSiteId=QString("insert into nextId values('siteId',1)");}
                //si l'entrée siteId pour nextId n'existe pas dans la base de données (base de données vide)
                //on le crée au moment d'ajouter le nouveau SITE

                QString inputMask=ui->lineEdit_3->text();
                QString qryText=QString("insert into sites(vpnId,siteId,siteName,siteAddressRange) values(%1,%2,'%3','%4/%5')").arg(vpnId).arg(nextSiteId).arg(inputSiteName).arg(inputIpAddress).arg(inputMask);
                //qDebug()<<"qryText="<<qryText;
                //qDebug()<<"vpnId="<<vpnId;

                newSiteQry.prepare(qryText);

                //bool ok=getNextVpnId.exec(updateVpnId);

                QString rangeValidError;
                rangeValidError=addressRangeValidAvailable(networkIp, broadcastIp, vpnAddressRange, vpnId);

                bool    ok=(rangeValidError=="ok");

                if(ok){
                rangeValidError="";
                getNextSiteId.exec(updateSiteId);}



                if(ok){
                ok=newSiteQry.exec();}

                if(ok)
                {
                    QMessageBox::information(this, tr("site added"), tr("site added!"));
                    addrecent(vpnId);
                    emit addedSite();
                    this->close();

                    qDebug()<<Q(QString("Successfully added site %1 (%2/%3) to VPN %4.").arg(inputSiteName).arg(inputIpAddress).arg(inputMask).arg(vpnName));
                }
                else
                {
                    QString errorMessage="Site not added. Error: "+QString(newSiteQry.lastError().databaseText())+QString(getNextSiteId.lastError().databaseText())+rangeValidError;
                    QMessageBox::critical(this, tr("Site not added"), errorMessage);

                    qDebug()<<Q(errorMessage);
                }
            }
        }

    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Couldn't check SITE availability, check database connection."));
    }
   }
}


/*void addSiteDialog::on_pushButton_2_clicked()     //clicking on the "add site" button
{

    QString inputIpAddress=ui->lineEdit_2->text();
    bool noSubnetMask=ui->lineEdit_3->text().isEmpty();

    if ( (!siteIpValidator(inputIpAddress))  ||  (noSubnetMask) )    {
        QMessageBox::critical(this, tr("Invalid IP address"), tr("Invalid IPv4 address or subnet mask."));}
    //checking if the IP address and mask provided are valid

    else
    {

        QString inputSiteName=ui->lineEdit->text();
        QSqlQuery checkSiteName;
        checkSiteName.prepare(QString("select * from sites where UPPER(siteName)=UPPER('%1') AND vpnId=%2").arg(inputSiteName).arg(vpnId));

        if(checkSiteName.exec())
        {
            if (checkSiteName.next())
            {
                QMessageBox::critical(this, tr("Name already exists!"), tr("Cannot create SITE: name is alreay in use in this VPN."));
                qDebug()<<Q(QString("Site name %1 already used in vpn %2.").arg(inputSiteName,vpnName));
            }

            else
            {
                QSqlQuery newSiteQry;

                QSqlQuery getNextSiteId;
                getNextSiteId.exec("select nextId from nextId where idName='siteId'");

                bool exists=getNextSiteId.first();
                //qDebug()<<"exists="<<exists;

                unsigned int nextSiteId;
                QString updateSiteId;

                if(exists){nextSiteId=getNextSiteId.value(0).toUInt();
                    updateSiteId=QString("update nextId set nextId=%2 Where idName='siteId'").arg(nextSiteId+1);}
                else{nextSiteId=0;updateSiteId=QString("insert into nextId values('siteId',1)");}
                //si l'entrée siteId pour nextId n'existe pas dans la base de données (base de données vide)
                //on le crée au moment d'ajouter le nouveau SITE

                QString inputMask=ui->lineEdit_3->text();
                QString qryText=QString("insert into sites(vpnId,siteId,siteName,siteAddressRange) values(%1,%2,'%3','%4/%5')").arg(vpnId).arg(nextSiteId).arg(inputSiteName).arg(inputIpAddress).arg(inputMask);
                //qDebug()<<"qryText="<<qryText;
                //qDebug()<<"vpnId="<<vpnId;

                newSiteQry.prepare(qryText);

                //bool ok=getNextVpnId.exec(updateVpnId);

                QString rangeValidError;
                rangeValidError=addressRangeValidAvailable(networkIp, broadcastIp, vpnAddressRange, vpnId);

                bool    ok=(rangeValidError=="ok");

                if(ok){
                rangeValidError="";
                getNextSiteId.exec(updateSiteId);}



                if(ok){
                ok=newSiteQry.exec();}

                if(ok)
                {
                    QMessageBox::information(this, tr("site added"), tr("site added!"));
                    addrecent(vpnId);
                    emit addedSite();
                    this->close();

                    qDebug()<<Q(QString("Successfully added site %1 (%2/%3) to VPN %4.").arg(inputSiteName).arg(inputIpAddress).arg(inputMask).arg(vpnName));
                }
                else
                {
                    QString errorMessage="Site not added. Error: "+QString(newSiteQry.lastError().databaseText())+QString(getNextSiteId.lastError().databaseText())+rangeValidError;
                    QMessageBox::critical(this, tr("Site not added"), errorMessage);

                    qDebug()<<Q(errorMessage);
                }
            }
        }

    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Couldn't check SITE availability, check database connection."));
    }
   }
}*/

void addSiteDialog::setVpnId(unsigned int inputVpnId)
{
    vpnId=inputVpnId;
    //qDebug()<<"vpnId:setvpnId "<<vpnId;
}

void addSiteDialog::on_pushButton_clicked()
{
    this->close();
}

void addSiteDialog::setVpnName(QString inputVpnName)
{
    vpnName=inputVpnName;
    ui->label_19->setText(inputVpnName);
}


void addSiteDialog::setVpnAddressRange(QString inputAddressRange)
{
    ui->label_25->setText(inputAddressRange);
    vpnAddressRange=inputAddressRange;
}



