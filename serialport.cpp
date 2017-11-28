#include "SerialPort.h"
#include <process.h>
#include <iostream>

bool SerialPort::s_bExit = false;

const UINT SLEEP_TIME_INTERVAL = 5;

SerialPort::SerialPort(void) : m_hListenThread(INVALID_HANDLE_VALUE)
{
    m_hComm = INVALID_HANDLE_VALUE;
    m_hListenThread = INVALID_HANDLE_VALUE;
    InitializeCriticalSection(&m_csCommunicationSync);
}

SerialPort::~SerialPort(void)
{
    CloseListenTread();
    ClosePort();
    DeleteCriticalSection(&m_csCommunicationSync);
}

bool SerialPort::InitPort(UINT portNo, UINT baud, char parity, UINT databits, UINT stopsbits, DWORD dwCommEvents)
{
    DCB  dcb;
    char szDCBparam[50];
    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

    if(!openPort(portNo))
    {
        return false;
    }
    EnterCriticalSection(&m_csCommunicationSync);
    BOOL bIsSuccess = TRUE;

    COMMTIMEOUTS  CommTimeouts;
    CommTimeouts.ReadIntervalTimeout = 0;
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    CommTimeouts.ReadTotalTimeoutConstant = 0;
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;
    CommTimeouts.WriteTotalTimeoutConstant = 0;
    if (bIsSuccess)
    {
       bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);
    }
    if (bIsSuccess)
    {
        DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
        wchar_t *pwText = new wchar_t[dwNum];
        if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
        {
            bIsSuccess = TRUE;
        }

        bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb);
        dcb.fRtsControl = RTS_CONTROL_ENABLE;

        delete[] pwText;
    }
    if (bIsSuccess)
    {
        bIsSuccess = SetCommState(m_hComm, &dcb);
    }
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    LeaveCriticalSection(&m_csCommunicationSync);
    return bIsSuccess == TRUE;
}

bool SerialPort::InitPort(UINT portNo, const LPDCB& plDCB)
{
    if (!openPort(portNo))
    {
        return false;
    }

    EnterCriticalSection(&m_csCommunicationSync);

    if (!SetCommState(m_hComm, plDCB))
    {
        return false;
    }

    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

    LeaveCriticalSection(&m_csCommunicationSync);

    return true;
}

void SerialPort::ClosePort()
{
    if (m_hComm != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hComm);
        m_hComm = INVALID_HANDLE_VALUE;
    }
}

bool SerialPort::openPort(UINT portNo)
{
    EnterCriticalSection(&m_csCommunicationSync);
    char szPort[50];
    sprintf_s(szPort, "COM%d", portNo);
    m_hComm = CreateFileA(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        LeaveCriticalSection(&m_csCommunicationSync);
        return false;
    }
    LeaveCriticalSection(&m_csCommunicationSync);
    return true;
}

bool SerialPort::OpenListenThread()
{
    UINT threadId;
    if (m_hListenThread != INVALID_HANDLE_VALUE)
    {
       return false;
    }
    s_bExit = false;
    m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);
    if (!m_hListenThread)
    {
       return false;
    }
    if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))
    {
       return false;
    }
    return true;
}

bool SerialPort::CloseListenTread()
{
    if(m_hListenThread != INVALID_HANDLE_VALUE)
    {
        s_bExit = true;
        Sleep(10);
        CloseHandle(m_hListenThread);
        m_hListenThread = INVALID_HANDLE_VALUE;
    }
    return true;
}

UINT SerialPort::GetBytesInCOM()
{
    DWORD dwError = 0;
    COMSTAT  comstat;
    memset(&comstat, 0, sizeof(COMSTAT));
    UINT BytesInQue = 0;
    if(ClearCommError(m_hComm, &dwError, &comstat))
    {
       BytesInQue = comstat.cbInQue;
    }
    return BytesInQue;
}

UINT WINAPI SerialPort::ListenThread(void* pParam)
{
    char cRecved = 0x00;
    SerialPort *pSerialPort = reinterpret_cast<SerialPort*>(pParam);
    while (!pSerialPort->s_bExit)
    {
        UINT BytesInQue = pSerialPort->GetBytesInCOM();
        if (BytesInQue == 0)
        {
            Sleep(SLEEP_TIME_INTERVAL);
            continue;
        }
        do
        {
            cRecved = 0x00;
            if (pSerialPort->ReadChar(cRecved) == true)
            {
                std::cout << cRecved;
                continue;
            }
        } while (--BytesInQue);
    }

    return 0;
}

bool SerialPort::ReadChar(char &cRecved)
{
    BOOL  bResult = TRUE;
    DWORD BytesRead = 0;
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    EnterCriticalSection(&m_csCommunicationSync);
    bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);
    if ((!bResult))
    {
        DWORD dwError = GetLastError();
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
        LeaveCriticalSection(&m_csCommunicationSync);
        return false;
    }
    LeaveCriticalSection(&m_csCommunicationSync);
    return (BytesRead == 1);
}

bool SerialPort::WriteData(unsigned char* pData, unsigned int length)
{
    BOOL   bResult = TRUE;
    DWORD  BytesToSend = 0;
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    EnterCriticalSection(&m_csCommunicationSync);
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);
    if (!bResult)
    {
        DWORD dwError = GetLastError();
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
        LeaveCriticalSection(&m_csCommunicationSync);

        return false;
    }
    LeaveCriticalSection(&m_csCommunicationSync);
    return true;
}
