#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addinterfacedialog.h"
#include "vpntab.h"
#include "customtabwidget.h"
#include "addvpndialog.h"

#define Q(string) (string).toStdString().c_str()

QTextEdit * mainwindow::s_textEdit = 0;

//this is the main window of the program
//the one just after the login screen
//it is composed of a VPN list/viewer with tabs
//a VPN group selection on the left pane (favorites/recent etc...)
//and a sort of debug console or command history below


unsigned int connectedUserId;

mainwindow::mainwindow(QWidget *parent) :       ///just a few settings and initializations
    QMainWindow(parent),
    ui(new Ui::mainwindow)
{

    QStandardItemModel* vpnListModel = new QStandardItemModel();

    ui->setupUi(this);

    ui->tabWidget->setTabsClosable(true);

    //ui->tabWidget->setMovable(true);                              disabled: causes the ALL VPNs tab to be closable
    //I want the "all VPNS" tab to remain

    ui->tabWidget->setTabText(0,"ALL VPNs");
    ui->listView_2->setModel(vpnListModel);
    ui->listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStandardItem *allVpnGroup = new QStandardItem(QString("ALL VPNs"));
    vpnListModel->appendRow(allVpnGroup);
    QStandardItem *recentGroup = new QStandardItem(QString("Recent"));
    vpnListModel->appendRow(recentGroup);

    connect(&addVpn, SIGNAL(addedVpn()),this, SLOT(refreshVpn()));
    connect(&del, SIGNAL(deleted()),this, SLOT(refreshVpn()));
    connect(&del, SIGNAL(deletedGroup()),this, SLOT(refreshVpnGroups()));
    connect(&addG,SIGNAL(addedVpnGroup()),this, SLOT(refreshVpnGroups()));
    ui->tabWidget->removeTab(1);


    s_textEdit = new QTextEdit; //debug pane, code from stackoverflow


    QFont font;
    font.setFamily("Courier new");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);     ///setting a monospace font to display
    //font.setBold(true);
    //font.setWeight(20);
    s_textEdit->setFont(font);                           ///formatted elements correctly

    ui->tabWidget_2->removeTab(1);
    ui->tabWidget_2->removeTab(0);
    ui->tabWidget_2->addTab(s_textEdit, "Activity log");

}
mainwindow::~mainwindow()
{
    delete ui;
}


void mainwindow::on_tabWidget_tabCloseRequested(int index)   ///closes any tab except the "all VPNs" tab after clicking on the close tab "x" symbol
{

    if (index!=0)
    {
        ui->tabWidget->removeTab(index);
    }

    //user can't close the all vpns tab, but if the tab is set to display a VPN group or a search query
    //clicking on the close button will revert to showing all vpns
    else
    {
        if(ui->tabWidget->tabText(0) != "ALL VPNs")
        {
            on_pushButton_5_clicked();                  //cancels filtering/showing groups
        }
    }
}



                                      ///initializes the window and loads the VPN list after login
void mainwindow::windowstarted()      ///this is done after login because some fucntions require databse connection
{


    vpnModel = new QSqlQueryModel;
    vpnModel->setQuery("SELECT * FROM VPNs");
    vpnModel->setHeaderData(0, Qt::Horizontal, tr("VPN id"));
    vpnModel->setHeaderData(1, Qt::Horizontal, tr("VPN name"));
    vpnModel->setHeaderData(2, Qt::Horizontal, tr("Description"));
    vpnModel->setHeaderData(3, Qt::Horizontal, tr("Address Range"));
    vpnModel->setHeaderData(4, Qt::Horizontal, tr("VLAN range"));


    ui->tableView->setModel(vpnModel);
    ui->tableView->setColumnHidden(0,1);
    ui->tableView->horizontalHeader()->setDefaultSectionSize(120);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    groupModel = new QSqlQueryModel;
    groupModel->setQuery(QString("select userId,groupId,groupName from vpnGroups where userId=%1").arg(connectedUserId));



    groupModel->setHeaderData(0, Qt::Horizontal, tr("user id"));
    groupModel->setHeaderData(1, Qt::Horizontal, tr("group id"));
    groupModel->setHeaderData(2, Qt::Horizontal, tr("group name"));

    ui->listView->setModel(groupModel);
    ui->listView->setModelColumn(2);
    //ui->columnView_2->set
    ui->checkBox->setChecked(1);
    ui->checkBox_2->setChecked(0);

}



