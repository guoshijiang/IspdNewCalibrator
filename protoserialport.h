#ifndef PROTOSERIALPORT_H
#define PROTOSERIALPORT_H

#include <QString>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QDataStream>
#include <QIODevice>
#include <QVector>

namespace protoserialport
{
    enum REQ_TYPE
    {
        ISPD_MODULE_CUSTOMISE = 0x34,    //用于工厂配置护腿板各项参数
        ISPD_MODULE_APPLICATE = 0x35,    //用于工厂配置结束恢复到正常应用程序状态
        ISPD_WR_ID = 0x30,               //配置标签ID
        ISPD_WR_HZ = 0x31,               //配置采样率
        ISPD_RD_ID = 0x01,               //获取标签指令
        ISPD_RD_HZ = 0x36,               //查询采样率
        ISPD_RD_HR = 0x37,               //读取心率指令
        ISPD_RD_ALL = 0x38,              //一键获取指令
        ISPD_RD_VERSION = 0x32,          //获取版本号指令集
        ISPD_RD_SENSOR = 0x33            //查询传感器和电量值
    };

    enum REP_TYPE
    {
        REP_ERR = 0X00,     //帧类型出错
        REP_WR_RES = 0x01,  //配置结果
        REP_ISPD_ID = 0x02,
        REP_ISPD_HZ = 0x05,
        REP_ISPD_HR = 0x06,
        REP_ISPD_ALL = 0x07,
        REP_ISPD_VERSION = 0x03,
        REP_ISPD_SERSOR = 0x04,
    };

    enum ERR_TYPE
    {
        ERR_CRC = 0x01,
        ERR_FRM = 0x02,
    };

    enum RES_TYPE
    {
        RES_SUCCESS = 0x00,
        RES_FAILURE = 0x01,
    };

    class IspdID
    {
    public:
        IspdID(quint32 id, REQ_TYPE type);
        QByteArray toBinary();
        quint32     getID() const;
        REQ_TYPE    getType() const;
        quint8      getLength() const;
        quint8      getCrc() const;
        IspdID(const IspdID & ispdID);
        IspdID &operator=(const IspdID & ispdId);
    private:
        quint32     m_id;
        REQ_TYPE    m_type;
    };

    class IspdHZ
    {
    public:
        IspdHZ(quint8 hz, REQ_TYPE type);
        QByteArray toBinary();
        quint32     getHZ() const {return m_hz;}
        REQ_TYPE    getType() const {return m_type;}
        quint8      getLength() const {return sizeof(m_hz);}
        quint8      getCrc() const {return m_hz;}
        IspdHZ(const IspdHZ & ispdHZ);
        IspdHZ &operator=(const IspdHZ & ispdHZ);

    private:
        quint8      m_hz;
        REQ_TYPE    m_type;
    };

    class IspdHR
    {
    public:
        IspdHR(quint8 hr, REQ_TYPE type);
        QByteArray toBinary();
        quint32     getHR() const { return m_hr; }
        quint8      getLength() const { return sizeof(m_hr); }
        quint8      getCrc() const { return m_hr; }
        IspdHR(const IspdHR & ispdHR);
        IspdHR &operator =(const IspdHR & ispdHR);

    private:
        quint8      m_hr;
        REQ_TYPE    m_type;
    };

    class ReqPkg
    {
    public:
        ReqPkg(REQ_TYPE type);
        virtual QByteArray toBinary();
        virtual void setCrc();
        virtual void setLength();

    protected:
        quint16  m_header;
        quint16  m_tailer;
        quint8   m_type;
        quint8   m_length;
        quint8   m_crc;
    };

    template <typename T>
    class ReqMsg : public ReqPkg
    {
    public:
        ReqMsg(const T & data) : m_data(data), ReqPkg(data.getType()) {
            setLength();
            setCrc();
        }

        QByteArray toBinary() override
        {
            QByteArray res;
            QDataStream data(&res, QIODevice::WriteOnly);
            QByteArray mid = m_data.toBinary();

            data << m_header
                 << m_type
                 << m_data.getLength();
            data.writeRawData(mid.data(), mid.length());// data
            data << m_crc // crc
                 << m_tailer;

            return res;
        }
        void setCrc() override
        {
            m_crc = (m_header >> 8) + m_header
                    + m_type + m_length + m_data.getCrc();
        }
        void setLength() override
        {
            m_length = m_data.getLength();
        }
    private:
        T m_data;
    };

