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
    void addAutoRecordUi(QString com_name);
    void AddMulChildUI(QString com_name, CheckData checkdata);
    void ServerDataHttpRequest(QString tag_id);

public slots:
    void testAutoSlot();
    void testMulSlot();
    void VersionSlot();
    void QuestionSlot();
    void DocSlot();
    void SerialPortSlot();
    void TogetherTestSlot();
    void RecordSlot();
    void CheckComSlot();
    void CloseApp();
    void ControlServerStutasInfoSlot();
    void ReadSpecificConfigSlot();
    void WriteSpecConfigSlot();
    void StartUpMrCloseAutoDomainSlot();
    void StartUpMrCloseAutoFactoryResetSlot();
    void SyncSpecMrDomainSlot();
    void FactoryResetSlot();
    void RestartMrSlot();
    void m_record_ledit_textChanged(QString text);

    void onCheckSuccLogMessage(const QString & msg);
    void onCheckFailLogMessage(const QString & msg);
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
    SerialPortWriteRead* m_sport_wr;
    SerialPort *m_serial_port;


    //主要控件
    QVBoxLayout *m_vbox_layout;
    QVBoxLayout *m_main_layout;

    //录入标签相关的控件
    QWidget *m_record_page;
    QGridLayout *m_record_layout;
    QLineEdit *m_record_ledit;
    QLabel *m_record_label;
    QPushButton *m_but;
    QList<QLineEdit*> m_auto_record_list;

    QStringList m_com_list;

private:
    unsigned int m_check_upload_process;
    unsigned int m_ispd_num;
    unsigned int m_check_upload_rate;

private slots:
    void on_pushButton_start_record_clicked();
    void on_pushButton_start_check_clicked();
    void on_pushButton_clear_check_clicked();
    void on_pushButton_clear_record_clicked();
};

#endif // MAINWINDOW_H
