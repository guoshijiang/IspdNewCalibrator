#include "factoryreset.h"
#include "ui_factoryreset.h"

FactoryReset::FactoryReset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FactoryReset), m_factory_reset(new HttpClient::FactoryResetHttpReqest)
{
    ui->setupUi(this);
}

FactoryReset::~FactoryReset()
{
    delete ui;
    if(this->m_factory_reset != NULL)
    {
        delete this->m_factory_reset;
        this->m_factory_reset = NULL;
    }
}

void FactoryReset::FactoryResetInit()
{
    this->setWindowIcon(QIcon(":/resetup.png"));
    this->setWindowTitle(QString::fromLocal8Bit("恢复指定终端出厂设置"));
    this->showNormal();
}

void FactoryReset::on_pushButton_clicked()
{
    this->m_req_pro = this->ui->lineEdit_req_pro->text();
    this->m_req_id = this->ui->lineEdit_req_mid->text();

    this->m_factory_reset->GetReqestDataFromUI(m_req_pro, m_req_id);
    this->m_factory_reset->FactoryReset([&](bool success, QMap<QString, int>sync_domain)
    {
       if(success)
       {
           int req_pro = sync_domain["pro"];
           int req_ec = sync_domain["ec"];
           this->ui->lineEdit_rep_pro->setText(QString::number(req_pro));
           this->ui->lineEdit_rep_status->setText(QString::number(req_ec));
           if(sync_domain["ec"] == 0)
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("恢复指定终端出厂设置成功，终端编号为:") + QString::number(req_pro));
           }
           else
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("恢复指定终端出厂设置失败, 终端编号为:") + QString::number(req_pro));
           }
       }
    });
}
