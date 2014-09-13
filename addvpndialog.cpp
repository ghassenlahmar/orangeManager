#include "addvpndialog.h"
#include "ui_addvpndialog.h"
#include "mainwindow.h"


#define Q(string) (string).toStdString().c_str()

extern void addrecent(unsigned int vpnId);


bool ipValidator(QString inputIp)          ///this function checks for empty or missing fields in the address field
{
//this function along with line_edit2's validator validates IP addresses
//the line_edit2 validator ensures correct format (no letters, fields<=4)
//this function checks for empty fields or missing fields

    //qDebug()<<"inputIp="<<inputIp;

    QStringList ipFieldsList = inputIp.split(".");
    int n = ipFieldsList.size();

    //qDebug()<<"n="<<n;


    if(n!=4) return false;

    for(int i=0; i<4; i++)
    {
        if(ipFieldsList[i].isEmpty())
        {
            return false;
            //qDebug()<<"field i="<<i<<"is empty";
        }
    }

    return true;
}



unsigned int qstringAddressToUint(QString inputAddress)         ///this function converts an IPv4
{                                                               ///address in string format
                                                                ///to unsigned int format
                                                                ///this is used in databse to
                                                                ///check overlapping intervals
    unsigned int address;
    QHostAddress addressBuffer;
    addressBuffer.setAddress(inputAddress);
    address=addressBuffer.toIPv4Address();
    return address;
}

            ///the next function checks if the IP range is available
            ///this is done just before adding the VPN and after checking the VPN name is available
            ///work in progress

bool vpnIpAvailable(unsigned int inputRangeStart,unsigned int inputRangeEnd)
{

    QString checkString=QString("select vpnId from vpnIpIntervals where (rangeStart <= %2)AND(rangeEnd >= %1)").arg(inputRangeStart).arg(inputRangeEnd);
    QSqlQuery checkQry;
    checkQry.prepare(checkString);
    checkQry.exec();

    if (!checkQry.next())
    {
        return true;
    }

    QString conflictQry=QString("select vpnName,vpnAddressRange from VPNs where vpnId=%1 ").arg(checkQry.value(0).toUInt());
    while (checkQry.next())
    {
        conflictQry=conflictQry+QString("or vpnId=%1 ").arg(checkQry.value(0).toUInt());

    }
    unsigned int cntr=0;
    checkQry.exec(conflictQry);

    qDebug()<<Q(QString("VPNs using this address range:"));
    while(checkQry.next())
    {
        cntr++;
        IpCalculator calculator(checkQry.value(1).toString());
        QString fCntr, fName,fAddress,fNetwork,fBroadcast;
        fCntr=QString("%1").arg(cntr).leftJustified(5,' ',false);
        fName=QString("%1").arg(checkQry.value(0).toString()).leftJustified(25,' ',false);
        fAddress=QString("%1").arg(checkQry.value(1).toString()).leftJustified(20,' ',false);
        fNetwork=QString("%1").arg(calculator.getNetwork()).leftJustified(15,' ',false);
        fBroadcast=QString("%1").arg(calculator.getBroadcast()).leftJustified(15,' ',false);
        qDebug()<<Q(QString("VPN %1: %2 Address Range:%3 Network:%4 Broadcast:%5").arg(fCntr,  fName  ,  fAddress  ,  fNetwork  ,  fBroadcast  ));
    }
    return false;
}