    //传感器
    class Sensor
    {
    public:
        Sensor(const char * str, size_t length)
            : m_ac_x(0)
            , m_ac_y(0)
            , m_ac_z(0)
            , m_gy_x(0)
            , m_gy_y(0)
            , m_gy_z(0)
            , m_hb(0)
            , m_pw(0)
        {
            if (16 == length)
            {
                m_ac_x = static_cast<quint8>(str[0]) | (str[1] << 8);
                m_ac_y = static_cast<quint8>(str[2]) | (str[3] << 8);
                m_ac_z = static_cast<quint8>(str[4]) | (str[5] << 8);
                m_gy_x = static_cast<quint8>(str[6]) | (str[7] << 8);
                m_gy_y = static_cast<quint8>(str[8]) | (str[9] << 8);
                m_gy_z = static_cast<quint8>(str[10]) | (str[11] << 8);
                m_hb = static_cast<quint8>(str[12]) | (str[13] << 8);
                m_pw = static_cast<quint8>(str[14]) | (str[15] << 8);
            }
        }

        qint16 get_ac_x() {return m_ac_x;}
        qint16 get_ac_y() {return m_ac_y;}
        qint16 get_ac_z() {return m_ac_z;}
        qint16 get_gy_x() {return m_gy_x;}
        qint16 get_gy_y() {return m_gy_y;}
        qint16 get_gy_z() {return m_gy_z;}
        quint16 get_hb() {return m_hb;}
        quint16 get_pw() {return m_pw;}

        quint8 getCrc()
        {
            return static_cast<quint8>(m_ac_x >> 8) + static_cast<quint8>(m_ac_x)
                    + static_cast<quint8>(m_ac_y >> 8) + static_cast<quint8>(m_ac_y)
                    + static_cast<quint8>(m_ac_z >> 8) + static_cast<quint8>(m_ac_z)
                    + static_cast<quint8>(m_gy_x >> 8) + static_cast<quint8>(m_gy_x)
                    + static_cast<quint8>(m_gy_y >> 8) + static_cast<quint8>(m_gy_y)
                    + static_cast<quint8>(m_gy_z >> 8) + static_cast<quint8>(m_gy_z)
                    + static_cast<quint8>(m_hb >> 8) + static_cast<quint8>(m_hb)
                    + static_cast<quint8>(m_pw >> 8) + static_cast<quint8>(m_pw);
        }

        friend QDataStream & operator >> (QDataStream & in, Sensor & res)
        {
            in >> res.m_ac_x >> res.m_ac_y >> res.m_ac_z >> res.m_gy_x >> res.m_gy_y >> res.m_gy_z >> res.m_hb >> res.m_pw;
            return in;
        }

    private:
        qint16 m_ac_x;
        qint16 m_ac_y;
        qint16 m_ac_z;
        qint16 m_gy_x;
        qint16 m_gy_y;
        qint16 m_gy_z;
        quint16 m_hb;
        quint8 m_pw;
    };

    class ResResult
    {
    public:
        ResResult(const char * str, size_t length)
            : m_res(0)
        {
            if (1 == length)
                m_res = str[0];
        }

        qint8 getRes() {return m_res;}

        quint8 getCrc()
        {
            return static_cast<quint8>(m_res);
        }

        friend QDataStream & operator >> (QDataStream & in, ResResult & res)
        {
            in >> res.m_res;
            return in;
        }

    private:
        qint8 m_res;
    };

    class ResHZ
    {
    public:
        ResHZ(const char * str, size_t length)
            : m_hz(0)
        {
            if (1 == length)
                m_hz = str[0];
        }

        qint8 getHZ() {return m_hz;}

        quint8 getCrc()
        {
            return static_cast<quint8>(m_hz);
        }

        friend QDataStream & operator >> (QDataStream & in, ResHZ & res)
        {
            in >> res.m_hz;
            return in;
        }

    private:
        qint8 m_hz;
    };

    class ResHR
    {
    public:
        ResHR(const char * str, size_t length)
            : m_hr1(0), m_hr2(0), m_hr3(0)
        {
            if (3 == length)
            {
                m_hr1 = str[0];
                m_hr2 = str[1];
                m_hr3 = str[2];
            }
        }

        quint8 getHR1() const { return m_hr1; }
        quint8 getHR2() const { return m_hr2; }
        quint8 getHR3() const { return m_hr3; }

        quint8 getCrc()
        {
            return static_cast<quint8>(m_hr1)
                    + static_cast<quint8>(m_hr2)
                    + static_cast<quint8>(m_hr3);
        }

        friend QDataStream & operator >> (QDataStream & in, ResHR & res)
        {
            in >> res.m_hr1 >> res.m_hr2 >> res.m_hr3;
            return in;
        }

    private:
        quint8 m_hr1;
        quint8 m_hr2;
        quint8 m_hr3;
    };

    class ResHR2 {
    public:
        ResHR2(const char * str, size_t length)
            : m_hr(0)
        {
            if (1 == length)
            {
                m_hr = str[0];
            }
        }

        quint8 get_hr() const { return m_hr; }

        quint8 getCrc() const { return m_hr; }

        friend QDataStream & operator >> (QDataStream & in, ResHR2 & res)
        {
            in >> res.m_hr;
            return in;
        }

