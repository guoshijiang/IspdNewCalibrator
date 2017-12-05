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
#if 1
    QScrollArea *pArea = new QScrollArea();
    m_add_but= new QPushButton();

    QWidget *pWidget = new QWidget();
    pWidget->setStyleSheet("QWidget" "{background:white;}");
    m_vbox_layout = new QVBoxLayout();
    m_vbox_layout->addSpacerItem(new QSpacerItem(100, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    pWidget->setLayout(m_vbox_layout);
    pArea->setWidget(pWidget);
    pArea->setWidgetResizable(true);
    m_main_layout = new QVBoxLayout();
    m_main_layout->addWidget(pArea);
    m_main_layout->addWidget(m_add_but);

    connect(m_add_but, &QPushButton::clicked, this, &MainWindow::addAutoRecordUi);
#endif

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
    QStringList comlist ;
    comlist.clear();
    for (int i=0; i < m_serial_port->m_set_len; i++)
    {
        m_serial_port->getSerialPortList(i, "value");
        comlist << this->m_serial_port->m_comm_result;
    }
    qDebug() << "windows:" << comlist;
    /*
    bool initSerilPort(UINT portNo = 1, UINT baud = CBR_9600,
                       char parity = 'N', UINT databits = 8,
                       UINT stopsbits = 1);
    */
    bool ret = this->m_serial_port->initSerilPort(3, 230400, 'N', 8, 1);
    if(ret == false)
    {
        qDebug() << "init fail !";
    }
    else
    {
        qDebug() << "init success !";
    }
    this->m_serial_port->SetWriteTimeout(9000);

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

    /*
    bool bl_pz = this->m_serial_port->ReadChar();
    if(bl_pz == false)
    {
        qDebug() << "ReadChar PZ error !";
    }
    else
    {
        qDebug() << "ReadChar PZ Success !";
    }
    */

    /*
    std::string current_sign_id = "222222";
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
    */

    protoserialport::ReqPkg cus_pkg_id(protoserialport::REQ_TYPE::ISPD_RD_ID);
    QByteArray send_cus_pkg_id = cus_pkg_id.toBinary();
    data.buffer = send_cus_pkg_id.data();
    data.length =  send_cus_pkg_id.length();
    int ok3 = this->m_serial_port->writeData(data);
    if(ok3 == -1)
    {
        qDebug() << "write 3 fail !";
    }
    else
    {
        qDebug() << "write 3 succes !";
    }

    char *buf;
    this->m_serial_port->readData(buf, 9);



    /*
    m_factory = new InterfaceUI::RecordUIFactory;
    m_baseui = m_factory->CreatUIRole();
    m_baseui->BaseScrollArea();
    this->ui->groupBox_record->setLayout(m_baseui->m_main_layout);
    for(int i = 0; i < 100; i++)
    {
        this->m_baseui->AddAutoChildUI("com" + QString::number(i));
    }

    m_factory = new InterfaceUI::CheckUIFactory;
    m_baseui = m_factory->CreatUIRole();
    m_baseui->BaseScrollArea();
    this->ui->groupBox_check->setLayout(m_baseui->m_main_layout);
    for(int i = 0; i < 10; i++)
    {
        this->m_baseui->AddMulChildUI();
    }
    */

    this->ui->groupBox_record->setLayout(m_main_layout);
    for(int i = 0; i < 20; ++i)
    {
        this->addAutoRecordUi();
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


void MainWindow::addAutoRecordUi()
{
    this->m_page = new QWidget;
    this->m_layout = new QGridLayout(m_page);

    this->m_label = new QLabel;
    this->m_label->setText("com");
    this->m_label->setFont(font());

    this->m_ledit = new QLineEdit;
    this->m_ledit->setFont(font());
    this->m_list.push_back(this->m_ledit);

    this->m_layout->addWidget(m_label, 0, 1);
    this->m_layout->addWidget(m_ledit, 0, 2);
    this->m_page->setLayout(m_layout);
    this->m_vbox_layout->insertWidget(m_vbox_layout->count()-1, m_page);

    this->m_ledit->setObjectName("m_ledit"+QString::number(m_list.count()));
    this->m_vec.resize(m_list.count());
    connect(this->m_ledit, SIGNAL(textChanged(QString)), SLOT(m_ledit_textChanged(QString)));

}

void MainWindow::m_ledit_textChanged(QString text)
{
    QObject *obj = QObject::sender();
    QString senderobjName = obj->objectName();
    int isTextchanged = senderobjName.indexOf("m_ledit") != -1;
    if (isTextchanged)
    {
        QStringList list = senderobjName.split("m_ledit");
        unsigned int index = list[1].toInt();
        this->m_vec[index] = text;
        qDebug() <<  this->m_vec[index];
    }
}