bool vpnVlanAvailable(int inputRangeStart,int inputRangeEnd)
{


    QString checkString=QString("select vpnId from vpnVlanRanges where (rangeStart <= %2)AND(rangeEnd >= %1)").arg(inputRangeStart).arg(inputRangeEnd);
    QSqlQuery checkQry;
    checkQry.prepare(checkString);
    checkQry.exec();

    if (!checkQry.next())
    {
        return true;
    }


    //qDebug()<<checkQry.value(0).toUInt();
    //unsigned int cntr=

    QString conflictQry=QString("select vpnName,vlanRange from VPNs where vpnId=%1 ").arg(checkQry.value(0).toUInt());
    while (checkQry.next())
    {
        conflictQry=conflictQry+QString("or vpnId=%1 ").arg(checkQry.value(0).toUInt());

    }
    unsigned int cntr=0;
    checkQry.exec(conflictQry);

    qDebug()<<Q(QString("VPNs using this vlan range:"));
    while(checkQry.next())
    {
        cntr++;

        QString fCntr, fName,fRangeStart,fRangeEnd;
        QStringList vlanStartEnd = checkQry.value(1).toString().split("-");
        int vpnVlanStart=vlanStartEnd[0].toInt();
        int vpnVlanEnd=vlanStartEnd[1].toInt();

        fCntr=QString("%1").arg(cntr).leftJustified(5,' ',false);
        fName=QString("%1").arg(checkQry.value(0).toString()).leftJustified(25,' ',false);
        fRangeStart=QString("%1").arg(vpnVlanStart).rightJustified(5,' ',false);
        fRangeEnd=QString("%1").arg(vpnVlanEnd).rightJustified(5,' ',false);

        qDebug()<<Q(QString("VPN %1: %2 VLAN range: %3  - %4").arg(fCntr,fName,fRangeStart,fRangeEnd));
    }

    return false;
}


addVpnDialog::addVpnDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addVpnDialog)
{
    ui->setupUi(this);
    ui->lineEdit_4->setPlaceholderText("start");
    ui->lineEdit_5->setPlaceholderText("end");



    ///IP address field validator
    QString Octet = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QString Mask = "(?:[0-2]?[0-9]|3[0-2])";
    QString VLAN="(?:[0-9]?[0-9]?[0-9]?[0-9])";

    ui->lineEdit_2->setValidator(new QRegExpValidator(
                                     QRegExp("^" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + "$"), this));

    ///This validator allows line_edit to only accept valid IP addresses formats while typing
    ///it doesn't check for missing or empty fields though
    ///this is what the function vpnIpValidator does



    ui->lineEdit_3->setValidator(new QRegExpValidator(
                                     QRegExp("^" + Mask +  "\\." + Mask + "$"), this));


    ///validator on vlan fields
    ui->lineEdit_4->setValidator(new QRegExpValidator(QRegExp("^" + VLAN + "$"), this));
    ui->lineEdit_5->setValidator(new QRegExpValidator(QRegExp("^" + VLAN + "$"), this));

}






addVpnDialog::~addVpnDialog()
{
    delete ui;
}