    private:
        quint8 m_hr;
    };

    class ResID
    {
    public:
        ResID(const char * str, size_t length)
            : m_id(0)
        {
            if (4 == length)
            {
                m_id = static_cast<quint8>(str[0]) | ( static_cast<quint8>(str[1]) << 8 )
                        | ( static_cast<quint8>(str[2]) << 16 ) | (static_cast<quint8>(str[3]) << 24);
            }
        }

        quint32 getID() { return m_id;}

        quint8 getCrc()
        {
            return static_cast<quint8>(m_id)
                    + static_cast<quint8>(m_id >> 8)
                    + static_cast<quint8>(m_id >> 16)
                    + static_cast<quint8>(m_id >> 24);

        }

        friend QDataStream & operator >> (QDataStream & in, ResID & res)
        {
            in >> res.m_id;
            return in;
        }

    private:
        quint32 m_id;
    };

    class ResVersion {
    public:
        ResVersion(const char * str, size_t length)
            : m_ispd_version(0)
            , m_uwb_version(0)
            , m_sp_version(0)
        {
            if (3 == length)
            {
                m_ispd_version = str[0];
                m_uwb_version = str[1];
                m_sp_version = str[2];
            }
        }

        quint8 get_ispd_version() {return m_ispd_version;}
        quint8 get_uwb_version() {return m_uwb_version;}
        quint8 get_sp_version() {return m_sp_version;}

        quint8 getCrc()
        {
            return static_cast<quint8>(m_ispd_version)
                    + static_cast<quint8>(m_uwb_version)
                    + static_cast<quint8>(m_sp_version);
        }

        friend QDataStream & operator >> (QDataStream & in, ResVersion & res)
        {
            in >> res.m_ispd_version >> res.m_uwb_version >> res.m_sp_version;
            return in;
        }

    private:
        quint8 m_ispd_version;
        quint8 m_uwb_version;
        quint8 m_sp_version;
    };

    class ResAll
    {
    public:
        ResAll(const char * str, size_t length)
            : m_id(0)
            , m_hz(0)
            , m_ispd_version(0)
            , m_uwb_version(0)
            , m_sp_version(0)
        {
            if (8 == length)
            {
                m_id = static_cast<quint8>(str[0])
                        | ( static_cast<quint8>(str[1]) << 8 )
                        | ( static_cast<quint8>(str[2]) << 16 )
                        | ( static_cast<quint8>(str[3]) << 24);
                m_hz = str[4];
                m_ispd_version = str[5];
                m_uwb_version = str[6];
                m_sp_version = str[7];
            }
        }

        quint32 getID() { return m_id;}
        quint8 getHZ() {return m_hz;}
        quint8 get_ispd_version() {return m_ispd_version;}
        quint8 get_uwb_version() {return m_uwb_version;}
        quint8 get_sp_version() {return m_sp_version;}

        quint8 getCrc()
        {
            return  static_cast<quint8>(m_id)
                    + static_cast<quint8>(m_id >> 8)
                    + static_cast<quint8>(m_id >> 16)
                    + static_cast<quint8>(m_id >> 24)
                    + static_cast<quint8>(m_hz)
                    + static_cast<quint8>(m_ispd_version)
                    + static_cast<quint8>(m_uwb_version)
                    + static_cast<quint8>(m_sp_version);
        }

        friend QDataStream & operator >> (QDataStream & in, ResAll & res)
        {
            in >> res.m_id >> res.m_hz >> res.m_ispd_version >> res.m_uwb_version >> res.m_sp_version;
            return in;
        }


    private:
        quint32 m_id;
        quint8 m_hz;
        quint8 m_ispd_version;
        quint8 m_uwb_version;
        quint8 m_sp_version;
    };

    template <typename T>
    class ResPkg
    {
    public:
        ResPkg(const char * str, size_t length)
            : m_pkg(str, length)
            , m_bin(&m_pkg, QIODevice::ReadOnly)
            , m_data(&str[4], sizeof(T))
        {
            T data(str, length);
            m_bin >> m_header >> m_type >> m_length >> data >> m_crc;
        }

        bool isValid() { return calcCrc() == m_crc;}
        quint16 getHeader() { return m_header;}
        quint8  getType() { return m_type;}
        quint8  getLength() {return m_length;}
        quint8  getCrc() {return m_crc;}

        T getData() { return m_data;}

        quint8  calcCrc()
        {
            return static_cast<quint8>(m_header >> 8)
                    + static_cast<quint8>(m_header)
                    + m_type
                    + m_length
                    + m_data.getCrc();
        }

    private:
        QByteArray  m_pkg;
        QDataStream m_bin;
        quint16 m_header;
        quint8  m_type;
        quint8  m_length;
        quint8  m_crc;
        T m_data;
    };
}
