#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QDebug>
#include <iostream>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <iostream>
#include <QScrollArea>

#include "serialport.h"
#include "protoserialport.h"

ServerMrData svrmr_data[1024] = {{0}};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_version(new Version),
    m_question(new Question), m_doc(new Document),
    m_port_set(new SerialPortSet), m_serial_port(new SerialPort),
    m_css_info(new ControlServerStutasInfo),
    m_rsc_config(new ReadSpecificConfig), m_write_spec_config(new WriteSpecConfig),
    m_start_mr_auto_domain(new StartUpMrAutoDomain), m_start_mr_auto_freset(new StartUpMrAutoFactoryReset),
    m_sync_spec_mr_domain(new SyncSpecMrDomain), m_restart_spec_mr(new RestartMr),
    m_factory_reset(new FactoryReset), m_mr_result(new HttpClient::GetMrReslutHttpReqest),
    m_sport_wr(new SerialPortWriteRead), m_udp_con_svr_info(new HttpClient::UdpControlServerInfoHttpReq),
    m_handle_err(new HandleError), m_encrypt(new Encrypt)
{
    ui->setupUi(this);
    m_bc_work = new BroadCastWorker();
    m_thread = new QThread();
    m_bc_work->moveToThread(m_thread);
    connect(this, &MainWindow::startWork, m_bc_work, &BroadCastWorker::UdpBoardCast);
    connect(m_bc_work, SIGNAL(sendServerBoardCastMsg(QByteArray, QString)), this, SLOT(onSeverBoardCastMsg(QByteArray, QString)));

    this->ui->groupBox_record->setStyleSheet("QGroupBox {background:#e7a16b}");
    this->ui->comboBox_com->setStyleSheet("QComboBox {background:#e7a16b}");
    this->ui->lineEdit_id->setStyleSheet("QLineEdit {background:#e7a16b}");
    this->ui->record_status->setStyleSheet("QLabel {background:#e7a16b}");

    this->fontSet();  //界面字体初始化

    //菜单项初始化
    this->ui->action_read_speconfig->setDisabled(true);
    this->ui->action_reset_specmr->setDisabled(true);
    this->ui->action_reset_cch->setDisabled(true);
    this->ui->action_open_close_reset->setDisabled(true);
    this->ui->action_open_close_domain->setDisabled(true);
    this->ui->action_udp_server_info->setDisabled(true);

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            this->m_com_list << serial.portName();
            serial.close();
        }
    }
    connect(&m_spt, SIGNAL(readyRead()), this, SLOT(onSerialPortRead()));


#if 0
    this->m_com_list.clear();
    for (int i=0; i < m_serial_port->m_set_len; i++)
    {
        m_serial_port->getSerialPortList(i, "value");
        m_com_list << this->m_serial_port->m_comm_result;
    }
    qDebug() << m_com_list;
#endif

}

