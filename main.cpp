#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QTime>



#define Q(string) (string).toStdString().c_str()
#define timeDebug(string) (Q(time->currentTime().toString()+" - "+string))

                ///debug pane code, code from stackoverflow

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QTime* time= new QTime;

    if(mainwindow::s_textEdit == 0)
    {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
            abort();
        }
    }
    else
    {
        switch (type) {
        case QtDebugMsg:
        case QtWarningMsg:
        case QtCriticalMsg:
            // redundant check, could be removed, or the
            // upper if statement could be removed
            if(mainwindow::s_textEdit != 0)



                mainwindow::s_textEdit->append(timeDebug(msg));
                //mainwindow::s_textEdit->append(msg);
            break;
        case QtFatalMsg:
            abort();
        }
    }
}




int main(int argc, char *argv[])
{

    qInstallMessageHandler(myMessageOutput);    ///debug pane snippet
    QApplication a(argc, argv);
    qDebug()<<"launching";

    loginDialog w;
    w.show();    


    return a.exec();
}
