#include "restartmr.h"
#include "ui_restartmr.h"

RestartMr::RestartMr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestartMr), m_restart_specmr(new HttpClient::RestartSpecMrHttpReqest)
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
}

void RestartMr::RestartMrInit()
{
    this->setWindowIcon(QIcon(":/startup.png"));
    this->setWindowTitle(QString::fromLocal8Bit("重启指定终端"));
    this->showNormal();
}

void RestartMr::on_pushButton_clicked()
{
    this->m_req_pro = this->ui->lineEdit_req_pro->text();
    this->m_req_id = this->ui->lineEdit_req_mi->text();

    this->m_restart_specmr->GetReqestDataFromUI(m_req_pro, m_req_id);
    this->m_restart_specmr->RestartSpecMr([&](bool success, QMap<QString, int>restart_specmr)
    {
       if(success)
       {
           int req_pro = restart_specmr["pro"];
           int req_ec = restart_specmr["ec"];
           this->ui->lineEdit_rep_pro->setText(QString::number(req_pro));
           this->ui->lineEdit_rep_status->setText(QString::number(req_ec));
           if(restart_specmr["ec"] == 0)
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("重启指定终端成功，终端编号为:") + QString::number(req_pro));
           }
           else
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("重启指定终端失败, 终端编号为:") + QString::number(req_pro));
           }
       }
    });
}
