#include "startupmrautofactoryreset.h"
#include "ui_startupmrautofactoryreset.h"

StartUpMrAutoFactoryReset::StartUpMrAutoFactoryReset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartUpMrAutoFactoryReset), m_handle_err(new Common::HandleError)
{
    ui->setupUi(this);
}

StartUpMrAutoFactoryReset::~StartUpMrAutoFactoryReset()
{
    delete ui;
    if(this->m_oc_mr_auto_freset != NULL)
    {
        delete this->m_oc_mr_auto_freset;
        this->m_oc_mr_auto_freset = NULL;
    }

    if(this->m_handle_err != NULL)
    {
        delete this->m_handle_err;
        this->m_handle_err = NULL;
    }
}

void StartUpMrAutoFactoryReset::CloseApp()
{
    this->close();
}

void StartUpMrAutoFactoryReset::StartUpMrAutoFactoryReset_start()
{
    this->setWindowIcon(QIcon(":/openreset.png"));
    this->setWindowTitle(QString::fromLocal8Bit("设置控制服务终端自动恢复出厂设置状态"));
    this->showNormal();

    if(this->m_reset == 1)
    {
        isOpen = true;
        this->ui->pushButton->setText(UTF8BIT("关闭"));
    }

    if(this->m_reset == 0)
    {
        isOpen = false;
        this->ui->pushButton->setText(UTF8BIT("开启"));
    }
}

void StartUpMrAutoFactoryReset::on_pushButton_clicked()
{
    if(isOpen)
    {
        this->m_oc_mr_auto_freset = new HttpClient::OpenCloseMrAutoFactoryResetHttpRequest(0, m_open_close_url);
        this->m_oc_mr_auto_freset->OpenCloseMrAutoFactoryReset([&](bool success, QMap<QString, int>ocmfr_freset)
        {
           if(success)
           {
               if(ocmfr_freset["ec"] == 0)
               {
                   QString succ_log = QString::fromLocal8Bit("设置控制服务终端自动恢复出厂设置状态成功");
                   QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_END;
                   this->ui->textEdit_status->append(succ_log_msg);
               }
               else
               {
                   this->m_handle_err->HandleHttpReqError(ocmfr_freset["ec"]);
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
        this->m_oc_mr_auto_freset = new HttpClient::OpenCloseMrAutoFactoryResetHttpRequest(1, m_open_close_url);
        this->m_oc_mr_auto_freset->OpenCloseMrAutoFactoryReset([&](bool success, QMap<QString, int>ocmfr_freset)
        {
           if(success)
           {
               if(ocmfr_freset["ec"] == 0)
               {
                   QString succ_log = QString::fromLocal8Bit("设置控制服务终端自动恢复出厂设置状态成功");
                   QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_END;
                   this->ui->textEdit_status->append(succ_log_msg);
               }
               else
               {
                   this->m_handle_err->HandleHttpReqError(ocmfr_freset["ec"]);
                   QString log = this->m_handle_err->m_http_req_error;
                   this->ui->textEdit_status->append(log);
               }
           }
        });
        isOpen = true;
        this->ui->pushButton->setText(UTF8BIT("关闭"));
    }
}

