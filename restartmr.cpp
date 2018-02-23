#include "restartmr.h"
#include "ui_restartmr.h"

RestartMr::RestartMr(QWidget *parent) :
    QDialog(parent), ui(new Ui::RestartMr),
    m_handle_err(new Common::HandleError)
{
    ui->setupUi(this);
}

RestartMr::~RestartMr()
{
    delete ui;
    if(this->m_restart_specmr != NULL)
    {
        delete this->m_restart_specmr;
        this->m_restart_specmr = NULL;
    }

    if(this->m_handle_err != NULL)
    {
        delete this->m_handle_err;
        this->m_handle_err = NULL;
    }
}

void RestartMr::CloseApp()
{
    this->close();
}

void RestartMr::RestartMrInit()
{
    this->setWindowIcon(QIcon(":/startup.png"));
    this->setWindowTitle(QString::fromLocal8Bit("重启指定终端"));
    this->showNormal();
    for(int i = 0; i < this->m_mid.size(); i++)
    {
        this->ui->comboBox_id->addItem(QString::number(this->m_mid.at(i)));
    }
}

void RestartMr::on_pushButton_clicked()
{
    this->m_req_id = this->ui->comboBox_id->currentText();
    this->m_restart_specmr = new HttpClient::RestartSpecMrHttpReqest(m_req_id, m_restart_url);
    this->m_restart_specmr->RestartSpecMr([&](bool success, QMap<QString, int>restart_specmr)
    {
       if(success)
       {
           if(restart_specmr["ec"] == SUCCESS_CODE)
           {
               QString succ_log = QString::fromLocal8Bit("重启指定终端成功，终端ID为:") + this->m_req_id;
               QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_END;
               this->ui->textEdit_status->append(succ_log_msg);
           }
           else
           {
               this->m_handle_err->HandleHttpReqError(restart_specmr["ec"]);
               QString log = this->m_handle_err->m_http_req_error;
               QString id_str = MESSAGE_RED + this->m_req_id + MESSAGE_END;
               this->ui->textEdit_status->append(log + id_str);
           }
       }
    });
}
