#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "addinterfacedialog.h"
#include "addvpndialog.h"
#include <QColumnView>
#include <QMainWindow>
#include <QDebug>
#include <QtGui>
#include <QColumnView>
#include <QtSql>
#include <QString>
#include "confirmdeletedialog.h"
#include "newvpngroupdialog.h"
#include "databaseoperations.h"
#include <QTextEdit>

namespace Ui {
class mainwindow;
}

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = 0);
    ~mainwindow();
    void setConnectedUser(QString inputUsername);
    void setConnectedUserId(unsigned int inputUserId);
    QSqlDatabase db;
    //bool newVpn(QString inputVpnName, QString inputDescription);
    unsigned int connectedUserId;

    static QTextEdit * s_textEdit;   //debugging code from http://stackoverflow.com/questions/22485208/redirect-qdebug-to-qtextedit




public slots:
    void windowstarted();
    void refreshVpn();
    void refreshVpnGroups();



private slots:
    void on_pushButton_9_clicked();
    void on_tabWidget_tabCloseRequested(int index);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushButton_7_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_8_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_pushButton_10_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_listView_2_doubleClicked(const QModelIndex &index);

    void on_pushButton_11_clicked();

    void on_checkBox_2_toggled(bool checked);

    void on_checkBox_toggled(bool checked);

    void groupsChanged();

private:
    Ui::mainwindow *ui;
    QColumnView columnview;
    QSqlQueryModel *vpnModel;
    QSqlQueryModel *groupModel;
    addInterfaceDialog addN;
    addVpnDialog addVpn;
    QString vpnName,connectedUsername,selectedVpnGroupName;
    confirmDeleteDialog del;
    unsigned int vpnId;
    unsigned int selectedVpnGroupOwner;
    unsigned int vpnGroupId;

    newVpnGroupDialog addG;
    databaseOperations op;

};

#endif // MAINWINDOW_H
