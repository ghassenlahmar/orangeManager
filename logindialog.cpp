#include "logindialog.h"
#include "ui_logindialog.h"

#define Q(string) (string).toStdString().c_str()



loginDialog::loginDialog(QWidget *parent) :             ///connects to database
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\ghoss\\Desktop\\stageorange\\testSQL17\\test.sqlite");

    //db.setHostName("xx");
    //db.setinputUsername("xx");       //saw these commands online
    //db.setPassword("xx");            //might be useful for secure authentication
}
loginDialog::~loginDialog()
{
    delete ui;
}


void loginDialog::on_pushButton_clicked()               ///clicking on the connect button
{
    connectionOk = db.open();

    if(!connectionOk)
    {
        //if connection to the database fails
        QMessageBox::critical(this, tr("Login error"), tr("Database connection failed!"));
        qDebug()<<"Database connection failed!";
    }

    else
    {
        //connection successful
        qDebug()<<"Connected to database!";
        QString inputUsername= ui->lineEdit->text();
        QString inputPassword= ui->lineEdit_2->text();
        QString qryString=QString("SELECT userId from users where username='%1' and password='%2'").arg(inputUsername).arg(inputPassword);


        //read the user inputs and prepare a query
        QSqlQuery qry;
        qry.prepare(qryString);


        qry.exec();

                if (qry.next())
            {

                unsigned int userId=qry.value(0).toUInt();

                QMessageBox::information(this, tr("Login Successful"), tr("Login successful!"));
                qDebug()<<Q(QString("User %1 authentified successfully!").arg(inputUsername));

                connect(this, SIGNAL(startEditor()),&editor, SLOT(windowstarted()));
                editor.setConnectedUser(inputUsername);
                editor.setConnectedUserId(userId);
                emit startEditor();
                editor.showMaximized();
                this->hide();
            }
        else
        {
            QMessageBox::critical(this, tr("Login error"), tr("Wrong username or password."));
            qDebug()<<"authentication failed!";
        }
       }
      }


void loginDialog::on_pushButton_2_clicked()             ///cancel button
{
    this->close();
}
