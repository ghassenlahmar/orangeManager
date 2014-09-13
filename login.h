#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QtSql>
#include "mainwindow.h"


namespace Ui {
class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();
    QString username;
    mainwindow editor;
    QSqlDatabase db;

signals:
    void startEditor();



private slots:
    void on_pushButton_clicked();

    
private:
    Ui::login *ui;
    bool found;
    QString connectedUsername;
};

#endif // LOGIN_H