void mainwindow::on_tableView_doubleClicked(const QModelIndex &index)   ///doubleclicking on a VPN
{
    //creates a vpn TAB when double-clicking on a VPN from the list

    vpnId=vpnModel->data(vpnModel->index(index.row(),0)).toUInt();
    //qDebug()<<"vpnId="<<vpnId;

    vpnName=vpnModel->data(vpnModel->index(index.row(),1)).toString();
    QString vpnDescription=vpnModel->data(vpnModel->index(index.row(),2)).toString();
    QString vpnAddressRange=vpnModel->data(vpnModel->index(index.row(),3)).toString();
    QString vpnVlanRange=vpnModel->data(vpnModel->index(index.row(),4)).toString();

    //this is the VPN information that will be displayed in the new tab
    //I'd rather pass it this way than send a new SQL query

    int createdTabIndex=ui->tabWidget->addTab(new vpnTab(vpnName,vpnDescription,vpnAddressRange,vpnVlanRange,vpnId),vpnName);    

    vpnTab* vpnTabPointer= qobject_cast<vpnTab*>(ui->tabWidget->widget(createdTabIndex));    ///vpnTabPointer, will be used to access values from vpnTab
    vpnTabPointer->setPointer(vpnTabPointer);


    //qDebug()<<"created tab index="<<createdTabIndex;
    ui->tabWidget->setCurrentIndex(createdTabIndex);
    qDebug()<<Q(QString("Displaying VPN %1.").arg(vpnName));
}



void mainwindow::on_pushButton_9_clicked()    ///clicking on the "add VPN" button
{

    addVpn.show();

}


void mainwindow::refreshVpn()      ///slot to refreshe the VPN list
{


    //this slot will be called when clicking on the refresh button or after a new VPN is added
    QString qry=vpnModel->query().lastQuery();
    vpnModel->setQuery(qry);
    //qDebug()<<"VPN list refreshed";

}



void mainwindow::on_pushButton_6_clicked()   ///refresh button, calls the above signal
{

    refreshVpn();
}



void mainwindow::on_pushButton_7_clicked()      ///edit VPN button
{

}


void mainwindow::on_pushButton_4_clicked()      ///search button
{
    QString searchVpn=ui->lineEdit->text();
    QString searchVpnQry;

    if((ui->tabWidget->tabText(0) != "ALL VPNs")&&(ui->tabWidget->tabText(0) != "Recent VPNs"))
    {
        searchVpnQry=vpnModel->query().lastQuery() + QString(" AND vpns.vpnName like '%1%'").arg(searchVpn);
    }

    else
    {
        searchVpnQry=vpnModel->query().lastQuery() + QString(" where vpns.vpnName like '%1%'").arg(searchVpn);
    }

    vpnModel->setQuery(searchVpnQry);
    ui->tabWidget->setTabText(0,QString("filter: %1").arg(searchVpn));

    qDebug()<<Q(QString("Displaying VPNs starting with %1.").arg(searchVpn));
}



                                                ///delete VPN button
void mainwindow::on_pushButton_8_clicked()      ///prompts a confirmation message of type confirmdeletedialog
{    
    del.settings("Vpn",vpnId,vpnName);
    del.show();
}




void mainwindow::on_tableView_clicked(const QModelIndex &index)         ///clicking on a VPN
{
    //whenever the table is clicked, the corresponding VPN ID and name are cached
    //this cached information will be used when clicking on the add or delete button next
    //this is pretty much selecting

    vpnId=vpnModel->data(vpnModel->index(index.row(),0)).toUInt();
    //qDebug()<<"vpnId="<<vpnId;
    vpnName=vpnModel->data(vpnModel->index(index.row(),1)).toString();
}


void mainwindow::on_pushButton_5_clicked()      ///cancel search button
{
    //when you cancel the search/filtering
    vpnModel->setQuery("SELECT * FROM VPNs");
    ui->tabWidget->setTabText(0,"ALL VPNs");
    //qDebug()<<"filtering cancelled";
}


