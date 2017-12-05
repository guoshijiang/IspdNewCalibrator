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
    std::string GetSystemFunctionErrorMessage();
    int getSerialPortList(int index, QString key_or_value);
    bool openPort(UINT portNo);
    void ClosePort();
    bool initSerilPort(UINT portNo = 1, UINT baud = CBR_9600,
                       char parity = 'N', UINT databits = 8,
                       UINT stopsbits = 1);

    bool initSerilPort(UINT portNo, const LPDCB& plDCB);

    inline unsigned int GetReadTimeout() const
    {
        return this->m_read_time_out;
    }
    inline void SetReadTimeout(unsigned int t = 3000)
    {
        this->m_read_time_out = t;
    }

    inline unsigned int GetWriteTimeout() const
    {
        return this->m_write_time_out;
    }
    inline void SetWriteTimeout(unsigned int t = 3000)
    {
        this->m_write_time_out = t;
    }
    bool readData(char *recv_buf, int recv_len);
    int readData(SerialPort::Data& data);
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
