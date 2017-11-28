#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QDebug>
#include <iostream>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <iostream>
#include <QScrollArea>



#include "serialport.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_version(new Version),
    m_question(new Question), m_doc(new Document),
    m_port_set(new SerialPortSet)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->m_version;
    delete this->m_question;
    delete this->m_doc;
    delete this->m_port_set;
}

void MainWindow::VersionSlot()
{
    this->m_version->ver_start();
}

void MainWindow::QuestionSlot()
{
    this->m_question->qus_start();
}

void MainWindow::DocSlot()
{
    this->m_doc->doc_start();
}

void MainWindow::SerialPortSlot()
{
    this->m_port_set->serPort_start();
}

void MainWindow::TogetherTestSlot()
{
    this->ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::RecordSlot()
{
    this->ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::CheckComSlot()
{
    this->ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::CloseApp()
{
    this->close();
}

QString MainWindow::getcomm(int index, QString keyorvalue)
{
    QString commresult;
    QString keymessage;
    QString message;
    QString valuemessage;
    indexnum = index;

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey) != 0)
    {
        QString error="Cannot open regedit!";
        return error;
    }

    keysize = sizeof(keyname);
    valuesize = sizeof(keyvalue);

    if(RegEnumValue(hKey, indexnum, keyname, &keysize, 0, &type, (BYTE*)keyvalue, &valuesize) == 0)
    {
        for (int i=0; i<(int)keysize; i++)
        {
            message = keyname[i];
            keymessage.append(message);
        }

        for (int j=0; j<(int)valuesize; j++)
        {
             if (keyvalue[j] != 0x00)
             {
                 valuemessage.append(keyvalue[j]);
             }
        }

        if(keyorvalue == "key")
        {
            commresult = keymessage;
        }

        if(keyorvalue == "value")
        {
            commresult=valuemessage;
        }
    }
    else
    {
         commresult = "nokey";
    }
    RegCloseKey(hKey);
    return commresult;
}

void MainWindow::start()
{
    QString path = "HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM";
    QSettings *settings = new QSettings(path, QSettings::NativeFormat);
    QStringList key = settings->allKeys();
    for(int i = 0; i < key.size(); ++i)
    {
        qDebug() << key[i];
    }

    QStringList comlist ;
    comlist.clear();
    for (int i=0; i < key.size(); i++)
    {
       comlist << getcomm(i, "value");
    }

    for(int j = 0; j < comlist.size(); ++j)
    {
        this->ui->comboBox->addItem(comlist[j]);
    }

    qDebug() << "windows:" << comlist;


    m_baseui = new BaseUi();
    m_baseui->BaseScrollArea();
    //this->ui->groupBox_record->setLayout(m_baseui->m_main_layout);
    this->ui->groupBox_check->setLayout(m_baseui->m_main_layout);
    for(int i = 0; i < 100; i++)
    {
        //this->m_baseui->addAutoRecordUi("com" + QString::number(i), "100100" + QString::number(i));
        this->m_baseui->addCheckComUi();
    }

    this->setWindowTitle(QString::fromLocal8Bit("超版板上位机测试软件"));
    this->showMaximized();

    //版本管理
    connect(this->ui->action_version, &QAction::triggered, this, &MainWindow::VersionSlot);

    //常见问题解决
    connect(this->ui->action_question, &QAction::triggered, this, &MainWindow::QuestionSlot);

    //操作文档说明
    connect(this->ui->action_doc, &QAction::triggered, this, &MainWindow::DocSlot);

    //串口通信设置
    connect(this->ui->action_serialport_set, &QAction::triggered, this, &MainWindow::SerialPortSlot);

    //合并测试
    connect(this->ui->action_hb, &QAction::triggered, this, &MainWindow::TogetherTestSlot);

    //录入标签
    connect(this->ui->action_record, &QAction::triggered, this, &MainWindow::RecordSlot);

    //检查通信
    connect(this->ui->action_com, &QAction::triggered, this, &MainWindow::CheckComSlot);

    //退出程序
    connect(this->ui->action_eixit, &QAction::triggered, this, &MainWindow::CloseApp);
}

