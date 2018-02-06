#include "startupmrautodomain.h"
#include "ui_startupmrautodomain.h"

StartUpMrAutoDomain::StartUpMrAutoDomain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartUpMrAutoDomain), m_sumad_req(new HttpClient::StartUpMrAutoDomainHttpRequest)
{
    ui->setupUi(this);
    this->ui->lineEdit_rep_pro->setDisabled(true);
    this->ui->lineEdit_rep_wc->setDisabled(true);
    connect(m_sumad_req, SIGNAL(SendProNumber(QString)), this, SLOT(onProNum(QString)));
    connect(m_sumad_req, SIGNAL(SendRequestFinish(QString)), this, SLOT(onRequestFinish(QString)));
    connect(m_sumad_req, SIGNAL(RequestSuccess(QString)), this, SLOT(onRequestSuccess(QString)));
    connect(m_sumad_req, SIGNAL(RequestFail(QString)), this, SLOT(onRequestFail(QString)));
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
    this->setWindowTitle(QString::fromLocal8Bit("开启接入终端自动同步Domain功能"));
    this->showNormal();  
}

void StartUpMrAutoDomain::on_pushButton_clicked()
{
    this->m_req_pro = this->ui->lineEdit_req_pro->text();
    this->m_sumad_req->GetReqProNumber(m_req_pro);
    this->m_sumad_req->StartUpMrAutoDomain([&](bool success, QMap<QString, int>open_domain)
    {
       if(success)
       {
           int req_pro = open_domain["pro"];
           int req_ec = open_domain["ec"];
           this->ui->lineEdit_rep_pro->setText(QString::number(req_pro));
           this->ui->lineEdit_rep_wc->setText(QString::number(req_ec));
           if(open_domain["ec"] == 0)
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

void StartUpMrAutoDomain::on_lineEdit_req_pro_textChanged(const QString &arg1)
{
    this->ui->textEdit_log->append(QString::fromLocal8Bit("您输入了:" ) + arg1);
}

void StartUpMrAutoDomain::onProNum(QString pro_num)
{
    this->ui->textEdit_log->append(QString::fromLocal8Bit("开始发送请求，终端编号为：") + pro_num);
}


void StartUpMrAutoDomain::onRequestFinish(QString finish_info)
{
    this->ui->textEdit_log->append(QString::fromLocal8Bit("发送请求完成,请求url地址为:") + finish_info);
}

void StartUpMrAutoDomain::onRequestSuccess(QString succ_info)
{
    this->ui->textEdit_log->append(succ_info);
}

void StartUpMrAutoDomain::onRequestFail(QString err_info)
{
    this->ui->textEdit_log->append(err_info);
}

