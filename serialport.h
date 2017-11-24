/*
#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <Windows.h>
#include <string>

#define ERROR_SUCCESS_EXT 0
#define ERROR_SYSTEM_FUNCTION -1
#define ERROR_PROGRAM_LOGIC -2

std::string GetSystemFunctionErrorMessage()
{
    DWORD rtn = GetLastError();

    char* message = NULL;
    DWORD ret = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, rtn, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), (LPTSTR) &message,  0, NULL );
    if ( ret == 0 )
        return "";
    return std::string(message);
}

class SerialPort
{
public:
    typedef struct data_t
    {
        const unsigned char* buffer;
        size_t length;
    }Data;

public:
    SerialPort( size_t rsize = 1024, size_t wsize = 1024 );
    ~SerialPort();

    int Open( const char* name );
    void Close();

    int Setup();

    //Millisecond
    inline unsigned int GetReadTimeout() const { return this->read_time_out_; }
    inline void SetReadTimeout( unsigned int t = 3000 ) { this->read_time_out_ = t; }

    //Millisecond
    inline unsigned int GetWriteTimeout() const { return this->write_time_out_; }
    inline void SetWriteTimeout( unsigned int t = 3000 ) { this->write_time_out_ = t; }

    int ReadData( SerialPort::Data& data );
    int WriteData( const SerialPort::Data& data );

private:
    SerialPort( const SerialPort& );
    SerialPort& operator = ( const SerialPort& );

private:
    HANDLE handle_;

    unsigned char* read_buffer_;
    size_t read_buffer_cap_;
    unsigned int read_time_out_;
    unsigned int write_time_out_;
};

#endif // SERIALPORT_H
*/