///Maybe I'll organize the next part in classes or functions
void addVpnDialog::on_pushButton_2_clicked()     ///clicking on the "add VPN" button
{
    int vlanStart=ui->lineEdit_4->text().toInt();
    int vlanEnd=ui->lineEdit_5->text().toInt();
    QString inputIpAddress=ui->lineEdit_2->text();
    bool noSubnetMask=ui->lineEdit_3->text().isEmpty();

    while(true)
    {

        if ( (!ipValidator(inputIpAddress))  ||  (noSubnetMask) )  ///checking if the IP address and
        {                                                             /// mask provided are valid
            QMessageBox::critical(this, tr("Invalid IP address"), tr("Invalid IPv4 address or subnet mask."));
            break;
        }


        QString inputVpnName=ui->lineEdit->text();
        QString inputDescription=ui->plainTextEdit->toPlainText();
        QString inputMask=ui->lineEdit_3->text();
        QSqlQuery checkVpnName;
        checkVpnName.prepare(QString("select vpnName, vpnAddressRange, vlanRange, description from VPNs where UPPER(vpnName)=UPPER('%1')").arg(inputVpnName));
        ///this query checks whether the name is already taken, case insensitive
        ///this is redundant if the databse uses UNIQUE ids, maybe I'll remove it

        if(checkVpnName.exec())
        {


            if (checkVpnName.next())
            {
                QMessageBox::critical(this, tr("Name used!"), tr("Cannot create VPN: name is alreay in use."));
                qDebug()<<Q(QString("VPN name %1 is already in use (%2, %3, %4 ,\"%5\").").arg(inputVpnName,checkVpnName.value(0).toString(),checkVpnName.value(1).toString(),checkVpnName.value(2).toString(),checkVpnName.value(3).toString()));
                break;
            }

            qDebug()<<Q(QString("VPN Name %1 available").arg(inputVpnName));
            QSqlQuery newVpnQry;
            QSqlQuery getNextVpnId;

            getNextVpnId.exec("select nextId from nextId where idName='vpnId'");

            bool exists=getNextVpnId.first();
            unsigned int nextVpnId;
            QString updateVpnId;

            if(exists)
            {
                    nextVpnId=getNextVpnId.value(0).toUInt();
                    updateVpnId=QString("update nextId set nextId=%2 Where idName='vpnId'").arg(nextVpnId+1);
            }

            else

            {
                    nextVpnId=0;
                    updateVpnId=QString("insert into nextId values('vpnId',1)");
            }
            //si l'entrée vpnId pour nextId n'existe pas dans la base de données (base de données vide)
            //on le crée au moment d'ajouter le nouveau VPN


            rangeStart=qstringAddressToUint(networkIp);
            rangeEnd=qstringAddressToUint(broadcastIp);

            if(!vpnIpAvailable(rangeStart,rangeEnd))            ///check if the address range is not in use
            {
                QMessageBox::critical(this, tr("Address range in use!"), tr("address range used by another VPN"));
                qDebug()<<"VPN address range in use, view the above list for conflicting VPNs.";
                break;
            }

            qDebug()<<Q(QString("VPN address range %1/%2 available.").arg(inputIpAddress,inputMask));


            if(vlanStart>vlanEnd)                       ///check for valid VPN range entered
            {
                QMessageBox::critical(this, tr("Invalid VLAN range!"), tr("Invalid VLAN range"));
                qDebug()<<Q(QString("Invalid VLAN range entered"));
                break;
            }


            if(!vpnVlanAvailable(vlanStart,vlanEnd))         ///check if the vpn Range is available
            {
                QMessageBox::critical(this, tr("VLAN range in use!"), tr("VLAN range used by another VPN"));
                qDebug()<<Q(QString("VPN VLAN range in use, view the above list for conflicting VPNs."));
                break;
            }


            qDebug()<<"VPN address range available";



            bool ok=getNextVpnId.exec(updateVpnId);         ///update the nextVpnId table
            qDebug()<<"Updating database.";


            if (ok)                                         ///if successful update the vpnIpIntervals table
            {
                QString rangeQryText=QString("insert into vpnIpIntervals values(%1,%2,%3)").arg(rangeStart).arg(rangeEnd).arg(nextVpnId);
                ok=getNextVpnId.exec(rangeQryText);
            }

            if (ok)                                          ///if successful update the vpnVlanRanges table
            {
                //qDebug()<<"rangeintervals updated";
                QString vlanQryText=QString("insert into vpnVlanRanges values(%1,%2,%3)").arg(vlanStart).arg(vlanEnd).arg(nextVpnId);
                ok=getNextVpnId.exec(vlanQryText);

            }

            if (ok)                                          ///isert values into VPNs table
            {
                //qDebug()<<"VLANintervals updated";
                QString qryText=QString("insert into VPNs values(%1,'%2','%3','%4/%5','%6-%7')").arg(nextVpnId).arg(inputVpnName).arg(inputDescription).arg(inputIpAddress).arg(inputMask).arg(vlanStart).arg(vlanEnd);
                newVpnQry.prepare(qryText);
                ok=newVpnQry.exec();

            }

            if(ok)                                          ///successfully added VPN message
            {
                addrecent(nextVpnId);
                qDebug()<<Q(QString("Successfully added VPN %1 (address range: %2/%3, VLAN range: %4-%5) with description: \"%6\".").arg(inputVpnName).arg(inputIpAddress).arg(inputMask).arg(vlanStart).arg(vlanEnd).arg(inputDescription));
                QMessageBox::information(this, tr("VPN added"), tr("VPN added!"));
                emit addedVpn();
                this->close();
                break;
            }


                                        ///if (!ok) display the error message
            QString errorMessage="VPN not added: "+QString(newVpnQry.lastError().databaseText())+QString(getNextVpnId.lastError().databaseText());
            QMessageBox::critical(this, tr("VPN not added"), errorMessage);
            qDebug()<<errorMessage;
            break;

            //to implement:
            //not successful?-->rollback changes //db.exec("delete from VPNs where vpnId=%1"); //etc

        }

                        ///if couldn't check vpn name availability
        QMessageBox::critical(this, tr("Error"), tr("Couldn't check VPN availability, check database connection."));
        break;
   }
}

