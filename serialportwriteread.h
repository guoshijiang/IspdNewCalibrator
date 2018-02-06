#ifndef SERIALPORTWRITEREAD_H
#define SERIALPORTWRITEREAD_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QElapsedTimer>
#include "serialport.h"
#include "protoserialport.h"
#include "common.h"
#include "basehttpclient.h"
#include "encrypt.h"

typedef struct _HeartRateData
{
    QString hr_first;
    QString sec_hr;
    QString third_hr;
    QString avg_hr;
}HeartRateData;

typedef struct _SerilPortData
{
    QString sp_a_speed;
    QString sp_gyroscope;
    QString sp_hr;
    QString sp_bat;
    QString sp_charge;
}SerilPortData;

typedef struct _IdVersionHz
{
    QString tag_id;
    QString hz;
    QString version;
}IdVerHz;

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
    HeartRateData m_heart_rate;
    SerilPortData m_sport_data;
    IdVerHz m_id_ver_hz;
    QString m_com_name;

    QString m_ispd_id;
    QString m_fail_log;
    QString m_succ_log;

private:
    SerialPortWriteRead(const SerialPortWriteRead &);
    const SerialPortWriteRead & operator = (const SerialPortWriteRead &);
};

#endif
