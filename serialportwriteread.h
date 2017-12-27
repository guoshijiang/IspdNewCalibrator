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

typedef struct _ServerMrData
{
    QString mr_a_speed;   //基站加速度
    QString mr_gyroscope; //基站陀螺仪
    QString mr_hr;        //基站心率
    QString mr_charge;    //基站电量
}ServerMrData;

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
    ServerMrData svrMrRelust; //基站数据
    HeartRateData hrResult;   //心率数据
    SerilPortData spResult;   //串口数据
}CheckData;

class SerialPortWriteRead
{
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

public:
    SerialPort *m_serial_port;
    Encrypt *m_encrypt;
    CheckData m_check_data;
private:
    SerialPortWriteRead(const SerialPortWriteRead &);
    const SerialPortWriteRead & operator = (const SerialPortWriteRead &);
};

#endif
