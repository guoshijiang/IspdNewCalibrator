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

void ReadSpecificConfig::ReadSpecConfig_start()
{
    this->setWindowIcon(QIcon(":/read_mr.png"));
    this->setWindowTitle(QString::fromLocal8Bit("读指定终端配置信息"));
    this->showNormal();
}

void ReadSpecificConfig::on_pushButton_clicked()
{
    this->m_req_pro = this->ui->lineEdit_req_pro->text();
    this->m_read_spec_mrinfo->GetReqestDataFromUI(m_req_pro);
    this->m_read_spec_mrinfo->ReadSpecMrConfigInfo([&](bool success, QMap<QString, QVariant>read_sepc_mr)
    {
       if(success)
       {
           this->ui->lineEdit_rep_mr_code->setText(QString::number(read_sepc_mr["pro"].toInt()));
           this->ui->lineEdit_rep_goal_id->setText(QString::number(read_sepc_mr["mid"].toInt()));
           this->ui->lineEdit_rep_status->setText(QString::number(read_sepc_mr["ec"].toInt()));
           this->ui->lineEdit_rep_rev->setText(QString::number(read_sepc_mr["rev"].toInt()));
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
           this->ui->lineEdit_rep_goal_ip->setText(read_sepc_mr["domain"].toString());
           this->ui->lineEdit_rep_network_select->setText(QString::number(read_sepc_mr["nsm"].toInt()));
           this->ui->lineEdit_rep_time->setText(QString::number(read_sepc_mr["ar"].toInt()));
           this->ui->lineEdit_rep_udata->setText(read_sepc_mr["udata"].toString());
           this->ui->lineEdit_rep_dipvalue->setText(read_sepc_mr["dipvalue"].toString());
           if(read_sepc_mr["ec"] == 0)
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("读指定终端配置信息成功，终端编号为:") + QString::number(read_sepc_mr["pro"].toInt()));
           }
           else
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("读指定终端配置信息失败, 终端编号为:") + QString::number(read_sepc_mr["pro"].toInt()));
           }
       }
    });
}
