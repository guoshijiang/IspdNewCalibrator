#include "controlserverstutasinfo.h"
#include "ui_controlserverstutasinfo.h"

ControlServerStutasInfo::ControlServerStutasInfo(QWidget *parent) : QDialog(parent),
    ui(new Ui::ControlServerStutasInfo), m_handle_err(new Common::HandleError)
{
    p_area = new QScrollArea();
    p_widget = new QWidget();
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
    if(m_label != NULL)
    {
        delete m_label;
        m_label = NULL;
    }

    if(m_ledit != NULL)
    {
        delete m_ledit;
        m_ledit = NULL;
    }

    if(m_layout != NULL)
    {
        delete m_layout;
        m_layout = NULL;
    }

    if(m_vbox_layout != NULL)
    {
        delete m_vbox_layout;
        m_vbox_layout =NULL;
    }

    if(m_main_layout != NULL)
    {
        delete m_main_layout;
        m_main_layout = NULL;
    }

    if(m_page != NULL)
    {
        delete m_page;
        m_page = NULL;
    }

    if(p_widget != NULL)
    {
        delete p_widget;
        p_widget = NULL;
    }

    if(p_area != NULL)
    {
        delete p_area;
        p_area = NULL;
    }

    if(this->m_udp_con_svr_info != NULL)
    {
        delete this->m_udp_con_svr_info;
        this->m_udp_con_svr_info = NULL;
    }    

    if(this->m_handle_err != NULL)
    {
        delete this->m_handle_err;
        this->m_handle_err = NULL;
    }
    delete ui;
}

void ControlServerStutasInfo::CloseApp()
{
    this->close();
}

void ControlServerStutasInfo::CSSInfoStart()
{
    this->setWindowIcon(QIcon(":/UDP.png"));
    this->setWindowTitle(QString::fromLocal8Bit("获取UDP控制服务状态信息"));
    this->getUdpControl();
    this->showNormal();
}

void ControlServerStutasInfo::getUdpControl()
{
    this->m_udp_con_svr_info = new HttpClient::UdpControlServerInfoHttpReq(m_udp_control_url);
    this->m_udp_con_svr_info->UdpControlServerInfo([&](bool success, QMap<QString, QVariant> udp_svr_data)
    {
       if(success)
       {
           this->ui->groupBox_mmr->setLayout(m_main_layout);
           if(udp_svr_data["ec"].toInt() == 0)
           {
               QString succ_log = QString::fromLocal8Bit("读取成功");
               this->ui->lineEdit_stutas->setText(succ_log);
               this->ui->lineEdit_host->setText(udp_svr_data["host"].toString());
               QJsonValue json_value = udp_svr_data["ipv4"].toJsonValue();
               QJsonArray json_arr= json_value.toArray();
               for(int i = 0; i < json_arr.size(); i++)
               {
                   this->ui->textEdit_ip->append(json_arr.at(i).toString());
               }
               if(udp_svr_data["sync"].toInt() == 0)
               {
                   this->ui->lineEdit_sync->setText(UTF8BIT("已经关闭自动同步"));
               }
               else if(udp_svr_data["sync"].toInt() == 1)
               {
                   this->ui->lineEdit_sync->setText(UTF8BIT("自动同步已开启"));
               }
               else
               {
                   this->ui->lineEdit_sync->setText(UTF8BIT("未知类型"));
               }
               if(udp_svr_data["reset"].toInt() == 0)
               {
                   this->ui->lineEdit_reset->setText(UTF8BIT("关闭终端自动恢复出厂设置"));
               }
               else if(udp_svr_data["reset"].toInt() == 1)
               {
                   this->ui->lineEdit_reset->setText(UTF8BIT("开启终端自动恢复出厂设置"));
               }
               else
               {
                   this->ui->lineEdit_reset->setText(UTF8BIT("未知类型"));
               }
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
                            if(value.isDouble())
                            {
                                int id_str = value.toDouble();
                                this->UdpSvrDataUi(UTF8BIT("终端id"), QString::number(id_str));
                            }
                        }
                        if(json_obj.contains("ip"))
                        {
                            QJsonValue value = json_obj.value("ip");
                            if(value.isString())
                            {
                                QString ip_str = value.toString();
                                this->UdpSvrDataUi(UTF8BIT("终端ip"), ip_str);
                            }
                        }
                        if(json_obj.contains("sync"))
                        {
                            QJsonValue value = json_obj.value("sync");
                            if(value.isDouble())
                            {
                                int sync_mr = value.toDouble();
                                if(sync_mr == 0)
                                {
                                    this->UdpSvrDataUi(UTF8BIT("终端同步信息"), UTF8BIT("已同步"));
                                }
                                if(sync_mr == 1)
                                {
                                    this->UdpSvrDataUi(UTF8BIT("终端同步信息"), UTF8BIT("未同步"));
                                }
                            }
                        }
                        if(json_obj.contains("reset"))
                        {
                            QJsonValue value = json_obj.value("reset");
                            if(value.isDouble())
                            {
                                int reset_mr = value.toDouble();
                                if(reset_mr == 0)
                                {
                                    this->UdpSvrDataUi(UTF8BIT("终端恢复出厂设置信息"), UTF8BIT("已恢复"));
                                }

                                if(reset_mr == 1)
                                {
                                    this->UdpSvrDataUi(UTF8BIT("终端恢复出厂设置信息"), UTF8BIT("未恢复"));
                                }
                            }
                        }
                    }
               }
           }
           else
           {
               this->m_handle_err->HandleHttpReqError(udp_svr_data["ec"].toInt());
               QString log = this->m_handle_err->m_http_req_error;
               this->ui->lineEdit_stutas->setText(UTF8BIT("读取失败") + log);
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
