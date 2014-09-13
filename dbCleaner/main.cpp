#include <QCoreApplication>

#include <QString>
#include <QDebug>
#include <QtSql>
#include <QString>
#include <stdio.h>
#include <iostream>
using namespace std;

QSqlDatabase db;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\ghoss\\Desktop\\stageorange\\testsql11\\test.sqlite");

    db.open();

    char y;

    cout<<"do you really wish to delete everything in database?(except users) (y/n)?"<<endl;
    cin>>y;

    if(y=='y')
    {

        db.exec("delete from VPNs");
        cout<<"VPNS done!"<<endl;
        db.exec("delete from VLANs");
        cout<<"VLANs done!"<<endl;
        db.exec("delete from sites");
        cout<<"sites done!"<<endl;
        db.exec("delete from interfaces");
        cout<<"interfaces done!"<<endl;
        db.exec("delete from details");
        cout<<"Details done!"<<endl;
        db.exec("delete from nextId");
        cout<<"nextId done!"<<endl;
        db.exec("delete from vpnIpIntervals");
        cout<<"VPN IP Intervals done!"<<endl;
        db.exec("delete from vpnVlanRanges");
        cout<<"VPN vlan ranges done!"<<endl;
        db.exec("delete from interfaceIpIntervals");
        cout<<"VPN vlan ranges done!"<<endl;
    }
    cout<<"All done!"<<endl;
    return 0;
}