void MainWindow::fontSet()
{
    QPalette pet;
    pet.setColor(QPalette::WindowText,Qt::darkCyan);

    QPalette petj;
    petj.setColor(QPalette::WindowText,Qt::darkYellow);

    this->ui->label_bq->setPalette(pet);
    this->ui->label_cyl->setPalette(pet);
    this->ui->label_bbh->setPalette(pet);

    this->ui->label_xl1->setPalette(pet);
    this->ui->label_xl2->setPalette(pet);
    this->ui->label_xl3->setPalette(pet);
    this->ui->label_xlpj->setPalette(pet);

    this->ui->label_jsd->setPalette(pet);
    this->ui->label_tly->setPalette(pet);
    this->ui->label_ppg->setPalette(pet);
    this->ui->label_dl->setPalette(pet);
    this->ui->label_cdzt->setPalette(pet);

    this->ui->label_ck_jg->setPalette(petj);
    this->ui->label_jz_jg->setPalette(petj);
    this->ui->label_dbsj->setPalette(petj);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(this->m_version != NULL)
    {
        delete this->m_version;
        this->m_version = NULL;
    }

    if(this->m_question != NULL)
    {
        delete this->m_question;
        this->m_question = NULL;
    }

    if(this->m_doc != NULL)
    {
        delete this->m_doc;
        this->m_doc = NULL;
    }

    if(this->m_port_set != NULL)
    {
        delete this->m_port_set;
        this->m_port_set = NULL;
    }
    if(this->m_serial_port != NULL)
    {
        delete this->m_serial_port;
        this->m_serial_port = NULL;
    }
    if(this->m_css_info != NULL)
    {
       delete this->m_css_info;
       this->m_css_info = NULL;
    }

    if(this->m_rsc_config != NULL)
    {
        delete this->m_rsc_config;
        this->m_rsc_config = NULL;
    }

    if(this->m_write_spec_config != NULL)
    {
        delete this->m_write_spec_config;
        this->m_write_spec_config = NULL;
    }

    if(this->m_start_mr_auto_domain != NULL)
    {
        delete this->m_start_mr_auto_domain;
        this->m_start_mr_auto_domain = NULL;
    }

    if(this->m_start_mr_auto_freset != NULL)
    {
        delete this->m_start_mr_auto_freset;
        this->m_start_mr_auto_freset = NULL;
    }

    if(this->m_sync_spec_mr_domain != NULL)
    {
        delete this->m_sync_spec_mr_domain;
        this->m_sync_spec_mr_domain = NULL;
    }

    if(this->m_restart_spec_mr != NULL)
    {
        delete this->m_restart_spec_mr;
        this->m_restart_spec_mr = NULL;
    }

    if(this->m_factory_reset != NULL)
    {
        delete this->m_factory_reset;
        this->m_factory_reset = NULL;
    }

    if(this->m_mr_result != NULL)
    {
        delete this->m_mr_result;
        this->m_mr_result = NULL;
    }

    if(this->m_sport_wr != NULL)
    {
        delete this->m_sport_wr;
        this->m_sport_wr = NULL;
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

    if(this->m_encrypt != NULL)
    {
        delete this->m_encrypt;
        this->m_encrypt = NULL;
    }

    this->m_bc_work->setStop(true);
    this->m_thread->quit();
    this->m_thread->wait();
}

void MainWindow::onSeverBoardCastMsg(const QByteArray &msg, QString serverIP)
{ 
    M_IP = serverIP.mid(7);
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
    this->ui->textEdit_log->append(QString::fromLocal8Bit("广播源的IP地址是")+M_IP + time_str);
    QString httpReqDataUrl = HTTP_PREDIX + M_IP + ":" + HTTP_REQEST_PORT + UDP_DATA;
    QString setReqUrl = HTTP_PREDIX + M_IP + ":" + HTTP_REQEST_PORT + UDP_CONTROL;
    if(this->m_mr_result->M_RUL.isEmpty())
    {
        this->m_mr_result->httpRequestUrl(httpReqDataUrl);
    }

    if(this->m_udp_con_svr_info->M_UDP_COMTROL_RUL.isEmpty())
    {
        this->m_udp_con_svr_info->M_UDP_COMTROL_RUL = setReqUrl;
        this->getServerConfigInfo();
    }

    if(this->m_rsc_config->m_url.isEmpty())
    {
        this->m_rsc_config->m_url = setReqUrl;
    }

    if(this->m_restart_spec_mr->m_restart_url.isEmpty())
    {
        this->m_restart_spec_mr->m_restart_url = setReqUrl;
    }

    if(this->m_factory_reset->m_factory_reset_url.isEmpty())
    {
        this->m_factory_reset->m_factory_reset_url = setReqUrl;
    }

    if(this->m_start_mr_auto_freset->m_open_close_url.isEmpty())
    {
        this->m_start_mr_auto_freset->m_open_close_url = setReqUrl;
    }

    if(this->m_start_mr_auto_domain->m_sync_rul.isEmpty())
    {
        this->m_start_mr_auto_domain->m_sync_rul = setReqUrl;
    }

    if(this->m_css_info->m_udp_control_url.isEmpty())
    {
        this->m_css_info->m_udp_control_url = setReqUrl;
    }
}

void MainWindow::VersionSlot()
{
    this->m_version->ver_start();
}

void MainWindow::QuestionSlot()
{
    this->m_question->qus_start();
}

void MainWindow::DocSlot()
{
    this->m_doc->doc_start();
}

void MainWindow::SerialPortSlot()
{
    this->m_port_set->serPort_start();
}

void MainWindow::CloseApp()
{
    this->close();
    this->m_css_info->CloseApp();
    this->m_doc->CloseApp();
    this->m_factory_reset->CloseApp();
    this->m_question->CloseApp();
    this->m_rsc_config->CloseApp();
    this->m_restart_spec_mr->CloseApp();
    this->m_port_set->close();
    this->m_start_mr_auto_domain->CloseApp();
    this->m_start_mr_auto_freset->CloseApp();
    this->m_sync_spec_mr_domain->CloseApp();
    this->m_version->CloseApp();
    this->m_write_spec_config->CloseApp();
}

void MainWindow::ControlServerStutasInfoSlot()
{
    this->m_css_info->CSSInfoStart();
}

void MainWindow::ReadSpecificConfigSlot()
{
    this->m_rsc_config->ReadSpecConfig_start();
}

void MainWindow::WriteSpecConfigSlot()
{
    this->m_write_spec_config->WriteSpecConfig_start();
}

void MainWindow::StartUpMrCloseAutoDomainSlot()
{
    this->m_start_mr_auto_domain->StartUpMrAutoDomain_start();
}

void MainWindow::StartUpMrCloseAutoFactoryResetSlot()
{
    this->m_start_mr_auto_freset->StartUpMrAutoFactoryReset_start();
}

void MainWindow::SyncSpecMrDomainSlot()
{
    this->m_sync_spec_mr_domain->SyncSpecMrDomainInit();
}

void MainWindow::FactoryResetSlot()
{
    this->m_factory_reset->FactoryResetInit();
}

void MainWindow::RestartMrSlot()
{
    this->m_restart_spec_mr->RestartMrInit();
}

void MainWindow::onRecordSuccLogMsg(const QString & msg)
{
    QString succ_log_msg = MESSAGE_SUC + msg + MESSAGE_CED;
    qDebug() << succ_log_msg;
}

void MainWindow::onRecordFailMsg(const QString & msg)
{
    QString fail_log_msg = MESSAGE_RED + msg + MESSAGE_END;
    qDebug() << fail_log_msg;
}

void MainWindow::getServerConfigInfo()
{
    this->m_udp_con_svr_info->UdpControlServerInfo([&](bool success, QMap<QString, QVariant> udp_svr_data)
    {
       if(success)
       {
           if(udp_svr_data["ec"].toInt() == 0)
           {
               this->m_start_mr_auto_domain->m_sync = udp_svr_data["sync"].toInt();
               this->m_start_mr_auto_freset->m_reset = udp_svr_data["reset"].toInt();
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
                                this->m_factory_reset->m_mid.append(id_str);
                                this->m_restart_spec_mr->m_mid.append(id_str);
                                this->m_rsc_config->m_list_mid.append(id_str);
                            }
                        }
                    }
               }
               //菜单项恢复
               this->ui->action_read_speconfig->setDisabled(false);
               this->ui->action_reset_specmr->setDisabled(false);
               this->ui->action_reset_cch->setDisabled(false);
               this->ui->action_open_close_reset->setDisabled(false);
               this->ui->action_open_close_domain->setDisabled(false);
               this->ui->action_udp_server_info->setDisabled(false);
               QString succ_log = UTF8BIT("获取服务端控制数据成功");
               this->ui->textEdit_log->append(succ_log);
           }
           else
           {
               QString log_err = UTF8BIT("获取服务端控制数据失败");
               QString fail_log_msg = MESSAGE_RED + log_err + MESSAGE_END;
               this->m_handle_err->HandleHttpReqError(udp_svr_data["ec"].toInt());
               QString fail_log = this->m_handle_err->m_http_req_error;
               this->ui->textEdit_log->append(fail_log_msg + fail_log);
               QMessageBox::information(this, UTF8BIT("信息"),fail_log_msg + fail_log);
           }
       }
    });
}

