#include "SerialPort.h"
#include <iostream>

SerialPort::SerialPort()
{
    this->m_set = new QSettings(g_path, QSettings::NativeFormat);
    QStringList key = this->m_set->allKeys();
    this->m_set_len = key.size();
}

SerialPort::~SerialPort()
{  
    delete this->m_set;
    this->ClosePort();
}

std::string SerialPort::GetSystemFunctionErrorMessage()
{
    DWORD rtn = GetLastError();

    char* message = NULL;
    DWORD ret = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, rtn, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), (LPTSTR) &message,  0, NULL );
    if ( ret == 0 )
    {
        return "";
    }
    return std::string(message);
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

bool SerialPort::openPort(UINT portNo)
{
    char szPort[50];
    sprintf_s(szPort, "COM%d", portNo);
    m_hle_comm = CreateFileA(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
    if (m_hle_comm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    return true;
}

bool SerialPort::initSerilPort(UINT portNo, UINT baud , char parity,
    UINT databits , UINT stopsbits)
{
    char szDCBparam[50];
    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);
    if (!openPort(portNo))
    {
        return false;
    }
    BOOL bIsSuccess = TRUE;
    if(bIsSuccess )
    {
       bIsSuccess = SetupComm(m_hle_comm, 10, 10);
    }
    COMMTIMEOUTS  CommTimeouts;
    CommTimeouts.ReadIntervalTimeout = 0;
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    CommTimeouts.ReadTotalTimeoutConstant = this->m_read_time_out;
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;
    CommTimeouts.WriteTotalTimeoutConstant = this->m_write_time_out;
    if (bIsSuccess)
    {
        bIsSuccess = SetCommTimeouts(m_hle_comm, &CommTimeouts);
    }
    DCB  dcb;
    if (bIsSuccess)
    {
        DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
        wchar_t *pwText = new wchar_t[dwNum];
        if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
        {
            bIsSuccess = TRUE;
        }
        bIsSuccess = GetCommState(m_hle_comm, &dcb) && BuildCommDCB(pwText, &dcb);
        dcb.fRtsControl = RTS_CONTROL_ENABLE; //开启RTS flow控制
        delete[] pwText;
    }

    if (bIsSuccess)
    {
        bIsSuccess = SetCommState(m_hle_comm, &dcb);
    }

    PurgeComm(m_hle_comm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

    return bIsSuccess == TRUE;
}

bool SerialPort::initSerilPort(UINT portNo, const LPDCB& plDCB)
{
    if (!openPort(portNo))
    {
        return false;
    }
    if (!SetCommState(m_hle_comm, plDCB))
    {
        return false;
    }
    PurgeComm(m_hle_comm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    return true;
}

int SerialPort::readData(SerialPort::Data& data)
{
    BOOL rtn = PurgeComm(this->m_hle_comm, PURGE_TXCLEAR|PURGE_RXCLEAR);
    if (!rtn)
    {
        qDebug() << "PurgeComm error !";
        return -1;
    }
    DWORD derror = 0;
    COMSTAT stat;
    rtn = ClearCommError(this->m_hle_comm, &derror, &stat);
    if (!rtn)
    {
        qDebug() << "ClearCommError error !";
        return -1;
    }
    DWORD len = 0;
    rtn = ReadFile(this->m_hle_comm, this->m_read_buffer, this->m_read_buffer_cap,
        &len, NULL);
    if (!rtn)
    {
        qDebug() << "ReadFile error !";
        return -1;
    }
    data.buffer = this->m_read_buffer;
    data.length = len;
    return 0;
}

bool SerialPort::readData(char *recv_buf, int recv_len)
{
    BOOL  bResult   = TRUE;
    DWORD BytesRead = 0;
    DWORD derror    = 0;
    COMSTAT stat;
    char buf[9];

    /*
    memset(recv_buf, 0x00, sizeof(recv_buf));
    recv_buf = (char*)malloc(recv_len);
    if(!recv_buf)
    {
        std::cout << "malloc recv_buf error" << endl;
    }
    */

    if (m_hle_comm == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    BOOL rtn = PurgeComm(this->m_hle_comm, PURGE_TXCLEAR|PURGE_RXCLEAR );
    if (!rtn)
    {
        return false;
    }

    rtn = ClearCommError(this->m_hle_comm, &derror, &stat );
    if (!rtn)
    {
        return false;
    }
    bResult = ReadFile(m_hle_comm, &buf, 9, &BytesRead, NULL);
    if((!bResult))
    {
        PurgeComm(m_hle_comm, PURGE_RXCLEAR | PURGE_RXABORT);
        return false;
    }
    //0F 0F 02 04 02 02 03 04 2F
    //0F 0F 02 04 01 02 03 04 2E
    for (size_t i = 0; i < sizeof(buf); ++i)
    {
        printf("%02X ", (char*)buf[i]);
    }
    return (BytesRead == 1);
}

int SerialPort::writeData(const SerialPort::Data& data)
{
    BOOL rtn = PurgeComm(this->m_hle_comm, PURGE_TXCLEAR|PURGE_RXCLEAR );
    if (!rtn)
    {
        return -1;
    }
    DWORD derror = 0;
    COMSTAT stat;
    rtn = ClearCommError(this->m_hle_comm, &derror, &stat );
    if (!rtn)
    {
        return -1;
    }
    DWORD len = 0;
    rtn = WriteFile(this->m_hle_comm, data.buffer, data.length, &len, NULL );
    if (!rtn)
    {
        return -1;
    }
    return 0;
}

void SerialPort::ClosePort()
{
    if(m_hle_comm != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hle_comm);
        m_hle_comm = INVALID_HANDLE_VALUE;
    }
}
