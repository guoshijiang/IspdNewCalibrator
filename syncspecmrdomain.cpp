#include "syncspecmrdomain.h"
#include "ui_syncspecmrdomain.h"

SyncSpecMrDomain::SyncSpecMrDomain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SyncSpecMrDomain), m_sync_specmr_domain(new HttpClient::SyncSpecMrDomainHttpRequest)
{
    ui->setupUi(this);
}

SyncSpecMrDomain::~SyncSpecMrDomain()
{
    delete ui;
    if(this->m_sync_specmr_domain != NULL)
    {
        delete this->m_sync_specmr_domain;
        this->m_sync_specmr_domain = NULL;
    }
}

void SyncSpecMrDomain::CloseApp()
{
    this->close();
}

void SyncSpecMrDomain::SyncSpecMrDomainInit()
{
    this->setWindowIcon(QIcon(":/synchronization.png"));
    this->setWindowTitle(QString::fromLocal8Bit("同步指定终端的Domain"));
    this->showNormal();
}

void SyncSpecMrDomain::on_pushButton_clicked()
{
    this->m_req_pro = this->ui->lineEdit_req_pro->text();
    this->m_req_id = this->ui->lineEdit_req_id->text();
    this->m_req_host = this->ui->lineEdit_req_host->text();

    this->m_sync_specmr_domain->GetReqestDataFromUI(m_req_pro, m_req_id, m_req_host);
    this->m_sync_specmr_domain->SyncSpecMrDomain([&](bool success, QMap<QString, int>sync_domain)
    {
       if(success)
       {
           int req_pro = sync_domain["pro"];
           int req_ec = sync_domain["ec"];
           this->ui->lineEdit_rep_pro->setText(QString::number(req_pro));
           this->ui->lineEdit_rep_status->setText(QString::number(req_ec));
           if(sync_domain["ec"] == 0)
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