void mainwindow::setConnectedUser(QString inputUsername)       ///sets the connected userId, called by login
{
    //this gets the username of the connected user after login
    //I haven't used this for anything yet
    connectedUsername=inputUsername;
}

void mainwindow::setConnectedUserId(unsigned int inputUserId)  ///sets the connected userName, called by login
{
    connectedUserId=inputUserId;

}

void mainwindow::on_pushButton_3_clicked()       ///new vpn group button
{
    addG.setUserId(connectedUserId);
    addG.show();
}

void mainwindow::on_pushButton_2_clicked()       ///edit VPN group button
{

}

void mainwindow::on_pushButton_clicked()         ///delete VPN group button
{
    if (selectedVpnGroupOwner==connectedUserId)
    {
        del.settings("VpnGroup",vpnGroupId, selectedVpnGroupName);
        del.show();
    }

    else
    {
        QMessageBox::critical(this, tr("Denied"), tr("You are not the owner of this group."));
    }


}


void mainwindow::refreshVpnGroups()              ///slot to refresh the VPN groups list
{
    QString qry=groupModel->query().lastQuery();
    groupModel->setQuery(qry);
    //qDebug()<<"VPN groups list refreshed";
}


void mainwindow::on_listView_clicked(const QModelIndex &index)  ///gets the VPN group info when clicking on list
{
    vpnGroupId=groupModel->data(groupModel->index(index.row(),1)).toUInt();
    //qDebug()<<"vpnGroupId="<<vpnGroupId;
    //vpnGroupName=vpnModel->data(vpnModel->index(index.row(),0)).toString();
    selectedVpnGroupOwner=groupModel->data(groupModel->index(index.row(),0)).toUInt();
    //qDebug()<<"selectedVpnGroupOwner"<<selectedVpnGroupOwner;
    selectedVpnGroupName=groupModel->data(groupModel->index(index.row(),2)).toString();
    //qDebug()<<"selectedVpnGroupName"<<selectedVpnGroupName;
}

void mainwindow::on_pushButton_10_clicked()      ///add VPN to group button
{    
    while(true)
    {

        if(selectedVpnGroupOwner!=connectedUserId)
        {
            QMessageBox::critical(this, tr("Error"), tr("You cannot edit this VPN group."));
            qDebug()<<"Error: cannot add VPN to group because you are not the owner of this VPN group.";
            break;
        }

        QString addVpnToGroupText=QString("select * from vpnGroupContents where (groupId=%1)AND(vpnId=%2)").arg(vpnGroupId).arg(vpnId);
        QSqlQuery addVpnToGroup;
        addVpnToGroup.exec(addVpnToGroupText);

        if(addVpnToGroup.next())
        {
            QMessageBox::critical(this, tr("Error"), tr("VPN already in group"));
            break;
        }


        addVpnToGroupText=QString("insert into vpnGroupContents(groupId,vpnId) values(%1,%2)").arg(vpnGroupId).arg(vpnId);

        if (addVpnToGroup.exec(addVpnToGroupText))
        {
            QMessageBox::information(this, tr("VPN added to group"), tr("VPN added to group!"));
            qDebug()<<Q(QString("Successfully added vpn %1 to group %2.").arg(vpnName).arg(selectedVpnGroupName));
            break;
        }

        QMessageBox::critical(this, tr("Error"), addVpnToGroup.lastError().databaseText());
        break;
    }    
}