void MainWindow::start()
{
    if(m_thread->isRunning())
    {
       return ;
    }
    m_thread->start();
    emit startWork();

    this->setWindowTitle(QString::fromLocal8Bit("超版板上位机测试软件"));
    this->showNormal();
    connect(this->ui->action_version, &QAction::triggered, this, &MainWindow::VersionSlot);
    connect(this->ui->action_question, &QAction::triggered, this, &MainWindow::QuestionSlot);
    connect(this->ui->action_doc, &QAction::triggered, this, &MainWindow::DocSlot);
    connect(this->ui->action_serialport_set, &QAction::triggered, this, &MainWindow::SerialPortSlot);
    connect(this->ui->action_eixit, &QAction::triggered, this, &MainWindow::CloseApp);
    connect(this->ui->action_open_close_domain, &QAction::triggered, this, &MainWindow::StartUpMrCloseAutoDomainSlot);
    connect(this->ui->action_open_close_reset, &QAction::triggered, this, &MainWindow::StartUpMrCloseAutoFactoryResetSlot);
    connect(this->ui->action_specmr_domain, &QAction::triggered, this, &MainWindow::SyncSpecMrDomainSlot);
    connect(this->ui->action_reset_cch, &QAction::triggered, this, &MainWindow::FactoryResetSlot);
    connect(this->ui->action_reset_specmr, &QAction::triggered, this, &MainWindow::RestartMrSlot);
    connect(this->ui->action_write_speconfig, &QAction::triggered, this, &MainWindow::WriteSpecConfigSlot);
    connect(this->ui->action_read_speconfig, &QAction::triggered, this, &MainWindow::ReadSpecificConfigSlot);
    connect(this->ui->action_udp_server_info, &QAction::triggered, this, &MainWindow::ControlServerStutasInfoSlot);
    connect(m_sport_wr, SIGNAL(sendRecordSuccLogMessage(QString)), this, SLOT(onRecordSuccLogMsg(QString)));
    connect(m_sport_wr, SIGNAL(sendRecordFailLogMessage(QString)), this, SLOT(onRecordFailMsg(QString)));

    for(int j = 0; j < m_com_list.size(); j++)
    {  
        this->ui->comboBox_com->addItem(m_com_list[j]);
    }
}

void MainWindow::UIinit()
{
    this->ui->lineEdit_rid->clear();
    this->ui->lineEdit_hz->clear();
    this->ui->lineEdit_version->clear();

    this->ui->lineEdit_heart_one->clear();
    this->ui->lineEdit_heart_two->clear();
    this->ui->lineEdit_heart_three->clear();
    this->ui->lineEdit_heart_avg->clear();

    this->ui->lineEdit_ck_a->clear();
    this->ui->lineEdit_ck_power->clear();
    this->ui->lineEdit_ck_ppg->clear();
    this->ui->lineEdit_ck_tly->clear();

    this->ui->lineEdit_jz_a->clear();
    this->ui->lineEdit_jz_tly->clear();
    this->ui->lineEdit_jz_ppg->clear();
    this->ui->lineEdit_jz_power->clear();

    this->ui->label_compare_a->clear();
    this->ui->label_compare_gyo->clear();
    this->ui->label_compare_power->clear();
    this->ui->label_compare_ppg->clear();

    this->ui->lineEdit_charge->clear();

    this->ui->textEdit_log->clear();
}

