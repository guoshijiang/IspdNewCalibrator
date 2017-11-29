#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <Windows.h>
#pragma comment(lib,"advapi32.lib")

#include<tchar.h>
#include <QSettings>
#include <QLineEdit>
#include "version.h"
#include "question.h"
#include "document.h"
#include "serialportset.h"
#include <QToolBox>
#include "baseui.hpp"

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
public slots:
    void VersionSlot();
    void QuestionSlot();
    void DocSlot();
    void SerialPortSlot();
    void TogetherTestSlot();
    void RecordSlot();
    void CheckComSlot();
    void CloseApp();
private:
    Ui::MainWindow *ui;
    Version *m_version;
    Question *m_question;
    Document *m_doc;
    SerialPortSet *m_port_set;
    InterfaceUI::BaseUI *m_baseui;
    InterfaceUI::AbstractFactory *m_factory;

public:
    HKEY hKey;
    LPCWSTR subkey;
    wchar_t keyname[256]; //键名数组
    char keyvalue[256];  //键值数组
    DWORD keysize,type,valuesize;
    int indexnum;

};

#endif // MAINWINDOW_H
