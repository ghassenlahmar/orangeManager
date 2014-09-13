#include "login.h"
#include "ui_login.h"
#include "iostream"
#include "QString"
#include "mainwindow.h"



login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    //db.setHostName("bigblue");


    db.setDatabaseName("c:\\users\\ghoss\\desktop\\stageorange\\test.sqlite");

    //db.setUserName("acarlson");
    //db.setPassword("1uTbSbAs");

    bool ok = db.open();

    if(ok){qDebug()<<"Connected to database!";}
    else  {qDebug()<<"Database connection failed!";}

}

login::~login()
{
    delete ui;
}

void login::on_pushButton_clicked()
{
    QString inputUsername= ui->lineEdit->text();
    QString inputPassword= ui->lineEdit_2->text();

    QSqlQuery qry;
    if(qry.exec("SELECT* from users where username='"+inputUsername+"'and password='"+inputPassword+"'"))

    {
    found=false;    
    while((qry.next())&&(!found)){found=true;}
    }

    if(found){
        username=inputUsername;
        QMessageBox::information(this, tr("Login Successful"), tr("Login successful!"));

        qDebug()<<"user "<<username<<" authentified successfully!";

        connect(this, SIGNAL(startEditor()),&editor, SLOT(windowstarted()));

        emit startEditor();
        editor.showMaximized();
        this->hide();
    }
    else{
        QMessageBox::critical(this, tr("Login error"), tr("Wrong username or password."));
        qDebug()<<"authentication failed!";
    }
}