void MainWindow::onSerialPortRead()
{
    m_spt.waitForReadyRead(100);
    QByteArray buf = m_spt.readAll();
    if (buf.length() < 5)
    {
        return ;
    }

    qDebug() << QString::fromLocal8Bit("护腿板发过来的数据长度size =") << buf.size();
    qDebug() << QString::fromLocal8Bit("护腿板发过来的数据recv buf = ") << buf;

    protoserialport::REP_TYPE type = static_cast<protoserialport::REP_TYPE>(buf.at(2));
    protoserialport::ResPkg<protoserialport::ResResult> res(buf.data(), buf.length());
    switch (type)
    {
        case protoserialport::REP_ERR:
        {
            if (protoserialport::ERR_CRC == res.getData().getRes())
            {
                m_log_error = QString::fromLocal8Bit("CRC出错");
                m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
                QMessageBox::information(this, UTF8BIT("信息"), m_handle_log_error);
                qCritical() << m_log_error;
            }
            else if(protoserialport::ERR_FRM == res.getData().getRes())
            {
                m_log_error = QString::fromLocal8Bit("帧类型出错");
                m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
                QMessageBox::information(this, UTF8BIT("信息"), m_handle_log_error);
                qCritical() << m_log_error;
            }
            else
            {
                m_log_error = QString::fromLocal8Bit("不知类型错误");
                m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
                QMessageBox::information(this, UTF8BIT("信息"), m_handle_log_error);
                qCritical() << m_log_error;
            }
        } break;

        case protoserialport::REP_WR_RES:
        {
            if (protoserialport::RES_SUCCESS == res.getData().getRes())
            {
                m_log_succ =  QString::fromLocal8Bit("配置成功");
                qInfo() << m_log_succ;
            }
            else if (protoserialport::RES_FAILURE == res.getData().getRes())
            {
                m_log_error = QString::fromLocal8Bit("配置失败");
                m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
                QMessageBox::information(this, UTF8BIT("信息"), m_handle_log_error);
                qCritical() << m_log_error;
            }
            else
            {
                m_log_error = QString::fromLocal8Bit("不知类型错误");
                m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
                QMessageBox::information(this, UTF8BIT("信息"), m_handle_log_error);
                qCritical() << m_log_error;
            }
        } break;

        //获取心率
        case protoserialport::REP_ISPD_HR:
        {
            protoserialport::ResPkg<protoserialport::ResHR2> reshr(buf.data(), buf.length());
            if (!reshr.isValid())
            {
                m_log_error = QString::fromLocal8Bit("收到CRC错误");
                m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
                QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
                qCritical() << m_log_error;
                break;
            }

            quint8 hr = reshr.getData().get_hr();

            QString hr1 = ui->lineEdit_heart_one->text();
            if (hr1.isEmpty())
            {
                ui->lineEdit_heart_one->setText(QString::number(hr));
                break;
            }

            QString hr2 = ui->lineEdit_heart_two->text();
            if (hr2.isEmpty())
            {
                ui->lineEdit_heart_two->setText(QString::number(hr));
                break;
            }

            QString hr3 = ui->lineEdit_heart_three->text();
            if (hr3.isEmpty())
            {
                ui->lineEdit_heart_three->setText(QString::number(hr));
            }
            hr3 = ui->lineEdit_heart_three->text();

            quint8 hr_avg = (hr1.toInt() + hr2.toInt() + hr3.toInt()) / 3;
            ui->lineEdit_heart_avg->setText(QString::number(hr_avg));
        } break;

        case protoserialport::REP_ISPD_SERSOR:
        {
            protoserialport::ResPkg<protoserialport::Sensor> sensor(buf.data(), buf.length());
            if (!sensor.isValid())
            {
                m_log_error = QString::fromLocal8Bit("收到CRC错误");
                m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
                QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
                qCritical() << m_log_error;
                break;
            }

            qint16 ac_x = sensor.getData().get_ac_x();
            qint16 ac_y = sensor.getData().get_ac_y();
            qint16 ac_z = sensor.getData().get_ac_z();
            qint16 gy_x = sensor.getData().get_gy_x();
            qint16 gy_y = sensor.getData().get_gy_y();
            qint16 gy_z = sensor.getData().get_gy_z();
            quint16 hb = sensor.getData().get_hb();
            quint8 pw = sensor.getData().get_pw() & 0x7F;
            quint8 is_charge = sensor.getData().get_pw() & 0x80;
            QString is_charge_str = is_charge == 1 ?  QString::fromLocal8Bit("正在充电") :  QString::fromLocal8Bit("未充电");

            QString ac_pt = "(" + QString::number(ac_x) + ","
                    + QString::number(ac_y) + ","
                    + QString::number(ac_z) + ")";

            QString gy_pt = "(" + QString::number(gy_x) + ","
                    + QString::number(gy_y) + ","
                    + QString::number(gy_z) + ")";
            this->ui->lineEdit_ck_a->setText(ac_pt);
            this->ui->lineEdit_ck_tly->setText(gy_pt);
            this->ui->lineEdit_ck_ppg->setText(QString::number(hb));
            this->ui->lineEdit_ck_power->setText(QString::number(pw));
            this->ui->lineEdit_charge->setText(is_charge_str);
        } break;

        //一键获取
        case protoserialport::REP_ISPD_ALL:
        {
            protoserialport::ResPkg<protoserialport::ResAll> resall(buf.data(), buf.length());
            if (!resall.isValid())
            {
                m_log_error = QString::fromLocal8Bit("收到CRC错误");
                m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
                QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
                qCritical() << m_log_error;
                break;
            }
            qint32 ispd_encryt_id = resall.getData().getID();
            this->m_mr_result->getIDFromUI(ispd_encryt_id);
            QString ispd_encryt_strid = this->m_encrypt->encrypt_ispd_id(ispd_encryt_id);
            this->ui->lineEdit_rid->setText(ispd_encryt_strid);
            this->ui->lineEdit_hz->setText(QString::number(resall.getData().getHZ()));
            QString ispd_version = QString::fromLocal8Bit("固件") +  QString("%1").arg(resall.getData().get_ispd_version(),3,10,QChar('0'));
            QString uwb_version = QString::fromLocal8Bit("透传") + QString("%1").arg(resall.getData().get_uwb_version(),3,10,QChar('0'));
            QString sp_version = QString::fromLocal8Bit("协议") + QString("%1").arg(resall.getData().get_sp_version(),3,10,QChar('0'));
            this->ui->lineEdit_version->setText(QString::fromLocal8Bit("v2.0.0"));
        } break;
    }
}
int MainWindow::OpenPort()
{
    if(!m_spt.isOpen())
    {
        m_spt.setPortName(ui->comboBox_com->currentText());
        m_spt.setBaudRate(230400);
        m_spt.setParity(QSerialPort::NoParity);
        m_spt.setDataBits(QSerialPort::Data8);
        m_spt.setStopBits(QSerialPort::OneStop);
        m_spt.setFlowControl(QSerialPort::NoFlowControl);
        if (!m_spt.open(QIODevice::ReadWrite))
        {
            qCritical() << m_spt.error();
            m_log_error = QString::fromLocal8Bit("打开串口失败");
            m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
            QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
            qCritical() << m_log_error;
            return -1;
        }
        m_log_succ = QString::fromLocal8Bit("打开串口成功");
        qInfo() << m_log_succ;
    }
    return 0;
}

