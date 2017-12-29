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
/*
 * 1.界面日志与实际输出日志同步
 * 2.进度条同步
 * 3.容错处里
 * 4.http请求完善
 * 5.整改
 * 6.操作文档完善
 * 7.版本完善
 *
*/

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
    m_sport_wr(new SerialPortWriteRead)
{
    ui->setupUi(this);

    this->ui->progressBar_check->setMinimum(0);
    this->ui->progressBar_check->setMaximum(100);
    this->ui->progressBar_check->setValue(0);

    this->ui->record_progressBar->setMinimum(0);
    this->ui->record_progressBar->setMaximum(100);
    this->ui->record_progressBar->setValue(0);
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
}

void MainWindow::testAutoSlot()
{
   this->ui->tabWidget->setCurrentIndex(4);
}

void MainWindow::testMulSlot()
{ 
    this->ui->tabWidget->setCurrentIndex(3);
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

void MainWindow::TogetherTestSlot()
{
    this->ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::RecordSlot()
{
    this->ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::CheckComSlot()
{
    this->ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::CloseApp()
{
    this->close();
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

void MainWindow::onCheckSuccLogMessage(const QString & msg)
{
    QString succ_log_msg = MESSAGE_SUC + msg + MESSAGE_CED;
    this->ui->textEdit_check_status->append(succ_log_msg);
}

void MainWindow::onCheckFailLogMessage(const QString & msg)
{
    QString fail_log_msg = MESSAGE_RED + msg + MESSAGE_END;
    this->ui->textEdit_check_status->append(fail_log_msg);
}

void MainWindow::onRecordSuccLogMsg(const QString & msg)
{
    QString succ_log_msg = MESSAGE_SUC + msg + MESSAGE_CED;
    this->ui->textEdit_record->append(succ_log_msg);
}

void MainWindow::onRecordFailMsg(const QString & msg)
{
    QString fail_log_msg = MESSAGE_RED + msg + MESSAGE_END;
    this->ui->textEdit_record->append(fail_log_msg);
}

void MainWindow::start()
{
    this->setWindowTitle(QString::fromLocal8Bit("超版板上位机测试软件"));
    this->showMaximized();
    connect(this->ui->action_auto, &QAction::triggered, this, &MainWindow::testAutoSlot);
    connect(this->ui->action_mul, &QAction::triggered, this, &MainWindow::testMulSlot);
    connect(this->ui->action_version, &QAction::triggered, this, &MainWindow::VersionSlot);
    connect(this->ui->action_question, &QAction::triggered, this, &MainWindow::QuestionSlot);
    connect(this->ui->action_doc, &QAction::triggered, this, &MainWindow::DocSlot);
    connect(this->ui->action_serialport_set, &QAction::triggered, this, &MainWindow::SerialPortSlot);
    connect(this->ui->action_hb, &QAction::triggered, this, &MainWindow::TogetherTestSlot);
    connect(this->ui->action_record, &QAction::triggered, this, &MainWindow::RecordSlot);
    connect(this->ui->action_com, &QAction::triggered, this, &MainWindow::CheckComSlot);
    connect(this->ui->action_eixit, &QAction::triggered, this, &MainWindow::CloseApp);
    connect(this->ui->action_open_close_domain, &QAction::triggered, this, &MainWindow::StartUpMrCloseAutoDomainSlot);
    connect(this->ui->action_open_close_reset, &QAction::triggered, this, &MainWindow::StartUpMrCloseAutoFactoryResetSlot);
    connect(this->ui->action_specmr_domain, &QAction::triggered, this, &MainWindow::SyncSpecMrDomainSlot);
    connect(this->ui->action_reset_cch, &QAction::triggered, this, &MainWindow::FactoryResetSlot);
    connect(this->ui->action_reset_specmr, &QAction::triggered, this, &MainWindow::RestartMrSlot);
    connect(this->ui->action_write_speconfig, &QAction::triggered, this, &MainWindow::WriteSpecConfigSlot);
    connect(this->ui->action_read_speconfig, &QAction::triggered, this, &MainWindow::ReadSpecificConfigSlot);
    connect(this->ui->action_udp_server_info, &QAction::triggered, this, &MainWindow::ControlServerStutasInfoSlot);

    connect(m_sport_wr, SIGNAL(sendCheckSuccLogMessage(QString)), this, SLOT(onCheckSuccLogMessage(QString)));
    connect(m_sport_wr, SIGNAL(sendCheckFailLogMessage(QString)), this, SLOT(onCheckFailLogMessage(QString)));
    connect(m_sport_wr, SIGNAL(sendRecordSuccLogMessage(QString)), this, SLOT(onRecordSuccLogMsg(QString)));
    connect(m_sport_wr, SIGNAL(sendRecordFailLogMessage(QString)), this, SLOT(onRecordFailMsg(QString)));

    this->m_com_list.clear();
    for (int i=0; i < m_serial_port->m_set_len; i++)
    {
        m_serial_port->getSerialPortList(i, "value");
        m_com_list << this->m_serial_port->m_comm_result;
    }
}

void MainWindow::on_pushButton_start_record_clicked()
{
    QScrollArea *record_area = new QScrollArea();
    QWidget *record_widget = new QWidget();
    record_widget->setStyleSheet("QWidget" "{background:white;}");
    m_vbox_layout = new QVBoxLayout();
    m_vbox_layout->addSpacerItem(new QSpacerItem(100, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    record_widget->setLayout(m_vbox_layout);
    record_area->setWidget(record_widget);
    record_area->setWidgetResizable(true);
    m_main_layout = new QVBoxLayout();
    m_main_layout->addWidget(record_area);
    this->ui->groupBox_record->setLayout(m_main_layout);
    for(int i = 0; i < m_com_list.size(); ++i)
    {
        if(m_com_list[i] != "COM1")
        {
            this->addAutoRecordUi(m_com_list[i]);
        }
        else
        {
            continue;
        }
    }
}

void MainWindow::addAutoRecordUi(QString com_name)
{
    this->m_record_page = new QWidget;
    this->m_record_layout = new QGridLayout(m_record_page);
    this->m_record_label = new QLabel;
    this->m_record_label->setText(com_name);
    this->m_record_label->setFont(font());
    this->m_record_ledit = new QLineEdit;
    this->m_record_ledit->setFont(font());
    this->m_auto_record_list.push_back(this->m_record_ledit);
    this->m_record_layout->addWidget(m_record_label, 0, 1);
    this->m_record_layout->addWidget(m_record_ledit, 0, 2);
    this->m_record_page->setLayout(m_record_layout);
    this->m_vbox_layout->insertWidget(m_vbox_layout->count()-1, m_record_page);
    this->m_record_ledit->setObjectName(com_name);
    connect(this->m_record_ledit, SIGNAL(textChanged(QString)), SLOT(m_record_ledit_textChanged(QString)));
}

void MainWindow::m_record_ledit_textChanged(QString text)
{
    QObject *obj = QObject::sender();
    QString senderobjName = obj->objectName();
    for(int k = 0; k < this->m_com_list.size(); ++k)
    {
        if(this->m_com_list[k] == "COM1")
        {
            continue;
        }
        if(this->m_com_list[k] == senderobjName)
        {
            int ret = this->m_sport_wr->EnterAppConfig(m_com_list[k]);
            if(ret == -1)
            {
                QString fail_log = UTF8BIT("初始化,打开,配置串口") + m_com_list[k] + UTF8BIT("失败");
                QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
                this->ui->textEdit_record->append(fail_log_msg);
                return ;
            }
            else
            {
                QString succ_log = UTF8BIT("初始化,打开,配置串口") + m_com_list[k] + UTF8BIT("成功");
                QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
                this->ui->textEdit_record->append(succ_log_msg);
            }

            ret = this->m_sport_wr->SetIspdTagId(text);
            if(ret == -1)
            {
                QString fail_log = UTF8BIT("向串口") + m_com_list[k] + UTF8BIT("写入ID失败");
                QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
                this->ui->textEdit_record->append(fail_log_msg);
                return ;
            }
            else
            {
                QString succ_log = UTF8BIT("向串口") + m_com_list[k] + UTF8BIT("写入ID成功");
                QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
                this->ui->textEdit_record->append(succ_log_msg);
            }

            ret = this->m_sport_wr->ExitAppConfig();
            if(ret == -1)
            {
                QString fail_log = UTF8BIT("串口号") + m_com_list[k] + UTF8BIT("退出配置模式失败");
                QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
                this->ui->textEdit_record->append(fail_log_msg);
                return ;
            }
            else
            {
                QString succ_log = UTF8BIT("串口号") + m_com_list[k] + UTF8BIT("退出配置模式成功");
                QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
                this->ui->textEdit_record->append(succ_log_msg);
            }
        }  
    } 
}

void MainWindow::ServerDataHttpRequest(QString tag_id)
{
    this->m_mr_result->m_get_tag_id = tag_id;
    this->m_mr_result->GetMrResult([&](bool success, QMap<QString, QVariant>mr_result)
    {
       if(success)
       {
           //svrmr_data[index].mr_a_speed = mr_result["a_speed"].toString();
           //svrmr_data[index].mr_gyroscope = mr_result["gyroscope"].toString();
           //svrmr_data[index].mr_hr = mr_result["hr"].toString();
           //svrmr_data[index].mr_charge  = mr_result["bat"].toString();
       }
    });
}

void MainWindow::on_pushButton_start_check_clicked()
{
    QScrollArea *check_area = new QScrollArea();
    QWidget *check_widget = new QWidget();
    check_widget->setStyleSheet("QWidget" "{background:white;}");
    m_vbox_layout = new QVBoxLayout();
    m_vbox_layout->addSpacerItem(new QSpacerItem(100, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    check_widget->setLayout(m_vbox_layout);
    check_area->setWidget(check_widget);
    check_area->setWidgetResizable(true);
    m_main_layout = new QVBoxLayout();
    m_main_layout->addWidget(check_area);

    this->ui->groupBox_check->setLayout(m_main_layout);

    this->m_ispd_num = m_com_list.size();
    this->m_check_upload_process = 0;
    this->ui->progressBar_check->setValue(this->m_check_upload_process);
    this->m_check_upload_rate = 100.0 / float(this->m_ispd_num);
    for(int i = 0; i < m_com_list.size(); ++i)
    {
        this->m_check_upload_process += this->m_check_upload_rate;
        this->ui->progressBar_check->setValue(int(this->m_check_upload_process));
        if(m_com_list[i] != "COM1")
        {
            int ret = this->m_sport_wr->EnterAppConfig(m_com_list[i]);
            if(ret == -1)
            {
                QString fail_log = UTF8BIT("初始化,打开,配置串口") + m_com_list[i] + UTF8BIT("失败");
                QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
                this->ui->textEdit_check_status->append(fail_log_msg);
                return ;
            }
            else
            {
                QString succ_log = UTF8BIT("初始化,打开,配置串口") + m_com_list[i] + UTF8BIT("成功");
                QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
                this->ui->textEdit_check_status->append(succ_log_msg);
            }

            ret = this->m_sport_wr->GetHzVersionTagId();
            if(ret  == -1)
            {
                QString fail_log = UTF8BIT("串口号") + m_com_list[i] + UTF8BIT("获取ID,采样率,版本号失败");
                QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
                this->ui->textEdit_check_status->append(fail_log_msg);
                return ;
            }
            else
            {
                QString succ_log = UTF8BIT("串口号") + m_com_list[i] + UTF8BIT("获取ID,采样率,版本号成功");
                QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
                this->ui->textEdit_check_status->append(succ_log_msg);
            }
            ret = this->m_sport_wr->GetSensor();
            if(ret == -1)
            {
                QString fail_log = UTF8BIT("串口号") + m_com_list[i] + UTF8BIT("获取传感器数据失败");
                QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
                this->ui->textEdit_check_status->append(fail_log_msg);
                return ;
            }
            else
            {
                QString succ_log = UTF8BIT("串口号") + m_com_list[i] + UTF8BIT("获取传感器数据成功");
                QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
                this->ui->textEdit_check_status->append(succ_log_msg);
            }
            ret = this->m_sport_wr->GetHeartRate();
            if(ret == -1)
            {
                QString fail_log = UTF8BIT("串口号") + m_com_list[i] + UTF8BIT("获取心率数据失败");
                QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
                this->ui->textEdit_check_status->append(fail_log_msg);
                return ;
            }
            else
            {
                QString succ_log = UTF8BIT("串口号") + m_com_list[i] + UTF8BIT("获取心率数据成功");
                QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
                this->ui->textEdit_check_status->append(succ_log_msg);
            }

            ret = this->m_sport_wr->ExitAppConfig();
            if(ret == -1)
            {
                QString fail_log = UTF8BIT("串口号") + m_com_list[i] + UTF8BIT("退出配置模式失败");
                QString fail_log_msg = MESSAGE_RED + fail_log + MESSAGE_END;
                this->ui->textEdit_check_status->append(fail_log_msg);
                return ;
            }
            else
            {
                QString succ_log = UTF8BIT("串口号") + m_com_list[i] + UTF8BIT("退出配置模式成功");
                QString succ_log_msg = MESSAGE_SUC + succ_log + MESSAGE_CED;
                this->ui->textEdit_check_status->append(succ_log_msg);
            }
            this->AddMulChildUI(m_com_list[i], m_sport_wr->m_check_data);
        }
        else
        {
            continue;
        }
    }
    this->ui->progressBar_check->setValue(100);
}

void MainWindow::AddMulChildUI(QString com_name, CheckData checkdata)
{
    QWidget *page = new QWidget;
    QGroupBox *all_group = new QGroupBox;
    all_group->setTitle(com_name);
    all_group->setFont(font());
    QVBoxLayout *all_box = new QVBoxLayout(all_group);
    QVBoxLayout *vbox = new QVBoxLayout();
    QGroupBox *con_group = new QGroupBox;
    QGridLayout *layout_config = new QGridLayout(con_group);

    con_group->setTitle(UTF8BIT("配置信息"));
    con_group->setFont(font());

    QLabel *ispd_id = new QLabel;
    ispd_id->setText(UTF8BIT("标签"));

    QLabel *sam_rate = new QLabel;
    sam_rate->setText(UTF8BIT("采用率"));

    QLabel *version = new QLabel;
    version->setText(UTF8BIT("版本号"));

    QLineEdit *ispd_id_edit = new QLineEdit;
    ispd_id_edit->setText(checkdata.tag_id);

    QLineEdit *sam_rate_edit = new QLineEdit;
    sam_rate_edit->setText(checkdata.hz);

    QLineEdit *version_edit = new QLineEdit;
    version_edit->setText(checkdata.version);

    layout_config->addWidget(ispd_id, 0, 1);
    layout_config->addWidget(ispd_id_edit, 0, 2);

    layout_config->addWidget(sam_rate, 1, 1);
    layout_config->addWidget(sam_rate_edit, 1, 2);

    layout_config->addWidget(version, 2, 1);
    layout_config->addWidget(version_edit, 2, 2);

    QGroupBox *hr_group = new QGroupBox;
    QGridLayout *layout_hr = new QGridLayout(hr_group);
    hr_group->setTitle(UTF8BIT("心率"));
    hr_group->setFont(font());

    QLabel *hr_label = new QLabel;
    hr_label->setText(UTF8BIT("读取心率"));

    QLabel *hr_one = new QLabel;
    hr_one->setText(UTF8BIT("第一次"));

    QLabel *hr_two = new QLabel;
    hr_two->setText(UTF8BIT("第二次"));

    QLabel *hr_three = new QLabel;
    hr_three->setText(UTF8BIT("第三次"));

    QLabel *hr_avg = new QLabel;
    hr_avg->setText(UTF8BIT("平均值"));

    QLineEdit *hr_edit_one = new QLineEdit;
    hr_edit_one->setText(checkdata.hrResult.hr_first);

    QLineEdit *hr_edit_two = new QLineEdit;
    hr_edit_two->setText(checkdata.hrResult.sec_hr);

    QLineEdit *hr_edit_three = new QLineEdit;
    hr_edit_three->setText(checkdata.hrResult.third_hr);

    QLineEdit *hr_edit_avg = new QLineEdit;
    hr_edit_avg->setText(checkdata.hrResult.avg_hr);

    layout_hr->addWidget(hr_label, 2, 1);

    layout_hr->addWidget(hr_one, 1, 2);
    layout_hr->addWidget(hr_two, 1, 3);
    layout_hr->addWidget(hr_three, 1, 4);
    layout_hr->addWidget(hr_avg, 1, 5);

    layout_hr->addWidget(hr_edit_one, 2, 2);
    layout_hr->addWidget(hr_edit_two, 2, 3);
    layout_hr->addWidget(hr_edit_three, 2, 4);
    layout_hr->addWidget(hr_edit_avg, 2, 5);

    QGroupBox *compare_group = new QGroupBox;
    QGridLayout *layout_datapare = new QGridLayout(compare_group);
    compare_group->setTitle(UTF8BIT("对比数据"));
    compare_group->setFont(font());

    QLabel *seriport_ret = new QLabel;
    seriport_ret->setText(UTF8BIT("串口结果"));

    QLabel *mr_ret = new QLabel;
    mr_ret->setText(UTF8BIT("基站结果"));

    QLabel *comp_ret = new QLabel;
    comp_ret->setText(UTF8BIT("对比结果"));

    QLabel *a_speed = new QLabel;
    a_speed->setText(UTF8BIT("加速度"));

    QLineEdit *edit_a_seripor = new QLineEdit;
    edit_a_seripor->setText(checkdata.spResult.sp_a_speed);

    QLineEdit *edit_a_mr = new QLineEdit;
    edit_a_mr->setText(svrmr_data[0].mr_a_speed);

    QLineEdit *edit_a_comp = new QLineEdit;
    edit_a_comp->setText(UTF8BIT("比对失败"));

    QLabel *gyro = new QLabel;
    gyro->setText(UTF8BIT("陀螺仪"));

    QLineEdit *edit_gyro_seripor = new QLineEdit;
    edit_gyro_seripor->setText(checkdata.spResult.sp_gyroscope);

    QLineEdit *edit_gyro_mr = new QLineEdit;
    edit_gyro_mr->setText(svrmr_data[0].mr_gyroscope);

    QLineEdit *edit_gyro_comp = new QLineEdit;
    edit_gyro_comp->setText(UTF8BIT("比对失败"));

    QLabel *heart_rate = new QLabel;
    heart_rate->setText(UTF8BIT("心率"));

    QLineEdit *edit_hr_seripor = new QLineEdit;
    edit_hr_seripor->setText(checkdata.spResult.sp_hr);

    QLineEdit *edit_hr_mr = new QLineEdit;
    edit_hr_mr->setText(svrmr_data[0].mr_hr);

    QLineEdit *edit_hr_comp = new QLineEdit;
    edit_hr_comp->setText(UTF8BIT("比对失败"));

    QLabel *bat = new QLabel;
    bat->setText(UTF8BIT("电量"));

    QLineEdit *edit_bat_seripor = new QLineEdit;
    edit_bat_seripor->setText(checkdata.spResult.sp_bat);

    QLineEdit *edit_bat_mr = new QLineEdit;
    edit_bat_mr->setText(svrmr_data[0].mr_charge);

    QLineEdit *edit_bat_comp = new QLineEdit;
    edit_bat_comp->setText(UTF8BIT("比对失败"));

    QLabel *charge = new QLabel;
    charge->setText(UTF8BIT("充电状态"));

    QLineEdit *edit_charge = new QLineEdit;
    edit_charge->setText(checkdata.spResult.sp_charge);

    layout_datapare->addWidget(seriport_ret, 0, 2);
    layout_datapare->addWidget(mr_ret, 0, 3);
    layout_datapare->addWidget(comp_ret, 0, 4);

    layout_datapare->addWidget(a_speed, 1, 1);
    layout_datapare->addWidget(edit_a_seripor, 1, 2);
    layout_datapare->addWidget(edit_a_mr, 1, 3);
    layout_datapare->addWidget(edit_a_comp, 1, 4);

    layout_datapare->addWidget(gyro, 2, 1);
    layout_datapare->addWidget(edit_gyro_seripor, 2, 2);
    layout_datapare->addWidget(edit_gyro_mr, 2, 3);
    layout_datapare->addWidget(edit_gyro_comp, 2, 4);

    layout_datapare->addWidget(heart_rate, 3, 1);
    layout_datapare->addWidget(edit_hr_seripor, 3, 2);
    layout_datapare->addWidget(edit_hr_mr, 3, 3);
    layout_datapare->addWidget(edit_hr_comp, 3, 4);

    layout_datapare->addWidget(bat, 4, 1);
    layout_datapare->addWidget(edit_bat_seripor, 4, 2);
    layout_datapare->addWidget(edit_bat_mr, 4, 3);
    layout_datapare->addWidget(edit_bat_comp, 4, 4);

    layout_datapare->addWidget(charge, 5, 1);
    layout_datapare->addWidget(edit_charge, 5, 2);

    all_box->addWidget(con_group);
    all_box->addWidget(hr_group);
    all_box->addWidget(compare_group);

    vbox->addWidget(all_group);

    page->setLayout(vbox);
    m_vbox_layout->insertWidget(m_vbox_layout->count()-1, page);
}

void MainWindow::on_pushButton_clear_record_clicked()
{
    this->ui->textEdit_record->clear();
}

void MainWindow::on_pushButton_clear_check_clicked()
{
    this->ui->textEdit_check_status->clear();
}


