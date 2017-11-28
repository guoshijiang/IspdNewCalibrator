#include "protoserialport.h"

namespace protoserialport
{
    ReqPkg::ReqPkg(REQ_TYPE type) : m_type(type)
    {
        m_header = 0xF0F0;
        m_tailer = 0xFEFE;
        setLength();
        setCrc();
    }

    QByteArray ReqPkg::toBinary()
    {
        QByteArray res;
        QDataStream data(&res, QIODevice::WriteOnly);
        data << m_header << m_type << m_length << m_crc << m_tailer;
        return res;
    }

    void ReqPkg::setCrc()
    {
        m_crc = (m_header >> 8) + m_header + m_type + m_length;
    }

    void ReqPkg::setLength()
    {
        m_length = 0x00;
    }



    IspdID::IspdID(quint32 id, REQ_TYPE type)
        : m_id(id)
        , m_type(type)
    { }

    QByteArray IspdID::toBinary()
    {
        QByteArray res;
        QDataStream data(&res, QIODevice::WriteOnly);
        data << qFromBigEndian(m_id);
        return res;
    }

    quint32 IspdID::getID() const
    {
        return m_id;
    }

    REQ_TYPE IspdID::getType() const
    {
        return m_type;
    }

    quint8 IspdID::getLength() const
    {
        return sizeof(m_id);
    }

    quint8 IspdID::getCrc() const
    {
        return static_cast<quint8>(m_id)
                + static_cast<quint8>(m_id >> 8)
                + static_cast<quint8>(m_id >> 16)
                + static_cast<quint8>(m_id >> 24);
    }

    IspdID::IspdID(const IspdID &ispdID)
    {
        m_id = ispdID.getID();
        m_type = ispdID.getType();
    }

    IspdID &IspdID::operator=(const IspdID &ispdID)
    {
        m_id = ispdID.getID();
        m_type = ispdID.getType();
        return *this;
    }



    IspdHZ::IspdHZ(quint8 hz, REQ_TYPE type)
        : m_hz(hz)
        , m_type(type)
    {

    }

    QByteArray IspdHZ::toBinary()
    {
        QByteArray res;
        QDataStream data(&res, QIODevice::WriteOnly);
        data << qFromBigEndian(m_hz);
        return res;
    }

    IspdHZ::IspdHZ(const IspdHZ &ispdHZ)
    {
        m_hz = ispdHZ.getHZ();
        m_type = ispdHZ.getType();
    }

    IspdHZ &IspdHZ::operator=(const IspdHZ &ispdHZ)
    {
        m_hz = ispdHZ.getHZ();
        m_type = ispdHZ.getType();
        return *this;
    }
}
