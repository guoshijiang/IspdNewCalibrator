#include "restartmr.h"
#include "ui_restartmr.h"

RestartMr::RestartMr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestartMr)
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
           if(restart_specmr["ec"] == 0)
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("重启指定终端成功，终端ID为:") + this->m_req_id);
           }
           else
           {
               this->ui->textEdit_status->append(QString::fromLocal8Bit("重启指定终端失败, 终端ID为:") + this->m_req_id);
           }
       }
    });
}
