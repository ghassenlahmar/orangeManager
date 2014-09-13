#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QtSql>
#include <QTime>
#include "mainwindow.h"

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

    mainwindow editor;
    QSqlDatabase db;

signals:
    void startEditor();

public:
    explicit loginDialog(QWidget *parent = 0);
    ~loginDialog();
    QTime time;


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::loginDialog *ui;    
    bool connectionOk;
};

#endif // LOGINDIALOG_H
