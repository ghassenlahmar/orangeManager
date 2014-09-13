#include "newvpngroupdialog.h"
#include "ui_newvpngroupdialog.h"

#define Q(string) (string).toStdString().c_str()


newVpnGroupDialog::newVpnGroupDialog(QWidget *parent) :             ///initializations
    QDialog(parent),
    ui(new Ui::newVpnGroupDialog)
{
    ui->setupUi(this);
    ui->checkBox->setToolTip(tr("If this option is checked, the vpn group can be seen by all users"));
    ui->checkBox->toolTipDuration();
}
newVpnGroupDialog::~newVpnGroupDialog()
{
    delete ui;
}

void newVpnGroupDialog::setUserId(int inputUserId)                  ///sets the user ID
{
    userId=inputUserId;

}

void newVpnGroupDialog::on_pushButton_clicked()                     ///clicking on the add group button
{
    while(true)
    {

        if (ui->lineEdit->text().isEmpty())
        {
            QMessageBox::critical(this, tr("VPN group not added: "),"No group name entered.");
            break;                          ///no name entered-->exit
        }

        QString vpnGroupName=ui->lineEdit->text();
        QSqlQuery getNextVpnGroupId;            ///get the entered name

        if(!getNextVpnGroupId.exec(QString("select * from vpnGroups where (groupName='%1')AND(userId='%2')").arg(vpnGroupName).arg(userId)))
        {
            qDebug()<<Q("Database problem: "+getNextVpnGroupId.lastError().databaseText());
            break;                              ///check if name exists
        }                                       ///if can't check->database/connection problem


        if (getNextVpnGroupId.next())           ///VPN group name already exists
        {
            QMessageBox::critical(this, tr("VPN group not added: "),"You have already created a group with this name");
            break;
        }

        getNextVpnGroupId.exec("select nextId from nextId where idName='vpnGroupId'");      ///get the nextId from databse

        bool exists=getNextVpnGroupId.first();              ///check if database entry for nextId of vpn groups exists
        unsigned int nextVpnGroupId;
        QString updateVpnGroupId;

        if(exists)
        {
            nextVpnGroupId=getNextVpnGroupId.value(0).toUInt();   ///if it exists take the current id and update the database
            updateVpnGroupId=QString("update nextId set nextId=%2 Where idName='vpnGroupId'").arg(nextVpnGroupId+1);
        }

        else
        {
            nextVpnGroupId=0;                                   ///if it doesn't create it
            updateVpnGroupId=QString("insert into nextId values('vpnGroupId',1)");
        }


        if(getNextVpnGroupId.exec(updateVpnGroupId))            ///execute the above query
        {
            bool isPublic=ui->checkBox->isChecked();            ///if group is set to public
            QString newVpnGroupQuery=QString("insert into vpnGroups(userId,groupId,isPublic,groupName) values(%1,%2,%3,'%4')"
                                            ).arg(userId).arg(nextVpnGroupId).arg(isPublic).arg(vpnGroupName);

            //qDebug()<<"query string: "<<newVpnGroupQuery;

            if(getNextVpnGroupId.exec(newVpnGroupQuery))        ///added successfully
            {
                QMessageBox::information(this, tr("VPN group added"), tr("VPN group added!"));
                emit addedVpnGroup();
                QString publicStr="";

                if(isPublic)
                {
                    publicStr="public ";
                }

                qDebug()<<Q(QString("Successfully added ")+publicStr+QString("group %1").arg(vpnGroupName));
                this->close();
                break;
            }


        }
        QMessageBox::critical(this, tr("VPN group not added: "),"Error :"+getNextVpnGroupId.lastError().databaseText());
        break;                                          ///vpn not added
    }
}



void newVpnGroupDialog::on_pushButton_2_clicked()           ///cancel button
{
    this->close();
}