void mainwindow::on_listView_doubleClicked(const QModelIndex &index) ///doubleclicking ON VPN group, displays group
{
    vpnGroupId=groupModel->data(groupModel->index(index.row(),1)).toUInt();
    //qDebug()<<"vpnGroupId="<<vpnGroupId;
    //vpnGroupName=vpnModel->data(vpnModel->index(index.row(),0)).toString();
    selectedVpnGroupOwner=groupModel->data(groupModel->index(index.row(),0)).toUInt();
    //qDebug()<<"selectedVpnGroupOwner"<<selectedVpnGroupOwner;
    selectedVpnGroupName=groupModel->data(groupModel->index(index.row(),2)).toString();
    //qDebug()<<"selectedVpnGroupName"<<selectedVpnGroupName;


    vpnModel->setQuery(QString("SELECT vpns.vpnId,vpns.vpnName,vpns.description,vpns.vpnAddressRange,vpns.vlanRange from vpns join vpnGroupContents ON vpns.vpnId=vpnGroupContents.vpnId where vpnGroupContents.groupId=%1").arg(vpnGroupId));
    ui->tableView->setColumnHidden(0,1);
    ui->tabWidget->setTabText(0,"group: "+selectedVpnGroupName);
    ui->tabWidget->setCurrentIndex(0);    
    qDebug()<<Q(QString("Displaying VPNs in group %1.").arg(selectedVpnGroupName));

}


void mainwindow::on_listView_2_doubleClicked(const QModelIndex &index)
{
    if (index.row()==0)
    {
        on_pushButton_5_clicked();      //just sets back to ALL VPNs
        ui->tableView->setColumnHidden(0,1);
        qDebug()<<"Displaying all VPNs.";
    }

        else
    {
        vpnModel->setQuery(QString("SELECT vpns.vpnId,vpns.vpnName,vpns.description,vpns.vpnAddressRange,vpns.vlanRange from vpns join recentVpns ON vpns.vpnId=recentVpns.vpnId where recentVpns.userId=%1").arg(connectedUserId));
        ui->tableView->setColumnHidden(0,1);
        ui->tabWidget->setTabText(0,"group: "+selectedVpnGroupName);
        //qDebug()<<"error "<<vpnModel->lastError().databaseText();
        qDebug()<<"Displaying recent VPNs.";

    }

    ui->tabWidget->setCurrentIndex(0);
}


void mainwindow::on_pushButton_11_clicked()         ///remove VPN from group button
{
    while(true)
    {
        if(selectedVpnGroupOwner!=connectedUserId)
        {
            QMessageBox::critical(this, tr("Error"), tr("You cannot edit this VPN group."));
            qDebug()<<"Error: cannot remove VPN from group because you are not the owner of this VPN group.";
            break;
        }

        QString deleteVpnFromGroupText=QString("select * from vpnGroupContents where (groupId=%1)and(vpnId=%2)").arg(vpnGroupId).arg(vpnId);
        QSqlQuery deleteVpnFromGroup;
        deleteVpnFromGroup.exec(deleteVpnFromGroupText);

        if(!deleteVpnFromGroup.next())
        {
            QMessageBox::critical(this, tr("Error"), tr("VPN not in selected group"));
            qDebug()<<"Error: cannot remove VPN from group because VPN is not in selected group";
            break;
        }

        deleteVpnFromGroupText=QString("delete from vpnGroupContents where (groupId=%1)and(vpnId=%2)").arg(vpnGroupId).arg(vpnId);
        if (!deleteVpnFromGroup.exec(deleteVpnFromGroupText))
        {
            QMessageBox::critical(this, tr("Error"), tr("Error: ") + deleteVpnFromGroup.lastError().databaseText());
            break;

        }

        refreshVpn();
        qDebug()<<Q(QString("Successfully removed vpn %1 from group %2.").arg(vpnName).arg(selectedVpnGroupName));
        break;

     }
}




void mainwindow::on_checkBox_2_toggled(bool checked)        ///public groups checkBox
{
    groupsChanged();
}


void mainwindow::on_checkBox_toggled(bool checked)          ///owned groups checkbox
{
    groupsChanged();
}


void mainwindow::groupsChanged()                ///when the public/owned groups checkboxes are toggled
{
    bool publicG, ownedG;
    publicG=ui->checkBox_2->isChecked();
    ownedG=ui->checkBox->isChecked();

    QString linker="";
    QString publicStr="";
    QString ownedStr="";

    if(publicG && ownedG)
    {
        linker="or ";
    }

    if(publicG)
    {
        publicStr="isPublic=1 ";
    }

    if(ownedG)
    {
        ownedStr=QString("userId=%1").arg(connectedUserId);
    }

    groupModel->setQuery(QString("select userId,groupId,groupName from vpnGroups where ")+publicStr+linker+ownedStr);
    ui->listView->setModelColumn(2);
}
