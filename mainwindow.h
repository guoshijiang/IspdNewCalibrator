#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Windows.h>
#pragma comment(lib,"advapi32.lib")

#include<tchar.h>
#include <QSettings>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QFrame>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QFont>
#include <QDebug>
#include <QMap>
#include <QAction>
#include <QByteArray>
#include <QToolBox>
#include <QTimer>
#include <QMessageBox>
#include <QPalette>
#include <QThread>
#include <QRegExp>

#include "version.h"
#include "question.h"
#include "document.h"
#include "serialportset.h"
#include "encrypt.h"
#include "controlserverstutasinfo.h"
#include "readspecificconfig.h"
#include "serialport.h"
#include "writespecconfig.h"
#include "startupmrautodomain.h"
#include "startupmrautofactoryreset.h"
#include "factoryreset.h"
#include "restartmr.h"
#include "syncspecmrdomain.h"
#include "basehttpclient.h"
#include "serialportwriteread.h"
#include "broadcastworker.h"

namespace Ui
{
    class MainWindow;
}

typedef struct ServerHttpReqData
{
    QString mr_a_speed;
    QString mr_gyroscope;
    QString mr_hr;
    QString mr_charge;
}ServerMrData;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void start();
    QString getcomm(int index, QString keyorvalue);
    inline QFont font()
    {
        QFont m_font("宋体", 18, 75);
        return m_font;
    }

    inline QString Succ()
    {
        return UTF8BIT("比对成功");
    }

    inline QString Fail()
    {
        return UTF8BIT("比对失败");
    }

    void UIinit();
    void CompareData();
    void fontSet();
    void getServerConfigInfo();

public slots:
    void VersionSlot();
    void QuestionSlot();
    void DocSlot();
    void SerialPortSlot();
    void CloseApp();
    void ControlServerStutasInfoSlot();
    void ReadSpecificConfigSlot();
    void WriteSpecConfigSlot();
    void StartUpMrCloseAutoDomainSlot();
    void StartUpMrCloseAutoFactoryResetSlot();
    void SyncSpecMrDomainSlot();
    void FactoryResetSlot();
    void RestartMrSlot();
    void onSeverBoardCastMsg(const QByteArray &msg, QString serverIP);
    void onRecordSuccLogMsg(const QString & msg);
    void onRecordFailMsg(const QString & msg);

public:
    Ui::MainWindow *ui;

    Version *m_version;
    Question *m_question;
    Document *m_doc;
    SerialPortSet *m_port_set;

    ControlServerStutasInfo *m_css_info;
    ReadSpecificConfig *m_rsc_config;
    WriteSpecConfig *m_write_spec_config;
    StartUpMrAutoDomain *m_start_mr_auto_domain;
    StartUpMrAutoFactoryReset *m_start_mr_auto_freset;
    SyncSpecMrDomain *m_sync_spec_mr_domain;
    RestartMr *m_restart_spec_mr;
    FactoryReset *m_factory_reset;

    HttpClient::GetMrReslutHttpReqest* m_mr_result;
    HttpClient::UdpControlServerInfoHttpReq* m_udp_con_svr_info;

    SerialPortWriteRead* m_sport_wr;
    SerialPort *m_serial_port;

    QVBoxLayout *m_vbox_layout;
    QVBoxLayout *m_main_layout;

    QWidget *m_record_page;
    QGridLayout *m_record_layout;
    QLineEdit *m_record_ledit;
    QLabel *m_record_label;
    QPushButton *m_but;
    QList<QLineEdit*> m_auto_record_list;
    QStringList m_com_list;
    QElapsedTimer m_tim;

    BroadCastWorker* m_bc_work;
    QThread *m_thread;

    int m_con_sync;
    int m_con_reset;
    QList<int> m_con_mid;

    QString M_IP;

signals:
    void startWork();

private slots:
    void on_pushButton_test_clicked();
    void on_lineEdit_id_textChanged(QString text);

private:
    unsigned int m_check_upload_process;
    unsigned int m_ispd_num;
    unsigned int m_check_upload_rate;
};

#endif // MAINWINDOW_H
