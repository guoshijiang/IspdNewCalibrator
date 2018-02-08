﻿#include "startupmrautodomain.h"
#include "ui_startupmrautodomain.h"

StartUpMrAutoDomain::StartUpMrAutoDomain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartUpMrAutoDomain)
{
    ui->setupUi(this);
}

StartUpMrAutoDomain::~StartUpMrAutoDomain()
{
    delete ui;
    delete this->m_sumad_req;
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

    qDebug() << "data:" << this->m_sync;

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
                   this->ui->textEdit_status->append(QString::fromLocal8Bit("设置控制服务终端自动同步状态成功"));
               }
               else
               {
                   this->ui->textEdit_status->append(QString::fromLocal8Bit("设置控制服务终端自动同步状态失败"));
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
                   this->ui->textEdit_status->append(QString::fromLocal8Bit("设置控制服务终端自动同步状态成功"));
               }
               else
               {
                   this->ui->textEdit_status->append(QString::fromLocal8Bit("设置控制服务终端自动同步状态失败"));
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