/*
void addVpnDialog::on_pushButton_2_clicked()     ///clicking on the "add VPN" button
{
    int vlanStart=ui->lineEdit_4->text().toInt();
    int vlanEnd=ui->lineEdit_5->text().toInt();
    QString inputIpAddress=ui->lineEdit_2->text();
    bool noSubnetMask=ui->lineEdit_3->text().isEmpty();

    while(true)
    {

        if ( (!vpnIpValidator(inputIpAddress))  ||  (noSubnetMask) )  ///checking if the IP address and
        {                                                             /// mask provided are valid
            QMessageBox::critical(this, tr("Invalid IP address"), tr("Invalid IPv4 address or subnet mask."));
            break;
        }


        QString inputVpnName=ui->lineEdit->text();
        QString inputDescription=ui->plainTextEdit->toPlainText();
        QString inputMask=ui->lineEdit_3->text();
        QSqlQuery checkVpnName;
        checkVpnName.prepare(QString("select vpnName, vpnAddressRange, vlanRange, description from VPNs where UPPER(vpnName)=UPPER('%1')").arg(inputVpnName));
        ///this query checks whether the name is already taken, case insensitive
        ///this is redundant if the databse uses UNIQUE ids, maybe I'll remove it

        if(checkVpnName.exec())
        {


            if (checkVpnName.next())
            {
                QMessageBox::critical(this, tr("Name used!"), tr("Cannot create VPN: name is alreay in use."));
                qDebug()<<Q(QString("VPN name %1 is already in use (%2, %3, %4 ,\"%5\").").arg(inputVpnName,checkVpnName.value(0).toString(),checkVpnName.value(1).toString(),checkVpnName.value(2).toString(),checkVpnName.value(3).toString()));
                break;
            }

            qDebug()<<Q(QString("VPN Name %1 available").arg(inputVpnName));
            QSqlQuery newVpnQry;
            QSqlQuery getNextVpnId;

            getNextVpnId.exec("select nextId from nextId where idName='vpnId'");

            bool exists=getNextVpnId.first();
            unsigned int nextVpnId;
            QString updateVpnId;

            if(exists)
            {
                    nextVpnId=getNextVpnId.value(0).toUInt();
                    updateVpnId=QString("update nextId set nextId=%2 Where idName='vpnId'").arg(nextVpnId+1);
            }

            else

            {
                    nextVpnId=0;
                    updateVpnId=QString("insert into nextId values('vpnId',1)");
            }
            //si l'entrée vpnId pour nextId n'existe pas dans la base de données (base de données vide)
            //on le crée au moment d'ajouter le nouveau VPN


            rangeStart=qstringAddressToUint(networkIp);
            rangeEnd=qstringAddressToUint(broadcastIp);

            if(!vpnIpAvailable(rangeStart,rangeEnd))            ///check if the address range is not in use
            {
                QMessageBox::critical(this, tr("Address range in use!"), tr("address range used by another VPN"));
                qDebug()<<"VPN address range in use, view the above list for conflicting VPNs.";
                break;
            }

            qDebug()<<Q(QString("VPN address range %1/%2 available.").arg(inputIpAddress,inputMask));


            if(vlanStart>vlanEnd)                       ///check for valid VPN range entered
            {
                QMessageBox::critical(this, tr("Invalid VLAN range!"), tr("Invalid VLAN range"));
                qDebug()<<Q(QString("Invalid VLAN range entered"));
                break;
            }


            if(!vpnVlanAvailable(vlanStart,vlanEnd))         ///check if the vpn Range is available
            {
                QMessageBox::critical(this, tr("VLAN range in use!"), tr("VLAN range used by another VPN"));
                qDebug()<<Q(QString("VPN VLAN range in use, view the above list for conflicting VPNs."));
                break;
            }


            qDebug()<<"VPN address range available";



            bool ok=getNextVpnId.exec(updateVpnId);         ///update the nextVpnId table
            qDebug()<<"Updating database.";


            if (ok)                                         ///if successful update the vpnIpIntervals table
            {
                QString rangeQryText=QString("insert into vpnIpIntervals values(%1,%2,%3)").arg(rangeStart).arg(rangeEnd).arg(nextVpnId);
                ok=getNextVpnId.exec(rangeQryText);
            }

            if (ok)                                          ///if successful update the vpnVlanRanges table
            {
                //qDebug()<<"rangeintervals updated";
                QString vlanQryText=QString("insert into vpnVlanRanges values(%1,%2,%3)").arg(vlanStart).arg(vlanEnd).arg(nextVpnId);
                ok=getNextVpnId.exec(vlanQryText);

            }

            if (ok)                                          ///isert values into VPNs table
            {
                //qDebug()<<"VLANintervals updated";
                QString qryText=QString("insert into VPNs values(%1,'%2','%3','%4/%5','%6-%7')").arg(nextVpnId).arg(inputVpnName).arg(inputDescription).arg(inputIpAddress).arg(inputMask).arg(vlanStart).arg(vlanEnd);
                newVpnQry.prepare(qryText);
                ok=newVpnQry.exec();

            }

            if(ok)                                          ///successfully added VPN message
            {
                addrecent(nextVpnId);
                qDebug()<<Q(QString("Successfully added VPN %1 (address range: %2/%3, VLAN range: %4-%5) with description: \"%6\".").arg(inputVpnName).arg(inputIpAddress).arg(inputMask).arg(vlanStart).arg(vlanEnd).arg(inputDescription));
                QMessageBox::information(this, tr("VPN added"), tr("VPN added!"));
                emit addedVpn();
                this->close();
                break;
            }


                                        ///if (!ok) display the error message
            QString errorMessage="VPN not added: "+QString(newVpnQry.lastError().databaseText())+QString(getNextVpnId.lastError().databaseText());
            QMessageBox::critical(this, tr("VPN not added"), errorMessage);
            qDebug()<<errorMessage;
            break;

            //to implement:
            //not successful?-->rollback changes //db.exec("delete from VPNs where vpnId=%1"); //etc

        }

                        ///if couldn't check vpn name availability
        QMessageBox::critical(this, tr("Error"), tr("Couldn't check VPN availability, check database connection."));
        break;
   }
}
 */


