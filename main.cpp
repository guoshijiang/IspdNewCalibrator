﻿#include "mainwindow.h"
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

/*
void enumPort(void)
{
    HKEY hKey;
    const char* pSource="HARDWARE\\DEVICEMAP\\SERIALCOMM\\";
    LPCTSTR lpSubKey = NULL;
    if(sizeof(TCHAR) == sizeof(char))
    {
        lpSubKey = (LPCTSTR)pSource;
    }
    else
    {
        int length= sizeof(TCHAR)*(strlen(pSource)+1);
        LPTSTR tcBuffer=new TCHAR[length];
        memset(tcBuffer,0,length);
        MultiByteToWideChar(CP_ACP,0,pSource,strlen(pSource),tcBuffer,length);
        lpSubKey=(LPCTSTR)tcBuffer ;
    }

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKey)!= ERROR_SUCCESS)
    {
      return ;
    }
    LPWSTR szValueName = NULL;
    //char szValueName[NAME_LEN];
    BYTE szPortName[NAME_LEN];
    LONG status;
    DWORD dwIndex = 0;
    DWORD dwSizeValueName=100;
    DWORD dwSizeofPortName=100;
    DWORD Type;
    dwSizeValueName = NAME_LEN;
    dwSizeofPortName = NAME_LEN;
    do
    {
      status = RegEnumValue(hKey, dwIndex++, szValueName, &dwSizeValueName, NULL, &Type,
      szPortName, &dwSizeofPortName);
      if((status == ERROR_SUCCESS))
      {
         printf("szPortName=%s\n", szPortName);
      }
      dwSizeValueName = NAME_LEN;
      dwSizeofPortName = NAME_LEN;
    }
    while((status!= ERROR_NO_MORE_ITEMS));
    RegCloseKey(hKey);
}
*/

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

    MainWindow w;
    w.start();

    return a.exec();
}
