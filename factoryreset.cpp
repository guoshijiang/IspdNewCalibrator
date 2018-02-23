#include "factoryreset.h"
#include "ui_factoryreset.h"

FactoryReset::FactoryReset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FactoryReset), m_handle_err(new Common::HandleError)
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

    if(this->m_handle_err != NULL)
    {
        delete this->m_handle_err;
        this->m_handle_err = NULL;
    }
}

void FactoryReset::CloseApp()
{
    this->close();
}

void FactoryReset::FactoryResetInit()
{
    this->setWindowIcon(QIcon(":/resetup.png"));
    this->setWindowTitle(QString::fromLocal8Bit("恢复指定终端出厂设置"));
    this->showNormal();
    for(int i = 0; i < m_mid.size(); i++)
    {
        this->ui->comboBox_id->addItem(QString::number(this->m_mid.at(i)));
    }
}

void FactoryReset::on_pushButton_clicked()
{
    this->m_req_id = this->ui->comboBox_id->currentText();
    this->m_factory_reset = new HttpClient::FactoryResetHttpReqest(m_req_id, m_factory_reset_url);
    this->m_factory_reset->FactoryReset([&](bool success, QMap<QString, int>sync_domain)
    {
       if(success)
       {
           if(sync_domain["ec"] == 0)
           {
               QString succ_log = QString::fromLocal8Bit("恢复指定终端出厂设置成功，终端ID为:") + this->m_req_id;
               QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_END;
               this->ui->textEdit_status->append(succ_log_msg);
           }
           else
           {
               this->m_handle_err->HandleHttpReqError(sync_domain["ec"]);
               QString log = this->m_handle_err->m_http_req_error;
               QString id_str = MESSAGE_RED + this->m_req_id + MESSAGE_END;
               this->ui->textEdit_status->append(log + id_str);
           }
       }
    });
}