void addVpnDialog::on_pushButton_clicked()
{
    this->close();
}





//this next part handles the subnet calculations
void addVpnDialog::on_lineEdit_2_editingFinished()
{
    doSubnetUpdate();
}

void addVpnDialog::on_lineEdit_2_textChanged(const QString &arg1)
{
    doSubnetUpdate();
}


void addVpnDialog::on_lineEdit_3_editingFinished()
{
    doSubnetUpdate();
}



void addVpnDialog::on_lineEdit_3_textChanged(const QString &arg1)
{
    doSubnetUpdate();
}




void addVpnDialog::doSubnetUpdate()
{
    IpCalculator calculator(ui->lineEdit_2->text()+'/'+ui->lineEdit_3->text());

    networkIp=calculator.getNetwork();
    broadcastIp=calculator.getBroadcast();


    ui->label_12->setText(networkIp);
    ui->label_15->setText(broadcastIp);

    ui->label_16->setText(calculator.getHostsPerNetwork());
    ui->label_17->setText(calculator.getMaxHost());
    ui->label_14->setText(calculator.getMinHost());
    ui->label_13->setText(calculator.getSubnetMask());


    /*QString networkClass = calculator.getNetworkClass();

    if (networkClass == IpCalculator::classUndefined) {
        ui->labelInformation->setText(QString("Undefined Class"));
    } else {
        QString classInformation = QString("Class %1 Network")
            .arg(calculator.getNetworkClass());
        ui->labelInformation->setText(classInformation);
    }*/
}


//displays error messages for the next par
/*void displayMessage(int code, QString msg)
{
    switch ( code ) {
    case 0:
      //used name
      QMessageBox::critical(this, tr("Name used!"), tr("Cannot create VPN: name is alreay in use."));
      break;

    case 1:
        //used range
        QMessageBox::critical(this, tr("Address range in use!"), tr("address range used by another VPN"));
        break;

    case 2:
        //invalidVlan
        QMessageBox::critical(this, tr("Invalid VLAN range!"), tr("Invalid VLAN range"));
        break;

     case 3:
        //usedVlan
        QMessageBox::critical(this, tr("VLAN range in use!"), tr("VLAN range used by another VPN"));
        break;

     case 4:
        //vpnNotAdded
        QMessageBox::critical(this, tr("VPN not added"), msg);
        break;

    case 5:
        //connectionUnavailable
        QMessageBox::critical(this, tr("Error"), tr("Couldn't check VPN availability, check database connection."));
        break;
    }



}

*/
