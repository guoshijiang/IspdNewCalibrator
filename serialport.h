#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include <Windows.h>

class SerialPort
{
public:
    SerialPort(void);
    ~SerialPort(void);
public:
    bool InitPort(UINT  portNo = 1, UINT  baud = CBR_9600, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);
    bool InitPort(UINT  portNo, const LPDCB& plDCB);
    bool OpenListenThread();
    bool CloseListenTread();
    bool WriteData(unsigned char* pData, unsigned int length);
    UINT GetBytesInCOM();
    bool ReadChar(char &cRecved);
private:
    bool openPort(UINT  portNo);
    void ClosePort();
    static UINT WINAPI ListenThread(void* pParam);
private:
    HANDLE  m_hComm;
    static bool s_bExit;
    volatile HANDLE    m_hListenThread;
    CRITICAL_SECTION   m_csCommunicationSync;

};
#endif //SERIALPORT_H_
