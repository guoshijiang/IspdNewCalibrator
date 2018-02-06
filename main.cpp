#include "mainwindow.h"
#include <String>
#include <QApplication>
#include <iostream>
#include <QString>
#include <QTextStream>
#include <QDateTime>
#include <QDate>
#include <QFile>
#include <QDir>
#include <QMessageLogContext>
#include <QMutex>
#include <QMutexLocker>
#include <QTextCodec>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define NAME_LEN 100

QFile* global_log_file = NULL;

const int MAX_BUF_LEN = 255;

static void logMessageHandler(QtMsgType type, const QMessageLogContext &msgContext, const QString & msg)
{
    QString txt;
    QDateTime date;
    txt.append(date.currentDateTime().toString()).append("[");
    switch (type)
    {
        case QtDebugMsg:
            txt.append("DEBUG").append("]");
            break;
        case QtInfoMsg:
            txt.append("INFO").append("]");
            break;
        case QtWarningMsg:
            txt.append("WARNING").append("]");
            break;
        case QtCriticalMsg:
            txt.append("CRITICAL").append("]");
            break;
        case QtFatalMsg:
            txt.append("FATAL").append("]");
            abort();
    }
    static QMutex mutex;
    QMutexLocker lock(&mutex);
    QTextStream Output(global_log_file);
    txt += msg;
    Output << txt << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString logFileName = QDir::currentPath();
    int mid = logFileName.lastIndexOf("/");
    logFileName = logFileName.left(mid);
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy-MM-dd_hh_mm_ss");
    logFileName.append("/log/");
    logFileName.append(time_str);
    logFileName.append(QString(".log"));
    global_log_file = new QFile(logFileName);
    if (!global_log_file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        delete global_log_file;
        global_log_file = NULL;
    }
    //qInstallMessageHandler(logMessageHandler);

    MainWindow win;
    win.start();

    return a.exec();
}

