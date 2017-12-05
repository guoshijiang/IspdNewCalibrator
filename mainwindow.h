#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Windows.h>
#pragma comment(lib,"advapi32.lib")

#include<tchar.h>
#include <QSettings>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QFrame>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QFont>
#include <QDebug>
#include <QMap>
#include <QAction>
#include <QByteArray>
#include "version.h"
#include "question.h"
#include "document.h"
#include "serialportset.h"
#include "encrypt.h"
#include <QToolBox>
#include "baseui.hpp"
#include "serialport.h"
#include <QTimer>

using namespace InterfaceUI;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void start();
    QString getcomm(int index, QString keyorvalue);
    inline QFont font()
    {
        QFont m_font("宋体", 18, 75);
        return m_font;
    }
    void addAutoRecordUi();
public slots:
    void VersionSlot();
    void QuestionSlot();
    void DocSlot();
    void SerialPortSlot();
    void TogetherTestSlot();
    void RecordSlot();
    void CheckComSlot();
    void CloseApp();
    void m_ledit_textChanged(QString text);
private:
    Ui::MainWindow *ui;
    Version *m_version;
    Question *m_question;
    Document *m_doc;
    SerialPortSet *m_port_set;
    Encrypt *m_encrypt;
    InterfaceUI::BaseUI *m_baseui;
    InterfaceUI::AbstractFactory *m_factory;

    QVBoxLayout *m_vbox_layout;
    QVBoxLayout *m_main_layout;

    QPushButton *m_add_but;

    QWidget *m_page;
    QGridLayout *m_layout;
    QLineEdit *m_ledit;
    QLabel *m_label;
    QPushButton *m_but;
    SerialPort *m_serial_port;

    QList<QLineEdit*> m_list;
    QVector<QString> m_vec;

public:
    HKEY hKey;
    LPCWSTR subkey;
    wchar_t keyname[256]; //键名数组
    char keyvalue[256];  //键值数组
    DWORD keysize,type,valuesize;
    int indexnum;
};

#endif // MAINWINDOW_H
