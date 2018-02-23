#include "startupmrautodomain.h"
#include "ui_startupmrautodomain.h"

StartUpMrAutoDomain::StartUpMrAutoDomain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartUpMrAutoDomain), m_handle_err(new Common::HandleError)
{
    ui->setupUi(this);
}

StartUpMrAutoDomain::~StartUpMrAutoDomain()
{
    delete ui;
    if(this->m_sumad_req != NULL)
    {
        delete this->m_sumad_req;
        this->m_sumad_req = NULL;
    }

    if(this->m_handle_err != NULL)
    {
        delete this->m_handle_err;
        this->m_handle_err = NULL;
    }
}

void StartUpMrAutoDomain::CloseApp()
{
    this->close();
}

void StartUpMrAutoDomain::StartUpMrAutoDomain_start()
{
    this->setWindowIcon(QIcon(":/opendomain.png"));
    this->setWindowTitle(QString::fromLocal8Bit("设置控制服务终端自动同步状态"));
    this->showNormal();  
    if(this->m_sync == 1)
    {
        isOpen = true;
        this->ui->pushButton->setText(UTF8BIT("关闭"));
    }

    if(this->m_sync == 0)
    {
        isOpen = false;
        this->ui->pushButton->setText(UTF8BIT("开启"));
    }
}

void StartUpMrAutoDomain::on_pushButton_clicked()
{
    if(isOpen)
    {
        this->m_sumad_req = new HttpClient::StartUpMrAutoDomainHttpRequest(0, m_sync_rul);
        this->m_sumad_req->StartUpMrAutoDomain([&](bool success, QMap<QString, int>open_domain)
        {
           if(success)
           {
               if(open_domain["ec"] == 0)
               {
                   QString succ_log = QString::fromLocal8Bit("设置控制服务终端自动同步状态成功");
                   QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_END;
                   this->ui->textEdit_status->append(succ_log_msg);
               }
               else
               {
                   this->m_handle_err->HandleHttpReqError(open_domain["ec"]);
                   QString log = this->m_handle_err->m_http_req_error;
                   this->ui->textEdit_status->append(log);
               }
           }
        });
        isOpen = false;
        this->ui->pushButton->setText(UTF8BIT("开启"));
    }
    else
    {
        this->m_sumad_req = new HttpClient::StartUpMrAutoDomainHttpRequest(1, m_sync_rul);
        this->m_sumad_req->StartUpMrAutoDomain([&](bool success, QMap<QString, int>open_domain)
        {
           if(success)
           {
               if(open_domain["ec"] == 0)
               {
                   QString succ_log = QString::fromLocal8Bit("设置控制服务终端自动同步状态成功");
                   QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_END;
                   this->ui->textEdit_status->append(succ_log_msg);
               }
               else
               {
                   this->m_handle_err->HandleHttpReqError(open_domain["ec"]);
                   QString log = this->m_handle_err->m_http_req_error;
                   this->ui->textEdit_status->append(log);
               }
           }
        });
        isOpen = true;
        this->ui->pushButton->setText(UTF8BIT("关闭"));
    }
}

void StartUpMrAutoDomain::onProNum(QString pro_num)
{

}


void StartUpMrAutoDomain::onRequestFinish(QString finish_info)
{

}

void StartUpMrAutoDomain::onRequestSuccess(QString succ_info)
{

}

void StartUpMrAutoDomain::onRequestFail(QString err_info)
{

}

