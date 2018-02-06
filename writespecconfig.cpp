#include "writespecconfig.h"
#include "ui_writespecconfig.h"

WriteSpecConfig::WriteSpecConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WriteSpecConfig), m_write_specmr_info(new HttpClient::WriteSpecMrConfigInfoHttpRequest)
{
    ui->setupUi(this);
}

WriteSpecConfig::~WriteSpecConfig()
{
    delete ui;
    if(this->m_write_specmr_info != NULL)
    {
        delete this->m_write_specmr_info;
        this->m_write_specmr_info = NULL;
    }
}

void WriteSpecConfig::CloseApp()
{
    this->close();
}

void  WriteSpecConfig::WriteSpecConfig_start()
{
    this->setWindowIcon(QIcon(":/write_mr.png"));
    this->setWindowTitle(QString::fromLocal8Bit("写指定终端配置信息"));
    this->showNormal();
}

void WriteSpecConfig::on_pushButton_clicked()
{
    this->m_write_specmr_info->m_wsmr.mmid = this->ui->lineEdit_req_mr_id->text().toInt();
    this->m_write_specmr_info->m_wsmr.lip = this->ui->lineEdit_req_jip->text();
    this->m_write_specmr_info->m_wsmr.nip = this->ui->lineEdit_childnet_code->text();
    this->m_write_specmr_info->m_wsmr.gip = this->ui->lineEdit_req_gateway->text();
    this->m_write_specmr_info->m_wsmr.dip = this->ui->lineEdit_dns->text();

    this->m_write_specmr_info->m_wsmr.domain = this->ui->lineEdit_sever_yuming->text();
    this->m_write_specmr_info->m_wsmr.tp = this->ui->lineEdit_req_tp->text().toInt();
    this->m_write_specmr_info->m_wsmr.up = this->ui->lineEdit_req_up->text().toInt();

    this->m_write_specmr_info->m_wsmr.version = this->ui->lineEdit_version->text();

    this->m_write_specmr_info->m_wsmr.nm = this->ui->lineEdit_network_select->text().toInt();
    this->m_write_specmr_info->m_wsmr.ari = this->ui->lineEdit_req_time->text().toInt();

    this->m_write_specmr_info->WriteSpecMrConfigInfo([&](bool success, QMap<QString, int>write_pecmr)
    {
       if(success)
       {
           if(write_pecmr["ec"] == 0)
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("写指定终端配置信息成功"));
           }
           else
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("写指定终端配置信息失败"));
           }
       }
    });
}


