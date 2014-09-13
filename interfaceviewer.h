#ifndef INTERFACEVIEWER_H
#define INTERFACEVIEWER_H

#include <QWidget>
#include <QDebug>
#include <QtSql>
#include <QString>
#include "adddetaildialog.h"
#include "ipcalculator.h"


namespace Ui {
class interfaceViewer;
}

class interfaceViewer : public QWidget
{
    Q_OBJECT

public:
    explicit interfaceViewer(QWidget *parent = 0);
    ~interfaceViewer();
    void set(unsigned int inputInterfaceId, QString inputInterfaceName, QString inputInterfaceAddressRange);
    void setVpnId(unsigned int inputVpnId);
    void setVpnName(QString inputVpnName);

signals:
    void back();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_6_clicked();
    void refreshDetails();
    void on_pushButton_5_clicked();
    void on_tableView_3_clicked(const QModelIndex &index);

private:
    Ui::interfaceViewer *ui;
    unsigned int interfaceId;
    QString interfaceName;
    QString interfaceAddressRange;
    QString vpnName;
    addDetailDialog addDetail;
    QSqlQueryModel *detailsModel;
    unsigned int detailId;
    unsigned int selectedDetailId;
    unsigned int vpnId;
};

#endif // INTERFACEVIEWER_H
