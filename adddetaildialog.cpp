#include "adddetaildialog.h"
#include "ui_adddetaildialog.h"
#include <QHostAddress>
#define Q(string) (string).toStdString().c_str()

extern void addrecent(unsigned int vpnId);
extern unsigned int qstringAddressToUint(QString inputAddress);


addDetailDialog::addDetailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addDetailDialog)
{
    ui->setupUi(this);

}

addDetailDialog::~addDetailDialog()
{
    delete ui;
}


void addDetailDialog::setInterfaceId(unsigned int inputInterfaceId)
{
    interfaceId=inputInterfaceId;
}


void addDetailDialog::setRangeStart(QString inputRangeStart)
{
    rangeStart=qstringAddressToUint(inputRangeStart);
    ui->label_4->setText(inputRangeStart);
}


void addDetailDialog::setRangeEnd(QString inputRangeEnd)
{
    rangeEnd=qstringAddressToUint(inputRangeEnd);
    ui->label_2->setText(inputRangeEnd);
}


void addDetailDialog::on_pushButton_4_clicked()             //ok button
{
    while(true)
    {

        QString detailAddress=ui->lineEdit_7->text();             //reading user inputs
        int localPort=ui->lineEdit_8->text().toInt();
        QString comment=ui->lineEdit_9->text();

        QSqlQuery newDetailQry;                                                   //get the nextId
        QSqlQuery getNextDetailId;
        getNextDetailId.exec("select nextId from nextId where idName='detailId'");

        bool exists=getNextDetailId.first();
        //qDebug()<<"exists="<<exists;

        unsigned int nextDetailId;
        QString updateDetailId;

        if(exists)
        {
            nextDetailId=getNextDetailId.value(0).toUInt();
            updateDetailId=QString("update nextId set nextId=%2 Where idName='detailId'").arg(nextDetailId+1);
        }
        else
        {
            nextDetailId=0;
            updateDetailId=QString("insert into nextId values('detailId',1)");
        }
        //si l'entrée siteId pour nextId n'existe pas dans la base de données (base de données vide)
        //on le crée au moment d'ajouter le nouveau SITE

        QString qryText=QString("insert into details(interfaceId,detailId,detailAddress,localPort,comment) values(%1,%2,'%3',%4,'%5')").arg(interfaceId).arg(nextDetailId).arg(detailAddress).arg(localPort).arg(comment);
        newDetailQry.prepare(qryText);

        unsigned int addressBuffer=qstringAddressToUint(detailAddress);
        bool ok=((addressBuffer>=rangeStart)&&(addressBuffer<=rangeEnd));

        if (ok)
        {
            ok=getNextDetailId.exec(updateDetailId);
        }

        else
        {
            QString errorMessage="Address not usable by interface";
            QMessageBox::critical(this, tr("Detail not added"), errorMessage);
            break;
        }

        if(ok)
        {
            getNextDetailId.exec(QString("select * from details where interfaceId=%1 AND detailAddress='%2'").arg(interfaceId).arg(detailAddress));
            ok=!getNextDetailId.next();
        }


        if(ok)
        {
            ok=newDetailQry.exec();

            if(ok)
            {
                QMessageBox::information(this, tr("detail added"), tr("detail added!"));
                addrecent(vpnId);
                emit addedDetail();


                //qDebug()<< Q(time.currentTime().toString()+" - "+ QString("Added detail(%1, %2, \"%3\") to interface x in VPN xx").arg(detailAddress).arg(localPort).arg(comment));
                qDebug()<<Q(QString("Successfully added detail(%1, %2, \"%3\") to interface %4 in VPN %5.").arg(detailAddress).arg(localPort).arg(comment).arg(interfaceName).arg(vpnName));

                this->close();
            }
            else
            {
                QString errorMessage="Detail not added. SQL error: "+QString(newDetailQry.lastError().databaseText());
                QMessageBox::critical(this, tr("Detail not added"), errorMessage);
                qDebug()<<errorMessage;
            }

        }

        else
        {
            QString errorMessage="Detail address already used.";
            QMessageBox::critical(this, tr("Detail not added"), errorMessage);
        }

        break;
    }
}

/*

void addDetailDialog::on_pushButton_4_clicked()             //ok button
{

    QString detailAddress=ui->lineEdit_7->text();             //reading user inputs
    int localPort=ui->lineEdit_8->text().toInt();
    QString comment=ui->lineEdit_9->text();

    QSqlQuery newDetailQry;                                                   //get the nextId
    QSqlQuery getNextDetailId;
    getNextDetailId.exec("select nextId from nextId where idName='detailId'");

    bool exists=getNextDetailId.first();
    //qDebug()<<"exists="<<exists;

    unsigned int nextDetailId;
    QString updateDetailId;

    if(exists){nextDetailId=getNextDetailId.value(0).toUInt();
        updateDetailId=QString("update nextId set nextId=%2 Where idName='detailId'").arg(nextDetailId+1);}
    else{nextDetailId=0;updateDetailId=QString("insert into nextId values('detailId',1)");}
    //si l'entrée siteId pour nextId n'existe pas dans la base de données (base de données vide)
    //on le crée au moment d'ajouter le nouveau SITE

    QString qryText=QString("insert into details(interfaceId,detailId,detailAddress,localPort,comment) values(%1,%2,'%3',%4,'%5')").arg(interfaceId).arg(nextDetailId).arg(detailAddress).arg(localPort).arg(comment);
    newDetailQry.prepare(qryText);

    unsigned int addressBuffer=qstringAddressToUint(detailAddress);
    bool ok=((addressBuffer>=rangeStart)&&(addressBuffer<=rangeEnd));

    if (ok){
    ok=getNextDetailId.exec(updateDetailId);
    }

    else{QString errorMessage="Address not usable by interface";
    QMessageBox::critical(this, tr("Detail not added"), errorMessage);}

    if(ok){
    ok=newDetailQry.exec();

    if(ok)
    {
        QMessageBox::information(this, tr("detail added"), tr("detail added!"));
        addrecent(vpnId);
        emit addedDetail();

        QTime time;
        time.currentTime();

        //qDebug()<< Q(time.currentTime().toString()+" - "+ QString("Added detail(%1, %2, \"%3\") to interface x in VPN xx").arg(detailAddress).arg(localPort).arg(comment));
        qDebug()<<Q(QString("Successfully added detail(%1, %2, \"%3\") to interface %4 in VPN %5.").arg(detailAddress).arg(localPort).arg(comment).arg(interfaceName).arg(vpnName));

        this->close();
    }
    else
    {
        QString errorMessage="Detail not added. SQL error: "+QString(newDetailQry.lastError().databaseText());
        QMessageBox::critical(this, tr("Detail not added"), errorMessage);
        qDebug()<<errorMessage;
    }

    }
    }

  */



void addDetailDialog::on_pushButton_3_clicked()  //cancel button
{
    this->close();
}

void addDetailDialog::setVpnId(unsigned int inputVpnId)
{
    vpnId=inputVpnId;
    //qDebug()<<"vpnId:setvpnId "<<vpnId;
}

void addDetailDialog::setInterfaceName(QString inputInterfaceName)
{
    interfaceName=inputInterfaceName;
    //qDebug()<<"vpnId:setvpnId "<<vpnId;
}

void addDetailDialog::setVpnName(QString inputVpnName)
{
    vpnName=inputVpnName;
    //qDebug()<<"vpnId:setvpnId "<<vpnId;
}
