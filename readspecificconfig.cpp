#include "readspecificconfig.h"
#include "ui_readspecificconfig.h"

ReadSpecificConfig::ReadSpecificConfig(QWidget *parent) :
    QDialog(parent), ui(new Ui::ReadSpecificConfig),
    m_write_specmr_info(new HttpClient::WriteSpecMrConfigInfoHttpRequest),
    m_handle_err(new Common::HandleError)
{
    ui->setupUi(this);
}

ReadSpecificConfig::~ReadSpecificConfig()
{
    delete ui;
    if(m_read_spec_mrinfo != NULL)
    {
        delete this->m_read_spec_mrinfo;
        this->m_read_spec_mrinfo = NULL;
    }

    if(this->m_handle_err != NULL)
    {
        delete this->m_handle_err;
        this->m_handle_err = NULL;
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
    m_read_spec_mrinfo = new HttpClient::ReadSpecMrConfigInfoHttpReqest(m_mid, m_url);
    this->m_read_spec_mrinfo->ReadSpecMrConfigInfo([&](bool success, QMap<QString, QVariant>read_sepc_mr)
    {
       if(success)
       {
           if(read_sepc_mr["ec"] == SUCCESS_CODE)
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

               QString succ_log = QString::fromLocal8Bit("读指定终端配置信息成功，终端ID为:") + QString::number(read_sepc_mr["mid"].toInt());
               QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_END;
               this->ui->textEdit_status->append(succ_log_msg);
           }
           else
           {
               this->m_handle_err->HandleHttpReqError(read_sepc_mr["ec"].toInt());
               QString log = this->m_handle_err->m_http_req_error;
               QString id_str = MESSAGE_RED + QString::number(read_sepc_mr["mid"].toInt()) + MESSAGE_END;
               this->ui->textEdit_status->append(log + id_str);
           }
       }
    });
}

void ReadSpecificConfig::on_pushButton_save_clicked()
{
    this->m_write_specmr_info->m_wsmr.mmid = this->ui->lineEdit_rep_goal_id->text().toInt();

    this->m_write_specmr_info->m_wsmr.lip = this->ui->lineEdit_rep_j_ip->text();
    this->m_write_specmr_info->m_wsmr.nip = this->ui->lineEdit_rep_j_zip->text();
    this->m_write_specmr_info->m_wsmr.gip = this->ui->lineEdit_rep_j_gateway->text();
    this->m_write_specmr_info->m_wsmr.dip = this->ui->lineEdit_rep_j_dip->text();
    this->m_write_specmr_info->m_wsmr.domain = this->ui->lineEdit_server_yuming->text();

    this->m_write_specmr_info->m_wsmr.tp = this->ui->lineEdit_rep_tp->text().toInt();
    this->m_write_specmr_info->m_wsmr.up = this->ui->lineEdit_rep_up->text().toInt();

    this->m_write_specmr_info->m_wsmr.version = this->ui->lineEdit_rep_ver->text();

    this->m_write_specmr_info->m_wsmr.nm = this->ui->lineEdit_rep_network_select->text().toInt();
    this->m_write_specmr_info->m_wsmr.ari = this->ui->lineEdit_rep_time->text().toInt();

    this->m_write_specmr_info->M_WRITE_RUL = m_url;
    this->m_write_specmr_info->WriteSpecMrConfigInfo([&](bool success, QMap<QString, int>write_pecmr)
    {
       if(success)
       {
           if(write_pecmr["ec"] == 0)
           {
               QString succ_log = QString::fromLocal8Bit("写指定终端配置信息成功，终端ID为:") + this->ui->lineEdit_rep_goal_id->text();
               QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_END;
               this->ui->textEdit_status->append(succ_log_msg);
           }
           else
           {
               this->m_handle_err->HandleHttpReqError(write_pecmr["ec"]);
               QString log = this->m_handle_err->m_http_req_error;
               QString id_str = MESSAGE_RED + this->ui->lineEdit_rep_goal_id->text() + MESSAGE_END;
               this->ui->textEdit_status->append(log + id_str);
           }
       }
    });
}

void ReadSpecificConfig::on_pushButton_cancel_clicked()
{
    this->close();
}
