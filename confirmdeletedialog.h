#ifndef CONFIRMDELETEDIALOG_H
#define CONFIRMDELETEDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>
#include <QMainWindow>



namespace Ui {
class confirmDeleteDialog;
}

class confirmDeleteDialog : public QDialog
{
    Q_OBJECT

public:
    confirmDeleteDialog(QWidget *parent = 0);
    ~confirmDeleteDialog();
    void settings(QString inputElementType,unsigned int inputId,QString elementName);

signals:
    void deleted();
    void deletedGroup();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();


private:
    Ui::confirmDeleteDialog *ui;
    unsigned int id;
    QString elementType;
    QString elementName;

};

#endif // CONFIRMDELETEDIALOG_H
