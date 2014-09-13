#include "databaseoperations.h"

///just declaring a few functions that will be used in the rest of the program


databaseOperations::databaseOperations()
{
}


void addrecent(unsigned int vpnId)           ///this function checks if a VPN is in the recent list
{                                            ///of the current user, and if not, adds it.
    //qDebug()<<"called add Recent";
    while(true)
    {

        unsigned int extern connectedUserId;

        QString actionQryText=QString("select * from recentVpns where (userId=%1)AND(vpnId=%2)").arg(connectedUserId).arg(vpnId);
        QSqlQuery actionQry;
        actionQry.prepare(actionQryText);
        actionQry.exec();
        //qDebug()<<"connected userId"<<connectedUserId;

        if(actionQry.next())
        {
            break;
            ///already in recent
        }


        ///getting the recentRank, if it doesn't exist create it
        ///I chose to rank the recent VPNs
        ///I set a maximum of 10 recent VPNs
        ///when adding a new one, the oldest is "no longer recent" thus, deleted
        int recentRank;

        actionQryText=QString("select recentRank from recentRankCounter where userId=%1").arg(connectedUserId);
        actionQry.exec(actionQryText);

        if(actionQry.next())

        {

            recentRank=(actionQry.value(0).toInt());
            int nextRank;

            if (recentRank==9)
            {
                nextRank=0;
            }

            else
            {
                nextRank=recentRank+1;
            }

            actionQryText=QString("update recentRankCounter set recentRank=%2 Where recentRank=%1").arg(recentRank).arg(nextRank);
        }

        else
        {
            recentRank=0;
            actionQryText=QString("insert into recentRankCounter(userId,recentRank) values(%1,1)").arg(connectedUserId);
        }

        actionQry.exec(actionQryText);

        actionQryText=QString("delete from recentVpns where recentRank=%1").arg(recentRank);
        actionQry.exec(actionQryText);

        actionQryText=QString("insert into recentVpns(userId,vpnId,recentRank) values(%1,%2,%3)").arg(connectedUserId).arg(vpnId).arg(recentRank);
        actionQry.exec(actionQryText);

        break;

    }
}


/*bool ipValidator(QString inputIp)          ///this function checks for empty or missing fields in the address field
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
}*/
