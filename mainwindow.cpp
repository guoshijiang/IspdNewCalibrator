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
    m_sport_wr(new SerialPortWriteRead), m_udp_con_svr_info(new HttpClient::UdpControlServerInfoHttpReq)
{
    ui->setupUi(this);

    //广播业务类
    m_bc_work = new BroadCastWorker();

    //广播线程
    m_thread = new QThread();
    m_bc_work->moveToThread(m_thread);
    connect(this, &MainWindow::startWork, m_bc_work, &BroadCastWorker::UdpBoardCast);


    connect(m_bc_work, SIGNAL(sendServerBoardCastMsg(QByteArray, QString)), this, SLOT(onSeverBoardCastMsg(QByteArray, QString)));


    this->ui->groupBox_record->setStyleSheet("QGroupBox {background:#e7a16b}");
    this->ui->comboBox_com->setStyleSheet("QComboBox {background:#e7a16b}");
    this->ui->lineEdit_id->setStyleSheet("QLineEdit {background:#e7a16b}");
    this->ui->record_status->setStyleSheet("QLabel {background:#e7a16b}");

    this->fontSet();  //界面字体初始化

    this->m_com_list.clear();
    for (int i=0; i < m_serial_port->m_set_len; i++)
    {
        m_serial_port->getSerialPortList(i, "value");
        m_com_list << this->m_serial_port->m_comm_result;
    }
    qDebug() << m_com_list;
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

    //this->m_bc_work->setStop(true);
    //this->m_thread->quit();
    //this->m_thread->wait();
}

void MainWindow::onSeverBoardCastMsg(const QByteArray &msg, QString serverIP)
{
    this->ui->textEdit_log->append(msg);
    this->ui->textEdit_log->append(serverIP);
    M_IP = serverIP.right(14);
    QString httpReqDataUrl = HTTP_PREDIX + M_IP + ":" + HTTP_REQEST_PORT + UDP_DATA;
    QString setReqUrl = HTTP_PREDIX + M_IP + ":" + HTTP_REQEST_PORT + UDP_CONTROL;

    if(this->m_mr_result->M_RUL.isEmpty())
    {
        this->m_mr_result->httpRequestUrl(httpReqDataUrl);
    }
    else
    {
        qDebug() << "httpReqDataUrl: " << httpReqDataUrl;
    }

    if(this->m_udp_con_svr_info->M_UDP_COMTROL_RUL.isEmpty())
    {
        this->m_udp_con_svr_info->M_UDP_COMTROL_RUL = setReqUrl;
        this->getServerConfigInfo();
    }
    else
    {
        qDebug() << "setReqUrl: " << setReqUrl;
    }

    if(this->m_rsc_config->m_url.isEmpty())
    {
        this->m_rsc_config->m_url = setReqUrl;
    }
    else
    {
        qDebug() << "setReqUrl: " << setReqUrl;
    }

    if(this->m_restart_spec_mr->m_restart_url.isEmpty())
    {
        this->m_restart_spec_mr->m_restart_url = setReqUrl;
    }
    else
    {
        qDebug() << "setReqUrl: " << setReqUrl;
    }

    if(this->m_factory_reset->m_factory_reset_url.isEmpty())
    {
        this->m_factory_reset->m_factory_reset_url = setReqUrl;
    }
    else
    {
        qDebug() << "setReqUrl: " << setReqUrl;
    }

    if(this->m_start_mr_auto_freset->m_open_close_url.isEmpty())
    {
        this->m_start_mr_auto_freset->m_open_close_url = setReqUrl;
    }
    else
    {
        qDebug() << "setReqUrl: " << setReqUrl;
    }

    if(this->m_start_mr_auto_domain->m_sync_rul.isEmpty())
    {
        this->m_start_mr_auto_domain->m_sync_rul = setReqUrl;
    }
    else
    {
        qDebug() << "setReqUrl: " << setReqUrl;
    }

    if(this->m_css_info->m_udp_control_url.isEmpty())
    {
        this->m_css_info->m_udp_control_url = setReqUrl;
    }
    else
    {
        qDebug() << "setReqUrl: " << setReqUrl;
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
              qDebug() << UTF8BIT("读取成功");
           }
           else
           {
              qDebug() << UTF8BIT("读取失败");
           }
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
        if(m_com_list[j] != "COM1")
        {
            this->ui->comboBox_com->addItem(m_com_list[j]);
        }
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
        qDebug() << succ_log_msg;
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
        qDebug() << succ_log_msg;
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
        qDebug() << succ_log_msg;
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
        qDebug() << succ_log_msg;
    }

    ret = this->m_sport_wr->GetHzVersionTagId();
    if(ret  == -1)
    {
        QString fail_log = UTF8BIT("串口号") +com_name + UTF8BIT("获取ID,采样率,版本号失败");
        QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
        this->ui->textEdit_log->append(fail_log_msg);
        qDebug() << fail_log_msg;
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("获取ID,采样率,版本号成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        this->ui->textEdit_log->append(succ_log_msg);
        qDebug() << succ_log_msg;
    }

    this->ui->lineEdit_rid->setText(this->m_sport_wr->m_id_ver_hz.tag_id);
    this->ui->lineEdit_hz->setText(this->m_sport_wr->m_id_ver_hz.hz);
    this->ui->lineEdit_version->setText(this->m_sport_wr->m_id_ver_hz.version);

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
       qDebug() << fail_log_msg;
       return ;
    }
    else
    {
       QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("获取心率数据成功");
       QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
       this->ui->textEdit_log->append(succ_log_msg);
       qDebug() << succ_log_msg;
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
        qDebug() << fail_log_msg;
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("获取传感器数据成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        this->ui->textEdit_log->append(succ_log_msg);
        qDebug() << succ_log_msg;
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
        QMessageBox::information(this, UTF8BIT("信息"), fail_log_msg);
        this->ui->textEdit_log->append(fail_log_msg);
        return ;
    }
    else
    {
        QString succ_log = UTF8BIT("串口号") + com_name + UTF8BIT("退出配置模式成功");
        QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
        this->ui->textEdit_log->append(succ_log_msg);
        qDebug() << succ_log_msg;
    }
    this->m_sport_wr->m_serial_port->ClosePort();
    m_tim.start();
    while(m_tim.elapsed() < 1000)
    {
        QCoreApplication::processEvents();
    }
    CompareData();
}

void MainWindow::CompareData()
{
    QString filenameSucc("D:/latestQt/IspdNewCalibrator/right.png");
    QString filenameError("D:/latestQt/IspdNewCalibrator/error.png");
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