int MainWindow::EnterConfig()
{
    QByteArray pre_array;
    quint8 pre_write = 0xFF;
    quint8 cnt = 15;
    while (cnt--)
    {
        pre_array.append(pre_write);
    }

    qint64 cus_w = m_spt.write(pre_array, pre_array.length());
    if(cus_w == -1)
    {
        m_log_error = QString::fromLocal8Bit("发送预配置指令失败,请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }
    bool cus_ok = m_spt.waitForBytesWritten(10);
    if(cus_ok == false)
    {
        m_log_error = QString::fromLocal8Bit("发送预配置指令失败,请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }

    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_MODULE_CUSTOMISE);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    qint64 cus_one_w = m_spt.write(send_cus_pkg.data(), send_cus_pkg.length());
    if(cus_one_w == -1)
    {
        m_log_error = QString::fromLocal8Bit("发送开始配置指令失败，请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }

    bool cus_one_ok = m_spt.waitForBytesWritten(10);
    if(cus_one_ok == false)
    {
        m_log_error = QString::fromLocal8Bit("发送开始配置指令失败，请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }
    emit m_spt.readyRead();
    return 0;
}

int MainWindow::ClosePort()
{
    if(m_spt.isOpen())
    {
        m_spt.setPortName(ui->comboBox_com->currentText());
        m_spt.setBaudRate(230400);
        m_spt.setParity(QSerialPort::NoParity);
        m_spt.setDataBits(QSerialPort::Data8);
        m_spt.setStopBits(QSerialPort::OneStop);
        qInfo() << QString::fromLocal8Bit("串口已关闭");
        m_spt.close();
    }
    return 0;
}

int MainWindow::ExitConfig()
{
    protoserialport::ReqPkg app_pkg(protoserialport::REQ_TYPE::ISPD_MODULE_APPLICATE);
    QByteArray send_cus_app = app_pkg.toBinary();
    qint64 app_w = m_spt.write(send_cus_app.data(), send_cus_app.length());
    if(app_w == -1)
    {
        m_log_error = QString::fromLocal8Bit("发送结束配置指令失败,请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }

    bool app_ok = m_spt.waitForBytesWritten(10);
    if(app_ok == false)
    {
        m_log_error = QString::fromLocal8Bit("发送结束配置指令失败,请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }
    m_spt.readyRead();
    return 0;
}

void MainWindow::on_lineEdit_id_textChanged(QString text)
{
    this->ui->record_status->clear();

    int ret = OpenPort();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("打开串口失败");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
    }
    qInfo() << QString::fromLocal8Bit("打开串口成功");

    m_tim.start();
    while(m_tim.elapsed() < 2000)
    {
        QCoreApplication::processEvents();
    }

    ret = EnterConfig();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("进入配置失败");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
    }
    qInfo() << QString::fromLocal8Bit("进入配置成功");

    m_tim.start();
    while(m_tim.elapsed() < 3000)
    {
        QCoreApplication::processEvents();
    }

    std::string current_sign_id;
    current_sign_id = text.toStdString();
    if (this->ui->lineEdit_id != NULL)
    {
        qint32 ispd_id_dencrypt = this->m_encrypt->dencrypt_ispd_id(current_sign_id);;
        protoserialport::IspdID ispd_id(ispd_id_dencrypt, protoserialport::REQ_TYPE::ISPD_WR_ID);
        protoserialport::ReqMsg<protoserialport::IspdID> res_msg(ispd_id);
        QByteArray send_id = res_msg.toBinary();
        qint64 w_ret = m_spt.write(send_id.data(), send_id.length());
        if(w_ret == -1)
        {
            m_log_error = QString::fromLocal8Bit("发送超级板ID录入指令失败,请重试...！");
            m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
            QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
            qCritical() << m_log_error;
            return ;
        }

        bool w_ok = m_spt.waitForBytesWritten(10);
        if(w_ok == false)
        {
            m_log_error = QString::fromLocal8Bit("发送超级板ID录入指令失败,请重试...！");
            m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
            QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
            qCritical() << m_log_error;
            return ;
        }
    }
    else
    {
        m_log_error = QString::fromLocal8Bit("输入ID为空,请您务必输入ID...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return ;
    }

    m_tim.start();
    while(m_tim.elapsed() < 2000)
    {
        QCoreApplication::processEvents();
    }

    ret = ExitConfig();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("退出配置模式失败");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
    }
    qInfo() << QString::fromLocal8Bit("退出配置模式成功");

    m_tim.start();
    while(m_tim.elapsed() < 2000)
    {
        QCoreApplication::processEvents();
    }

    ret = ClosePort();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("关闭串口失败");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
    }
    qInfo() << QString::fromLocal8Bit("关闭串口成功");
    this->ui->record_status->setText(QString::fromLocal8Bit("录入ID成功"));
}

int MainWindow::GetIdHzVersion()
{
    if(!m_spt.isOpen())
    {
        m_log_error = QString::fromLocal8Bit("串口没有打开");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }

    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_RD_ALL);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    qint64 read_all_w = m_spt.write(send_cus_pkg.data(), send_cus_pkg.length());
    if(read_all_w == -1)
    {
        m_log_error = QString::fromLocal8Bit("一键读取指令发送失败,请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }
    bool read_all_ok = m_spt.waitForBytesWritten(10);
    if(read_all_ok == false)
    {
        m_log_error = QString::fromLocal8Bit("一键读取指令发送失败,请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }
    emit m_spt.readyRead();
    return 0;
}

int MainWindow::ReadHeartRate()
{
    if (!m_spt.isOpen())
    {
        m_log_error = QString::fromLocal8Bit("串口没有打开");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }

    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_RD_HR);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    qint64 ispd_hr_w = m_spt.write(send_cus_pkg.data(), send_cus_pkg.length());
    if(ispd_hr_w == -1)
    {
        m_log_error = QString::fromLocal8Bit("发送读取心率指令失败,请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }
    bool ispd_hr_ok = m_spt.waitForBytesWritten(10);
    if(ispd_hr_ok == false)
    {
        m_log_error = QString::fromLocal8Bit("发送读取心率指令失败,请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }
    emit m_spt.readyRead();
    return 0;
}

int MainWindow::GetSensorData()
{
    if (!m_spt.isOpen())
    {
         m_log_error = QString::fromLocal8Bit("串口没有打开");
         m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
         QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
         qWarning() << m_log_error;
         return -1;
    }
    protoserialport::ReqPkg cus_pkg(protoserialport::REQ_TYPE::ISPD_RD_SENSOR);
    QByteArray send_cus_pkg = cus_pkg.toBinary();
    qint64 ispd_sensor_w = m_spt.write(send_cus_pkg.data(), send_cus_pkg.length());
    if(ispd_sensor_w == -1)
    {
        m_log_error = QString::fromLocal8Bit("发送获取传感器和电量值指令失败,请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }
    bool ispd_sensor_ok = m_spt.waitForBytesWritten(10);
    if(ispd_sensor_ok == false)
    {
        m_log_error = QString::fromLocal8Bit("发送获取传感器和电量值指令失败,请重试...");
        m_handle_log_error = MESSAGE_RED + m_log_error + MESSAGE_END;
        QMessageBox::information(this,QString::fromLocal8Bit("信息"), m_handle_log_error);
        qCritical() << m_log_error;
        return -1;
    }
    emit m_spt.readyRead();
    return 0;
}

void MainWindow::on_pushButton_test_clicked()
{
    this->UIinit();
    m_tim.start();
    while(m_tim.elapsed() < 100)
    {
        QCoreApplication::processEvents();
    }
    this->ui->pushButton_test->setDisabled(true);
    int ret = OpenPort();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("打开串口失败");
        qCritical() << m_log_error;
        this->ui->pushButton_test->setDisabled(false);
        return ;
    }
    this->ui->textEdit_log->append(QString::fromLocal8Bit("打开串口成功"));

    m_tim.start();
    while(m_tim.elapsed() < 2000)
    {
        QCoreApplication::processEvents();
    }

    ret = EnterConfig();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("进入配置失败");
        qCritical() << m_log_error;
        this->ui->pushButton_test->setDisabled(false);
        return ;
    }
    this->ui->textEdit_log->append(QString::fromLocal8Bit("进入配置模式成功"));

    m_tim.start();
    while(m_tim.elapsed() < 5000)
    {
        QCoreApplication::processEvents();
    }

    ret = GetIdHzVersion();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("读ID,采样率,版本号失败");
        qCritical() << m_log_error;
        this->ui->pushButton_test->setDisabled(false);
        return ;
    }
    this->ui->textEdit_log->append(QString::fromLocal8Bit("读ID,采样率,版本号成功"));

    m_tim.start();
    while(m_tim.elapsed() < 9000)
    {
        QCoreApplication::processEvents();
    }

    ret = ReadHeartRate();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("获取心率失败");
        qCritical() << m_log_error;
        this->ui->pushButton_test->setDisabled(false);
        return ;
    }
    this->ui->textEdit_log->append(QString::fromLocal8Bit("获取心率成功"));

    m_tim.start();
    while(m_tim.elapsed() < 10000)
    {
        QCoreApplication::processEvents();
    }

    ret = GetSensorData();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("获取传感器失败");
        this->ui->pushButton_test->setDisabled(false);
        qCritical() << m_log_error;
        return ;
    }
    this->ui->textEdit_log->append(QString::fromLocal8Bit("获取传感器成功"));
    m_tim.start();
    while(m_tim.elapsed() < 3000)
    {
        QCoreApplication::processEvents();
    }

    this->m_mr_result->GetMrResult([&](bool success, QMap<QString, QVariant> udp_svr_data)
    {
       if(success)
       {
           if(udp_svr_data["ec"] == SUCCESS_CODE)
           {
               QJsonArray ac_array = udp_svr_data["ac"].toJsonArray();
               QJsonArray gy_array = udp_svr_data["gy"].toJsonArray();
               int hr =  udp_svr_data["hr"].toDouble();
               int power = udp_svr_data["power"].toDouble();

               QString a_speed = "(" + QString::number(ac_array.at(0).toDouble()) + ","
                       + QString::number(ac_array.at(1).toDouble()) + "," +
                       QString::number(ac_array.at(2).toDouble()) + ")";

               QString tly = "(" + QString::number(gy_array.at(0).toDouble()) + ","
                       + QString::number(gy_array.at(1).toDouble()) + "," +
                       QString::number(gy_array.at(2).toDouble()) + ")";

               this->ui->lineEdit_jz_a->setText(a_speed);
               this->ui->lineEdit_jz_tly->setText(tly);
               this->ui->lineEdit_jz_ppg->setText(QString::number(hr));
               this->ui->lineEdit_jz_power->setText(QString::number(power));
           }
           else
           {
               this->m_handle_err->HandleHttpReqError(udp_svr_data["ec"].toInt());
               QString log = this->m_handle_err->m_http_req_error;
               QMessageBox::information(this,QString::fromLocal8Bit("信息"), log);
               this->ui->textEdit_log->append(log);
           }
       }
    });

    ret = ExitConfig();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("退出配置模式失败");
        qCritical() << m_log_error;
        this->ui->pushButton_test->setDisabled(false);
        return ;
    }
    this->ui->textEdit_log->append(QString::fromLocal8Bit("退出配置模式成功"));

    m_tim.start();
    while(m_tim.elapsed() < 2000)
    {
        QCoreApplication::processEvents();
    }

    ret = ClosePort();
    if(ret == -1)
    {
        m_log_error = QString::fromLocal8Bit("关闭串口失败");
        qCritical() << m_log_error;
        this->ui->pushButton_test->setDisabled(false);
        return ;
    }
    this->ui->textEdit_log->append(QString::fromLocal8Bit("关闭串口成功"));
    this->ui->pushButton_test->setDisabled(false);
    this->CompareData();
}

#if 0
void MainWindow::on_lineEdit_id_textChanged(QString text)
{
    QString com_name = this->ui->comboBox_com->currentText();
    int ret = this->m_sport_wr->EnterAppConfig(com_name);
    if(ret == -1)
    {
        QString fail_log = UTF8BIT("初始化,打开,配置串口") + com_name + UTF8BIT("失败");
        QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
        QMessageBox::information(this, UTF8BIT("信息"), fail_log_msg);
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("初始化,打开,配置串口") + com_name + UTF8BIT("成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        qInfo() << succ_log_msg;
    }

    ret = this->m_sport_wr->SetIspdTagId(text);
    if(ret == -1)
    {
        QString fail_log = UTF8BIT("向串口") + com_name + UTF8BIT("写入ID失败");
        QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
        QMessageBox::information(this, UTF8BIT("信息"), fail_log_msg);
        ret = this->m_sport_wr->ExitAppConfig();
        if(ret == -1)
        {
            QString fail_log = UTF8BIT("串口号") + com_name + UTF8BIT("退出配置模式失败");
            QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
            QMessageBox::information(this, UTF8BIT("信息"), fail_log_msg);
            return ;
        }
        else
        {
            QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("退出配置模式成功");
            QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
            qDebug() << succ_log_msg;
        }
        this->m_sport_wr->m_serial_port->ClosePort();
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("向串口") + com_name + UTF8BIT("写入ID成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        qInfo() << succ_log_msg;
    }

    ret = this->m_sport_wr->ExitAppConfig();
    if(ret == -1)
    {
        QString fail_log = UTF8BIT("串口号") + com_name + UTF8BIT("退出配置模式失败");
        QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
        QMessageBox::information(this, UTF8BIT("信息"), fail_log_msg);
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("退出配置模式成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        qInfo() << succ_log_msg;
    }
    this->m_sport_wr->m_serial_port->ClosePort();
    this->ui->record_status->setText(QString::fromLocal8Bit("录入成功"));
}

void MainWindow::on_pushButton_test_clicked()
{
    this->UIinit();
    m_tim.start();
    while(m_tim.elapsed() < 100)
    {
        QCoreApplication::processEvents();
    }

    QString com_name = this->ui->comboBox_com->currentText();
    int ret = this->m_sport_wr->EnterAppConfig(com_name);
    if(ret == -1)
    {
        QString fail_log = UTF8BIT("初始化,打开,配置串口") + com_name + UTF8BIT("失败");
        QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
        QMessageBox::information(this, UTF8BIT("信息"), fail_log_msg);
        this->ui->textEdit_log->append(fail_log_msg);
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("初始化,打开,配置串口") + com_name + UTF8BIT("成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        this->ui->textEdit_log->append(succ_log_msg);
        qInfo() << succ_log_msg;
    }
    ret = this->m_sport_wr->GetIspdTagId();
    if(ret == -1)
    {
        QString fail_log = UTF8BIT("串口号") +com_name + UTF8BIT("获取ID失败");
        QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
        this->ui->textEdit_log->append(fail_log_msg);
        qCritical() << fail_log_msg;
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("获取ID成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        this->ui->textEdit_log->append(succ_log_msg);
        qInfo() << succ_log_msg;
    }
    this->ui->lineEdit_rid->setText(this->m_sport_wr->m_ispd_id);

    ret = this->m_sport_wr->GetHz();
    if(ret  == -1)
    {
        QString fail_log = UTF8BIT("串口号") +com_name + UTF8BIT("获取采样率失败");
        QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
        this->ui->textEdit_log->append(fail_log_msg);
        qCritical() << fail_log_msg;
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("获采样率成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        this->ui->textEdit_log->append(succ_log_msg);
        qInfo() << succ_log_msg;
    }
    this->ui->lineEdit_hz->setText(this->m_sport_wr->m_hz_lv);

    /*
    ret = this->m_sport_wr->GetVersion();
    if(ret  == -1)
    {
        QString fail_log = UTF8BIT("串口号") +com_name + UTF8BIT("获取版本号失败");
        QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
        this->ui->textEdit_log->append(fail_log_msg);
        qDebug() << fail_log_msg;
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("获取版本号成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        this->ui->textEdit_log->append(succ_log_msg);
        qDebug() << succ_log_msg;
    }
    */
    this->ui->lineEdit_version->setText(UTF8BIT("v2.0.1"));

    m_tim.start();
    while(m_tim.elapsed() < 100)
    {
        QCoreApplication::processEvents();
    }

    ret = this->m_sport_wr->GetHeartRate();
    if(ret == -1)
    {
       QString fail_log = UTF8BIT("串口号") + com_name + UTF8BIT("获取心率数据失败");
       QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
       this->ui->textEdit_log->append(fail_log_msg);
       qCritical() << fail_log_msg;
       return ;
    }
    else
    {
       QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("获取心率数据成功");
       QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
       this->ui->textEdit_log->append(succ_log_msg);
       qInfo() << succ_log_msg;
    }
    this->ui->lineEdit_heart_one->setText(this->m_sport_wr->m_heart_rate.hr_first);
    this->ui->lineEdit_heart_two->setText(this->m_sport_wr->m_heart_rate.sec_hr);
    this->ui->lineEdit_heart_three->setText(this->m_sport_wr->m_heart_rate.third_hr);
    this->ui->lineEdit_heart_avg->setText(this->m_sport_wr->m_heart_rate.avg_hr);

    ret = this->m_sport_wr->GetSensor();
    if(ret == -1)
    {
        QString fail_log = UTF8BIT("串口号") + com_name + UTF8BIT("获取传感器数据失败");
        QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
        this->ui->textEdit_log->append(fail_log_msg);
        qCritical() << fail_log_msg;
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("获取传感器数据成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        this->ui->textEdit_log->append(succ_log_msg);
        qInfo() << succ_log_msg;
    }

    m_tim.start();
    while(m_tim.elapsed() < 1000)
    {
        QCoreApplication::processEvents();
    }

    this->ui->lineEdit_ck_a->setText(this->m_sport_wr->m_sport_data.sp_a_speed);
    this->ui->lineEdit_ck_tly->setText(this->m_sport_wr->m_sport_data.sp_gyroscope);
    this->ui->lineEdit_ck_ppg->setText(this->m_sport_wr->m_sport_data.sp_hr);
    this->ui->lineEdit_ck_power->setText(this->m_sport_wr->m_sport_data.sp_bat);
    this->ui->lineEdit_charge->setText(this->m_sport_wr->m_sport_data.sp_charge);
    this->m_mr_result->m_get_tag_id = this->m_sport_wr->m_id_ver_hz.tag_id.toInt();


    this->m_mr_result->getIDFromUI(this->m_sport_wr->m_http_req_id);
    this->m_mr_result->GetMrResult([&](bool success, QMap<QString, QVariant> udp_svr_data)
    {
       if(success)
       {
           QJsonArray ac_array = udp_svr_data["ac"].toJsonArray();
           QJsonArray gy_array = udp_svr_data["gy"].toJsonArray();
           int hr =  udp_svr_data["hr"].toDouble();
           int power = udp_svr_data["power"].toDouble();

           QString a_speed = "(" + QString::number(ac_array.at(0).toDouble()) + ","
                   + QString::number(ac_array.at(1).toDouble()) + "," +
                   QString::number(ac_array.at(2).toDouble()) + ")";

           QString tly = "(" + QString::number(gy_array.at(0).toDouble()) + ","
                   + QString::number(gy_array.at(1).toDouble()) + "," +
                   QString::number(gy_array.at(2).toDouble()) + ")";

           this->ui->lineEdit_jz_a->setText(a_speed);
           this->ui->lineEdit_jz_tly->setText(tly);
           this->ui->lineEdit_jz_ppg->setText(QString::number(hr));
           this->ui->lineEdit_jz_power->setText(QString::number(power));
       }
    });

    ret = this->m_sport_wr->ExitAppConfig();
    if(ret == -1)
    {
        QString fail_log = UTF8BIT("串口号") + com_name + UTF8BIT("退出配置模式失败");
        QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
        qCritical() << fail_log_msg;
        QMessageBox::information(this, UTF8BIT("信息"), fail_log_msg);
        this->ui->textEdit_log->append(fail_log_msg);
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("退出配置模式成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        this->ui->textEdit_log->append(succ_log_msg);
        qInfo() << succ_log_msg;
    }
    this->m_sport_wr->m_serial_port->ClosePort();
    m_tim.start();
    while(m_tim.elapsed() < 1000)
    {
        QCoreApplication::processEvents();
    }
    CompareData();
}
#endif

void MainWindow::CompareData()
{
    QString imgFileName = QDir::currentPath();
    int mid = imgFileName.lastIndexOf("/");
    imgFileName = imgFileName.left(mid);
    imgFileName.append("/img/");
    QString filenameSucc(imgFileName + "right.png");
    QString filenameError(imgFileName + "error.png");
    if(this->ui->lineEdit_ck_a->text() == this->ui->lineEdit_jz_a->text())
    {
        QImage* img=new QImage;
        if(!(img->load(filenameSucc)))
        {
            delete img;
            return;
        }
        ui->label_compare_a->setPixmap(QPixmap::fromImage(*img));
    }
    else
    {
        QImage* img=new QImage;
        if(!(img->load(filenameError)))
        {
            delete img;
            return;
        }
        ui->label_compare_a->setPixmap(QPixmap::fromImage(*img));
    }

    if(this->ui->lineEdit_ck_tly->text() == this->ui->lineEdit_jz_tly->text())
    {
        QImage* img=new QImage;
        if(!(img->load(filenameSucc)))
        {
            delete img;
            return;
        }
        ui->label_compare_gyo->setPixmap(QPixmap::fromImage(*img));
    }
    else
    {
        QImage* img=new QImage;
        if(!(img->load(filenameError)))
        {
            delete img;
            return;
        }
        ui->label_compare_gyo->setPixmap(QPixmap::fromImage(*img));
    }

    if(this->ui->lineEdit_ck_ppg->text() == this->ui->lineEdit_jz_ppg->text())
    {
        QImage* img=new QImage;
        if(!(img->load(filenameSucc)))
        {
            delete img;
            return;
        }
        ui->label_compare_ppg->setPixmap(QPixmap::fromImage(*img));
    }
    else
    {
        QImage* img=new QImage;
        if(!(img->load(filenameError)))
        {
            delete img;
            return;
        }
        ui->label_compare_ppg->setPixmap(QPixmap::fromImage(*img));
    }

    if(this->ui->lineEdit_ck_power->text() == this->ui->lineEdit_jz_power->text())
    {
        QImage* img=new QImage;
        if(!(img->load(filenameSucc)))
        {
            delete img;
            return;
        }
        ui->label_compare_power->setPixmap(QPixmap::fromImage(*img));
    }
    else
    {
        QImage* img=new QImage;
        if(!(img->load(filenameError)))
        {
            delete img;
            return;
        }
        ui->label_compare_power->setPixmap(QPixmap::fromImage(*img));
    }
}
