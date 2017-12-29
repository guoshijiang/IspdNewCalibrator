#ifndef SERIALPORTWRITEREAD_H
#define SERIALPORTWRITEREAD_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include "serialport.h"
#include "protoserialport.h"
#include "common.h"
#include "encrypt.h"

typedef struct _HeartRateData
{
    QString hr_first;   //第一次获取到的心率
    QString sec_hr;     //第二次获取到的心率
    QString third_hr;   //第三次获取到的心率
    QString avg_hr;     //平均心率
}HeartRateData;

typedef struct _SerilPortData
{
    QString sp_a_speed;   //串口加速度
    QString sp_gyroscope; //串口陀螺仪
    QString sp_hr;        //串口心率
    QString sp_bat;       //串口电量
    QString sp_charge;    //串口充电状态
}SerilPortData;

//全部数据
typedef struct _CheckData
{
    QString tag_id;           //标签号
    QString hz;               //采样率
    QString version;          //版本号
    HeartRateData hrResult;   //心率数据
    SerilPortData spResult;   //串口数据
}CheckData;

class SerialPortWriteRead : public QObject
{
     Q_OBJECT
public:
    SerialPortWriteRead();
    SerialPortWriteRead(QString com_name);
    ~SerialPortWriteRead();

    int EnterAppConfig(QString com_name);
    int ExitAppConfig();
    int SetIspdTagId(QString record_id);
    int GetIspdTagId();
    int GetHz();
    int GetVersion();
    int GetHeartRate();
    int GetSensor();
    int GetHzVersionTagId();

public slots:

signals:
    void sendRecordSuccLogMessage(const QString & msg);
    void sendRecordFailLogMessage(const QString & msg);

    void sendCheckSuccLogMessage(const QString & msg);
    void sendCheckFailLogMessage(const QString & msg);

public:
    SerialPort *m_serial_port;
    Encrypt *m_encrypt;
    CheckData m_check_data;
    QString m_com_name;
    QString m_ispd_id;
    QString m_fail_log;
    QString m_succ_log;

private:
    SerialPortWriteRead(const SerialPortWriteRead &);
    const SerialPortWriteRead & operator = (const SerialPortWriteRead &);
};

#endif
