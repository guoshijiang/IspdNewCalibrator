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

void StartUpMrAutoFactoryReset::StartUpMrAutoFactoryReset_start()
{
    this->setWindowIcon(QIcon(":/openreset.png"));
    this->setWindowTitle(QString::fromLocal8Bit("开启接入终端自动恢复出厂设置功能"));
    this->showNormal();
}

void StartUpMrAutoFactoryReset::on_pushButton_clicked()
{
    this->m_req_pro = this->ui->lineEdit_req_pro->text();
    this->m_oc_mr_auto_freset->GetReqProNumber(m_req_pro);
    this->m_oc_mr_auto_freset->OpenCloseMrAutoFactoryReset([&](bool success, QMap<QString, int>ocmfr_freset)
    {
       if(success)
       {
           int req_pro = ocmfr_freset["pro"];
           int req_ec = ocmfr_freset["ec"];
           this->ui->lineEdit_rep_pro->setText(QString::number(req_pro));
           this->ui->lineEdit_rep_status->setText(QString::number(req_ec));
           if(ocmfr_freset["ec"] == 0)
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("开启接入终端自动同步Domain功能成功，终端编号为:") + QString::number(req_pro));
           }
           else
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("开启接入终端自动同步Domain功能失败, 终端编号为:") + QString::number(req_pro));
           }
       }
    });
}

