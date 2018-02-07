#include "startupmrautofactoryreset.h"
#include "ui_startupmrautofactoryreset.h"

StartUpMrAutoFactoryReset::StartUpMrAutoFactoryReset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartUpMrAutoFactoryReset), m_oc_mr_auto_freset(new HttpClient::OpenCloseMrAutoFactoryResetHttpRequest)
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
        this->m_oc_mr_auto_freset->GetReqProNumber(0);
        this->m_oc_mr_auto_freset->OpenCloseMrAutoFactoryReset([&](bool success, QMap<QString, int>ocmfr_freset)
        {
           if(success)
           {
               if(ocmfr_freset["ec"] == 0)
               {
                   this->ui->textEdit_status->append(QString::fromLocal8Bit("设置控制服务终端自动恢复出厂设置状态成功"));
               }
               else
               {
                   this->ui->textEdit_status->append(QString::fromLocal8Bit("设置控制服务终端自动恢复出厂设置状态"));
               }
           }
        });
        isOpen = false;
        this->ui->pushButton->setText(UTF8BIT("开启"));
    }
    else
    {
        this->m_oc_mr_auto_freset->GetReqProNumber(1);
        this->m_oc_mr_auto_freset->OpenCloseMrAutoFactoryReset([&](bool success, QMap<QString, int>ocmfr_freset)
        {
           if(success)
           {
               if(ocmfr_freset["ec"] == 0)
               {
                   this->ui->textEdit_status->append(QString::fromLocal8Bit("设置控制服务终端自动恢复出厂设置状态成功"));
               }
               else
               {
                   this->ui->textEdit_status->append(QString::fromLocal8Bit("设置控制服务终端自动恢复出厂设置状态"));
               }
           }
        });
        isOpen = true;
        this->ui->pushButton->setText(UTF8BIT("关闭"));
    }

}

