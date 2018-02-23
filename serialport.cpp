#include "SerialPort.h"
#include <iostream>

SerialPort::SerialPort()
{
    this->m_read_time_out = 5000;
    this->m_write_time_out = 5000;
    this->m_set = new QSettings(g_path, QSettings::NativeFormat);
    QStringList key = this->m_set->allKeys();
    this->m_set_len = key.size();
}

SerialPort::~SerialPort()
{  
    delete this->m_set;
    this->ClosePort();
}

int SerialPort::getSerialPortList(int index, QString key_or_value)
{
    QString key_msg;
    QString msg;
    QString value_msg;
    this->m_index_num = index;
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
                    0, KEY_READ, &this->m_h_key) != 0)
    {
        QString error = "can not open regedit";
        qDebug() << error;
        return GETSERIALPORTERROR;
    }
    this->m_key_size = sizeof(this->m_key_name);
    this->m_value_size = sizeof(this->m_key_value);
    if(RegEnumValue(this->m_h_key,  this->m_index_num, this->m_key_name, &this->m_key_size,
                    0, &this->m_type, (BYTE*)this->m_key_value, &this->m_value_size) == 0)
    {
        for (int i=0; i<(int)this->m_key_size; i++)
        {
            msg = this->m_key_name[i];
            key_msg.append(msg);
        }

        for (int j=0; j<(int)this->m_value_size; j++)
        {
             if (this->m_key_value[j] != 0x00)
             {
                 value_msg.append(this->m_key_value[j]);
             }
        }

        if(key_or_value == "key")
        {
            this->m_comm_result = key_msg;
        }

        if(key_or_value == "value")
        {
            this->m_comm_result = value_msg;
        }
    }
    else
    {
        this->m_comm_result = "nokey";
        qDebug() << "there is no key or value in the Settings";
        return GETSERIALPORTERROR;
    }
    RegCloseKey(this->m_h_key);
    return 0;
}

bool SerialPort::openPort(char* portName)
{
    this->m_hle_comm = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
    if(this->m_hle_comm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    return true;
}

bool SerialPort::initSerilPort(char *portName, UINT baud , char parity,
    UINT databits , UINT stopsbits)
{
    char szDCBparam[50];
    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);
    if (!openPort(portName))
    {
        return false;
    }
    BOOL is_success = TRUE;
    if(is_success)
    {
       is_success = SetupComm(this->m_hle_comm, 10, 10);
    }
    COMMTIMEOUTS CommTimeouts;
    CommTimeouts.ReadIntervalTimeout = 0;
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    CommTimeouts.ReadTotalTimeoutConstant = this->m_read_time_out;
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;
    CommTimeouts.WriteTotalTimeoutConstant = this->m_write_time_out;
    if(is_success)
    {
        is_success = SetCommTimeouts(this->m_hle_comm, &CommTimeouts);
    }
    DCB  dcb;
    if(is_success)
    {
        DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
        wchar_t *pwText = new wchar_t[dwNum];
        if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
        {
            is_success = TRUE;
        }
        is_success = GetCommState(m_hle_comm, &dcb) && BuildCommDCB(pwText, &dcb);
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
        delete[] pwText;
    }
    if(is_success)
    {
        is_success = SetCommState(m_hle_comm, &dcb);
    }
    PurgeComm(m_hle_comm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    return is_success == TRUE;
}

bool SerialPort::initSerilPort(char* portName, const LPDCB& plDCB)
{
    if (!openPort(portName))
    {
        return false;
    }
    if (!SetCommState(m_hle_comm, plDCB))
    {
        return false;
    }
    PurgeComm(this->m_hle_comm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    return true;
}

UINT SerialPort::GetBytesFromCOM()
{
    DWORD dwError = 0;
    COMSTAT  comstat;
    memset(&comstat, 0, sizeof(COMSTAT));
    UINT BytesInQue = 0;
    if(ClearCommError(this->m_hle_comm, &dwError, &comstat))
    {
        BytesInQue = comstat.cbInQue;
    }
    return BytesInQue;
}

int SerialPort::readData(char *msg, int msg_len)
{
    BOOL      b_ret      = TRUE;
    DWORD     byte_read  = 0;
    DWORD     d_error    = 0;
    COMSTAT   stat;
    char buf[64]         = { 0 };
    if(this->m_hle_comm == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    BOOL rtn = PurgeComm(this->m_hle_comm, PURGE_TXCLEAR|PURGE_RXCLEAR );
    if(!rtn)
    {
        return -1;
    }
    rtn = ClearCommError(this->m_hle_comm, &d_error, &stat);
    if(!rtn)
    {
        return -1;
    }
    b_ret = ReadFile(this->m_hle_comm, &buf, msg_len, &byte_read, NULL);
    if((!b_ret))
    {
        PurgeComm(this->m_hle_comm, PURGE_RXCLEAR | PURGE_RXABORT);
        return -1;
    }

    for(size_t i = 0; i < sizeof((char*)buf); ++i )
    {
       printf(" %02X", (char*)buf[i]);
    }


    memcpy(msg, buf, msg_len);
    return 0;
}

int SerialPort::writeData(const SerialPort::Data& data)
{
    BOOL rtn = PurgeComm(this->m_hle_comm, PURGE_TXCLEAR|PURGE_RXCLEAR );
    if(!rtn)
    {
        return -1;
    }
    DWORD d_error = 0;
    COMSTAT stat;
    rtn = ClearCommError(this->m_hle_comm, &d_error, &stat);
    if(!rtn)
    {
        return -1;
    }
    DWORD len = 0;
    rtn = WriteFile(this->m_hle_comm, data.buffer, data.length, &len, NULL );
    if(!rtn)
    {
        return -1;
    }
    return 0;
}

void SerialPort::ClosePort()
{
    if(this->m_hle_comm != INVALID_HANDLE_VALUE)
    {
        CloseHandle(this->m_hle_comm);
        this->m_hle_comm = INVALID_HANDLE_VALUE;
    }
}
