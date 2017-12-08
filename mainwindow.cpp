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
#include "protoserialport.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_version(new Version),
    m_question(new Question), m_doc(new Document),
    m_port_set(new SerialPortSet), m_serial_port(new SerialPort),
    m_encrypt(new Encrypt)
{
    QScrollArea *pArea = new QScrollArea();
    QWidget *pWidget = new QWidget();
    pWidget->setStyleSheet("QWidget" "{background:white;}");
    m_vbox_layout = new QVBoxLayout();
    m_vbox_layout->addSpacerItem(new QSpacerItem(100, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    pWidget->setLayout(m_vbox_layout);
    pArea->setWidget(pWidget);
    pArea->setWidgetResizable(true);
    m_main_layout = new QVBoxLayout();
    m_main_layout->addWidget(pArea);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->m_version;
    delete this->m_question;
    delete this->m_doc;
    delete this->m_port_set;
    delete this->m_serial_port;
    delete this->m_encrypt;
}

void MainWindow::testAutoSlot()
{
   this->ui->tabWidget->setCurrentIndex(4);
}

void MainWindow::testMulSlot()
{ 
    this->ui->tabWidget->setCurrentIndex(3);
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

void MainWindow::start()
{
    this->setWindowTitle(QString::fromLocal8Bit("超版板上位机测试软件"));
    this->showMaximized();

    connect(this->ui->action_auto, &QAction::triggered, this, &MainWindow::testAutoSlot);
    connect(this->ui->action_mul, &QAction::triggered, this, &MainWindow::testMulSlot);
    connect(this->ui->action_version, &QAction::triggered, this, &MainWindow::VersionSlot);
    connect(this->ui->action_question, &QAction::triggered, this, &MainWindow::QuestionSlot);
    connect(this->ui->action_doc, &QAction::triggered, this, &MainWindow::DocSlot);
    connect(this->ui->action_serialport_set, &QAction::triggered, this, &MainWindow::SerialPortSlot);
    connect(this->ui->action_hb, &QAction::triggered, this, &MainWindow::TogetherTestSlot);
    connect(this->ui->action_record, &QAction::triggered, this, &MainWindow::RecordSlot);
    connect(this->ui->action_com, &QAction::triggered, this, &MainWindow::CheckComSlot);
    connect(this->ui->action_eixit, &QAction::triggered, this, &MainWindow::CloseApp);

    this->m_com_list.clear();
    for (int i=0; i < m_serial_port->m_set_len; i++)
    {
        m_serial_port->getSerialPortList(i, "value");
        m_com_list << this->m_serial_port->m_comm_result;
    }

    /*
    this->ui->groupBox_record->setLayout(m_main_layout);

    for(int i = 0; i < m_com_list.size(); ++i)
    {
        if(m_com_list[i] != "COM1")
        {
            this->addAutoRecordUi(m_com_list[i]);
        }
        else
        {
            continue;
        }
    }
    */

    this->ui->groupBox_Mul_Recoed->setLayout(m_main_layout);
    for(int i = 0; i < m_com_list.size(); ++i)
    {
        if(m_com_list[i] != "COM1")
        {
            this->addMulRecordUi(m_com_list[i]);
        }
        else
        {
            continue;
        }
    }
}

void MainWindow::addAutoRecordUi(QString com_name)
{
    this->m_page = new QWidget;
    this->m_layout = new QGridLayout(m_page);

    this->m_label = new QLabel;
    this->m_label->setText(com_name);
    this->m_label->setFont(font());

    this->m_ledit = new QLineEdit;
    this->m_ledit->setFont(font());
    this->m_auto_record_list.push_back(this->m_ledit);

    this->m_layout->addWidget(m_label, 0, 1);
    this->m_layout->addWidget(m_ledit, 0, 2);

    this->m_page->setLayout(m_layout);

    this->m_vbox_layout->insertWidget(m_vbox_layout->count()-1, m_page);

    this->m_ledit->setObjectName(com_name);

    connect(this->m_ledit, SIGNAL(textChanged(QString)), SLOT(m_ledit_textChanged(QString)));
}

void MainWindow::addMulRecordUi(QString com_name)
{
    this->m_page = new QWidget;
    this->m_layout = new QGridLayout(m_page);

    this->m_label = new QLabel;
    this->m_label->setText(com_name);
    this->m_label->setFont(font());

    this->m_ledit = new QLineEdit;
    this->m_ledit->setFont(font());
    this->m_mul_record_list.push_back(this->m_ledit);

    this->m_but = new QPushButton;
    this->m_but->setText("record");
    this->m_but->setFont(font());

    this->m_layout->addWidget(m_label, 0, 1);
    this->m_layout->addWidget(m_ledit, 0, 2);
    this->m_layout->addWidget(m_but, 0, 3);

    this->m_page->setLayout(m_layout);

    this->m_vbox_layout->insertWidget(m_vbox_layout->count()-1, m_page);

    this->m_ledit->setObjectName(com_name);
    this->m_but->setObjectName(com_name);

    connect(m_but, SIGNAL(clicked()), this, SLOT(on_m_but_clicked()));

}

void MainWindow::on_m_but_clicked()
{
    QObject *obj = QObject::sender();
    QString senderobjName = obj->objectName();
    qDebug() << senderobjName;
}

void MainWindow::m_ledit_textChanged(QString text)
{
    QObject *obj = QObject::sender();
    QString senderobjName = obj->objectName();
    for(int k = 0; k < this->m_com_list.size(); ++k)
    {
        if(this->m_com_list[k] == "COM1")
        {
            continue;
        }

        if(this->m_com_list[k] == senderobjName)
        {
            QString f_com_name = m_com_list[k];
            QByteArray to_data = f_com_name.toLatin1();
            char* com_str = to_data.data();
            qDebug() << "com_str = " << com_str;
            bool ret = this->m_serial_port->initSerilPort(com_str, 230400, 'N', 8, 1);
            if(ret == false)
            {
                qDebug() << "init fail !";
            }
            else
            {
                //预配置
                QByteArray pre_array;
                quint8 pre_write = 0xFF;
                quint8 cnt = 15;
                while (cnt--)
                {
                    pre_array.append(pre_write);
                }

                SerialPort::Data data;
                data.buffer = pre_array;
                data.length = pre_array.length();
                int ok = this->m_serial_port->writeData(data);
                if(ok == -1)
                {
                    qDebug() << "write fail !";
                }
                else
                {
                    qDebug() << "write succes !";
                }

                //进入配置模式
                protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_MODULE_CUSTOMISE);
                QByteArray send_cus_pkg = cus_pkg.toBinary();
                data.buffer = send_cus_pkg.data();
                data.length =send_cus_pkg.length();
                int ok1 = this->m_serial_port->writeData(data);
                if(ok1 == -1)
                {
                    qDebug() << "write 1 fail !";
                }
                else
                {
                    qDebug() << "write 1 succes !";
                }

                //录入ID
                QString q_ispd = text;
                std::string current_sign_id = q_ispd.toStdString();
                qint32 ispd_id_dencrypt = this->m_encrypt->dencrypt_ispd_id(current_sign_id);
                protoserialport::IspdID ispd_id(ispd_id_dencrypt, protoserialport::REQ_TYPE::ISPD_WR_ID);
                protoserialport::ReqMsg<protoserialport::IspdID> res_msg(ispd_id);
                QByteArray send_id = res_msg.toBinary();
                data.buffer = send_id.data();
                data.length =  send_id.length();
                int ok2 = this->m_serial_port->writeData(data);
                if(ok2 == -1)
                {
                    qDebug() << "write 2 fail !";
                }
                else
                {
                    qDebug() << "write 2 succes !";
                }
            }
        }
    }
}




