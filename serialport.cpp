/*
#include "serialport.h"

SerialPort::SerialPort(size_t rsize, size_t wsize) : read_buffer_cap_(rsize), read_time_out_(3), write_time_out_(3)
{
    this->read_buffer_ = new unsigned char(rsize);
}

SerialPort::~SerialPort()
{
    this->Close();
}

int SerialPort::Open(const char* name)
{
    this->handle_ = CreateFile(name, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
    if (INVALID_HANDLE_VALUE == this->handle_)
    {
        return ERROR_SYSTEM_FUNCTION;
    }
    return ERROR_SUCCESS_EXT;
}

void SerialPort::Close()
{
    if (NULL != this->handle_)
    {
        CloseHandle(this->handle_);
    }
    this->handle_ = NULL;
}

int SerialPort::Setup()
{
    if (NULL == this->handle_)
    {
        return ERROR_PROGRAM_LOGIC;
    }
    BOOL rtn = SetupComm(this->handle_, 1024, 1024);
    if(!rtn)
    {
        return ERROR_SYSTEM_FUNCTION;
    }

    COMMTIMEOUTS timeout;
    timeout.ReadIntervalTimeout = 0;
    timeout.ReadTotalTimeoutMultiplier = 0;
    timeout.ReadTotalTimeoutConstant = this->read_time_out_;
    timeout.WriteTotalTimeoutMultiplier = 0;
    timeout.WriteTotalTimeoutConstant  = this->write_time_out_;
    rtn = SetCommTimeouts(this->handle_, &timeout);
    if (!rtn)
    {
        return ERROR_SYSTEM_FUNCTION;
    }
    return ERROR_SUCCESS_EXT;
}

int SerialPort::ReadData(SerialPort::Data& data)
{
    BOOL rtn = PurgeComm(this->handle_, PURGE_TXCLEAR|PURGE_RXCLEAR);
    if (!rtn)
    {
        return ERROR_SYSTEM_FUNCTION;
    }
    DWORD derror = 0;
    COMSTAT stat;
    rtn = ClearCommError(this->handle_, &derror, &stat);
    if (!rtn)
    {
        return ERROR_SYSTEM_FUNCTION;
    }
    DWORD len = 0;
    rtn = ReadFile(this->handle_, this->read_buffer_, this->read_buffer_cap_, &len, NULL);
    if (!rtn)
    {
        return ERROR_SYSTEM_FUNCTION;
    }
    data.buffer = this->read_buffer_;
    data.length = len;
    return ERROR_SUCCESS_EXT;
}

int SerialPort::WriteData(const SerialPort::Data& data)
{
    BOOL rtn = PurgeComm(this->handle_, PURGE_TXCLEAR|PURGE_RXCLEAR);
    if (!rtn)
    {
        return ERROR_SYSTEM_FUNCTION;
    }
    DWORD derror = 0;
    COMSTAT stat;
    rtn = ClearCommError( this->handle_, &derror, &stat );
    if (!rtn)
    {
        return ERROR_SYSTEM_FUNCTION;
    }
    DWORD len = 0;
    rtn = WriteFile( this->handle_, data.buffer, data.length, &len, NULL );
    if (!rtn )
    {
        return ERROR_SYSTEM_FUNCTION;
    }
    return ERROR_SUCCESS_EXT;
}
*/


