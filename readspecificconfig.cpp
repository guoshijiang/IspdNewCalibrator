#include "readspecificconfig.h"
#include "ui_readspecificconfig.h"

ReadSpecificConfig::ReadSpecificConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadSpecificConfig), m_read_spec_mrinfo(new HttpClient::ReadSpecMrConfigInfoHttpReqest)
{
    ui->setupUi(this);
}

ReadSpecificConfig::~ReadSpecificConfig()
{
    delete ui;
    if(this->m_read_spec_mrinfo != NULL)
    {
        delete this->m_read_spec_mrinfo;
        this->m_read_spec_mrinfo = NULL;
    }
}

void ReadSpecificConfig::CloseApp()
{
    this->close();
}

void ReadSpecificConfig::ReadSpecConfig_start()
{
    this->setWindowIcon(QIcon(":/read_mr.png"));
    this->setWindowTitle(QString::fromLocal8Bit("读指定终端配置信息"));
    this->showNormal();
    for(int i = 0; i < m_list_mid.size(); i++)
    {
        this->ui->comboBox_id->addItem(QString::number(this->m_list_mid.at(i)));
    }
}

void ReadSpecificConfig::on_pushButton_clicked()
{
    this->m_mid = this->ui->comboBox_id->currentText();
    this->m_read_spec_mrinfo->GetReqestDataFromUI(m_mid);
    this->m_read_spec_mrinfo->ReadSpecMrConfigInfo([&](bool success, QMap<QString, QVariant>read_sepc_mr)
    {
       if(success)
       {
           this->ui->lineEdit_rep_goal_id->setText(QString::number(read_sepc_mr["mid"].toInt()));
           this->ui->lineEdit_rep_j_ip->setText(read_sepc_mr["lip"].toString());
           this->ui->lineEdit_rep_j_zip->setText(read_sepc_mr["nip"].toString());
           this->ui->lineEdit_rep_j_gateway->setText(read_sepc_mr["gip"].toString());
           this->ui->lineEdit_rep_j_dip->setText(read_sepc_mr["dip"].toString());
           this->ui->lineEdit_rep_dyn_ip->setText(read_sepc_mr["lip2"].toString());
           this->ui->lineEdit_rep_dyn_code->setText(read_sepc_mr["nip2"].toString());
           this->ui->lineEdit_rep_dyn_gip->setText(read_sepc_mr["gip2"].toString());
           this->ui->lineEdit_rep_dyn_dip->setText(read_sepc_mr["dip2"].toString());
           this->ui->lineEdit_rep_tp->setText(QString::number(read_sepc_mr["tp"].toInt()));
           this->ui->lineEdit_rep_up->setText(QString::number(read_sepc_mr["up"].toInt()));
           this->ui->lineEdit_rep_network_select->setText(QString::number(read_sepc_mr["nm"].toInt()));
           this->ui->lineEdit_rep_time->setText(QString::number(read_sepc_mr["ari"].toInt()));
           this->ui->lineEdit_rep_ver->setText(read_sepc_mr["ver"].toString());
           this->ui->lineEdit_rep_dv->setText(read_sepc_mr["dv"].toString());
           this->ui->lineEdit_mr_mac->setText(read_sepc_mr["mac"].toString());
           this->ui->lineEdit_server_yuming->setText(read_sepc_mr["domain"].toString());
           if(read_sepc_mr["ec"] == 0)
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("读指定终端配置信息成功，终端编号为:") + QString::number(read_sepc_mr["mid"].toInt()));
           }
           else
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("读指定终端配置信息失败, 终端编号为:") + QString::number(read_sepc_mr["mid"].toInt()));
           }
       }
    });
}
