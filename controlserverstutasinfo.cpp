#include "controlserverstutasinfo.h"
#include "ui_controlserverstutasinfo.h"

ControlServerStutasInfo::ControlServerStutasInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlServerStutasInfo), m_udp_con_svr_info(new HttpClient::UdpControlServerInfoHttpReq)
{
    QScrollArea *p_area = new QScrollArea();
    QWidget *p_widget = new QWidget();
    p_widget->setStyleSheet("QWidget" "{background:white;}");
    m_vbox_layout = new QVBoxLayout();
    m_vbox_layout->addSpacerItem(new QSpacerItem(100, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    p_widget->setLayout(m_vbox_layout);
    p_area->setWidget(p_widget);
    p_area->setWidgetResizable(true);
    m_main_layout = new QVBoxLayout();
    m_main_layout->addWidget(p_area);
    ui->setupUi(this);
}

ControlServerStutasInfo::~ControlServerStutasInfo()
{

    if(this->m_udp_con_svr_info != NULL)
    {
        delete this->m_udp_con_svr_info;
        this->m_udp_con_svr_info = NULL;
    }
    delete ui;
}

void ControlServerStutasInfo::CSSInfoStart()
{
    this->setWindowIcon(QIcon(":/UDP.png"));
    this->setWindowTitle(QString::fromLocal8Bit("获取UDP控制服务状态信息"));
    this->showNormal();
}

void ControlServerStutasInfo::on_lineEdit_pro_textChanged(const QString &arg1)
{
    this->ui->textEdit_log->append(QString::fromLocal8Bit("您输入了") + arg1);
}

void ControlServerStutasInfo::on_pushButton_clicked()
{
    this->m_req_pro = this->ui->lineEdit_pro->text();
    this->m_udp_con_svr_info->GetReqProNumber(m_req_pro);
    this->m_udp_con_svr_info->UdpControlServerInfo([&](bool success, QMap<QString, QVariant> udp_svr_data)
    {
       if(success)
       {
           this->ui->groupBox_mmr->setLayout(m_main_layout);
           this->ui->lineEdit_pro_2->setText(udp_svr_data["pro"].toString());
           this->ui->lineEdit_stutas->setText(QString::number(udp_svr_data["ec"].toInt()));
           this->ui->lineEdit_ip->setText(udp_svr_data["host"].toString());
           this->ui->lineEdit_sync->setText(QString::number(udp_svr_data["sync"].toInt()));
           this->ui->lineEdit_reset->setText(QString::number(udp_svr_data["reset"].toInt()));
           QJsonValue json_vale = udp_svr_data["mr"].toJsonValue();
           if(json_vale.isArray())
           {
                QJsonArray json_array = json_vale.toArray();
                for(int i = 0; i < json_array.size(); i++)
                {
                    QJsonObject json_obj = json_array.at(i).toObject();
                    if(json_obj.contains("id"))
                    {
                        QJsonValue value = json_obj.value("id");
                        if(value.isString())
                        {
                            QString id_str = value.toString();
                            this->UdpSvrDataUi("id", id_str);
                        }
                    }

                    if(json_obj.contains("ip"))
                    {
                        QJsonValue value = json_obj.value("ip");
                        if(value.isString())
                        {
                            QString ip_str = value.toString();
                            this->UdpSvrDataUi("ip", ip_str);
                        }
                    }
                }
           }
       }
    });
}

void ControlServerStutasInfo::UdpSvrDataUi(QString key_name, QString value_name)
{
    this->m_page = new QWidget;
    this->m_layout = new QGridLayout(m_page);

    this->m_label = new QLabel;
    this->m_label->setText(key_name);
    this->m_label->setFont(font());

    this->m_ledit = new QLineEdit;
    this->m_ledit->setFont(font());
    this->m_ledit->setText(value_name);

    this->m_layout->addWidget(m_label, 0, 1);
    this->m_layout->addWidget(m_ledit, 0, 2);

    this->m_page->setLayout(m_layout);
    this->m_vbox_layout->insertWidget(m_vbox_layout->count()-1, m_page);
}
