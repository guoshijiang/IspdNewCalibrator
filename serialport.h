#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include <QSettings>
#include <QStringList>
#include <QString>
#include <QDebug>

#include <iostream>

#include <Windows.h>

#include "common.h"

const QString g_path = "HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM";
typedef struct SerialPortInfo
{

}sport_info;

class SerialPort
{
public:
    typedef struct data_t
    {
       const char* buffer;
       size_t length;
    }Data;

public:
    SerialPort();
    ~SerialPort();
    int getSerialPortList(int index, QString key_or_value);
    bool openPort(char* portName);
    void ClosePort();
    bool initSerilPort(char *portName = "com1", UINT baud = CBR_9600,
                       char parity = 'N', UINT databits = 8,
                       UINT stopsbits = 1);
    bool initSerilPort(char* portName, const LPDCB& plDCB);
    UINT GetBytesFromCOM();
    int readData(char *msg, int msg_len);
    int writeData(const SerialPort::Data& data);

private:
    SerialPort(const SerialPort &);
    const SerialPort & operator = (const SerialPort &);
public:
    const char *m_buffer;
    size_t m_length;
    HANDLE  m_hle_comm;

    char* m_read_buffer;
    size_t m_read_buffer_cap;

    //获取串口相关变量
    QSettings *m_set;
    QString m_comm_result;
    int m_set_len;

    //超时相关
    unsigned int m_read_time_out;
    unsigned int m_write_time_out;

private:
    HKEY m_h_key;
    LPCWSTR m_sub_key;
    wchar_t m_key_name[256];
    char m_key_value[256];
    DWORD m_key_size, m_type, m_value_size;
    int m_index_num;
};
#endif //SERIALPORT_H_
