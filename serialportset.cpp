#include "serialportset.h"
#include "ui_serialportset.h"
#include "common.h"

using namespace Common;

SerialPortSet::SerialPortSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialPortSet), m_sport_xml(new ParseXml)
{
    ui->setupUi(this);
    this->ui->lineEdit_btl->setDisabled(true);
    this->ui->lineEdit_checkbit->setDisabled(true);
    this->ui->lineEdit_databit->setDisabled(true);
    this->ui->lineEdit_stopbit->setDisabled(true);
}

SerialPortSet::~SerialPortSet()
{
    delete ui;
    delete m_sport_xml;
}

void SerialPortSet::serPort_start()
{
    int ret = this->init();
    if(ret == SERIPORTCINITERR)
    {
        std::cout << "init fail" << std::endl;
        return ;
    }
    this->setWindowIcon(QIcon(":/sportset.png"));
    this->setWindowTitle(QString::fromLocal8Bit("串口信息配置"));
    this->showNormal();
}

int SerialPortSet::init()
{
   int ret_list =  m_sport_xml->ParseConfigListXml("D:/latestQt/IspdNewCalibrator/etc/serialport.xml");
   if(ret_list != 7000)
   {
       this->ui->comboBox_btl->addItems(m_sport_xml->m_sport_con.baud_rate);
       this->ui->comboBox_checkbit->addItems(m_sport_xml->m_sport_con.check_dit);
       this->ui->comboBox_databit->addItems(m_sport_xml->m_sport_con.data_bit);
       this->ui->comboBox_stopbit->addItems(m_sport_xml->m_sport_con.stop_bit);
   }
   else
   {
       std::cout << "parse config list xml fail" << std::endl;
       return SERIPORTCINITERR;
   }

   int ret =  m_sport_xml->ParseConfigXml("D:/latestQt/IspdNewCalibrator/etc/datainfo.xml");
   if(ret != 7000)
   {
       this->ui->lineEdit_btl->setText(m_sport_xml->m_baud_rate);
       this->ui->lineEdit_checkbit->setText(m_sport_xml->m_check_dit);
       this->ui->lineEdit_databit->setText(m_sport_xml->m_data_bit);
       this->ui->lineEdit_stopbit->setText(m_sport_xml->m_stop_bit);
   }
   else
   {
       std::cout << "parese config xml fail" << std::endl;
       return SERIPORTCINITERR;
   }
   return 0;
}

void SerialPortSet::on_pushButton_sport_set_clicked()
{
    this->m_sport_xml->m_dcinfo.baud_rate = this->ui->comboBox_btl->currentText();
    this->m_sport_xml->m_dcinfo.check_dit = this->ui->comboBox_checkbit->currentText();
    this->m_sport_xml->m_dcinfo.data_bit = this->ui->comboBox_databit->currentText();
    this->m_sport_xml->m_dcinfo.stop_bit = this->ui->comboBox_stopbit->currentText();
    int ret = this->m_sport_xml->WriteConfigXml("D:/latestQt/IspdNewCalibrator/etc/datainfo.xml");
    if(ret != 7001)
    {
        QString info = QString::fromLocal8Bit("配置成功");
        HANDLEMSGBOXINFO(info);
        return ;
    }
    else
    {
        QString info = QString::fromLocal8Bit("配置失败");
        HANDLEMSGBOXWARNING(info);
        return ;
    }
}

void SerialPortSet::on_tabWidget_tabBarClicked()
{
    int ret =  m_sport_xml->ParseConfigXml("D:/latestQt/IspdNewCalibrator/etc/datainfo.xml");
    if(ret != 7000)
    {
        this->ui->lineEdit_btl->setText(m_sport_xml->m_baud_rate);
        this->ui->lineEdit_checkbit->setText(m_sport_xml->m_check_dit);
        this->ui->lineEdit_databit->setText(m_sport_xml->m_data_bit);
        this->ui->lineEdit_stopbit->setText(m_sport_xml->m_stop_bit);
    }
    else
    {
        std::cout << "get config info error" << std::endl;
        return ;
    }
}

void SerialPortSet::on_pushButton_clicked()
{
    this->close();
}
