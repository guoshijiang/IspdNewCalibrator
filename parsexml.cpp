#include "parsexml.h"

ParseXml::ParseXml()
{

}

ParseXml::~ParseXml()
{

}

int ParseXml::ParseConfigListXml(QString xml_path)
{
    QFile file(xml_path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader reader(&file);
        while(!reader.atEnd())
        {
            if(reader.isStartElement())
            {
                if(reader.name() == "BaudRate")
                {
                    this->m_sport_con.baud_rate.append(QString("%1").arg(reader.readElementText()));
                }
                else if(reader.name() == "DataBit")
                {
                    this->m_sport_con.data_bit.append(QString("%1").arg(reader.readElementText()));
                }
                else if(reader.name() == "CheckDigit")
                {
                    this->m_sport_con.check_dit.append(QString("%1").arg(reader.readElementText()));
                }
                else if(reader.name() == "StopBit")
                {
                    this->m_sport_con.stop_bit.append(QString("%1").arg(reader.readElementText()));
                }
             }
             reader.readNext();
        }
        file.close();
    }
    else
    {
        std::cout << "open xml file failure" << std::endl;
        return PARSECONFIGXMLERR;
    }
    return 0;
}

int ParseXml::ParseConfigXml(QString xml_path)
{
    QFile file(xml_path);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader reader(&file);
        while(!reader.atEnd())
        {
            if(reader.isStartElement())
            {
                if(reader.name() == "BaudRate")
                {
                    this->m_baud_rate = (QString("%1").arg(reader.readElementText()));
                }
                else if(reader.name() == "DataBit")
                {
                    this->m_data_bit =  (QString("%1").arg(reader.readElementText()));
                }
                else if(reader.name() == "CheckDigit")
                {
                    this->m_check_dit = (QString("%1").arg(reader.readElementText()));
                }
                else if(reader.name() == "StopBit")
                {
                    this->m_stop_bit = (QString("%1").arg(reader.readElementText()));
                }
             }
             reader.readNext();
        }
        file.close();
    }
    else
    {
        std::cout << "open xml file failure" << std::endl;
        return PARSECONFIGXMLERR;
    }
    return 0;
}

int ParseXml::WriteConfigXml(QString xml_path)
{
    QFile file(xml_path);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.setAutoFormattingIndent(2);
        writer.writeStartDocument();
        writer.writeStartElement("SerialPort");
        writer.writeStartElement("PortNode");
        writer.writeAttribute("Option", "checked");
        writer.writeTextElement("BaudRate", this->m_dcinfo.baud_rate);
        writer.writeTextElement("DataBit",this->m_dcinfo.data_bit);
        writer.writeTextElement("CheckDigit",this->m_dcinfo.check_dit);
        writer.writeTextElement("StopBit",this->m_dcinfo.stop_bit);
        writer.writeEndElement();
        writer.writeEndElement();
        writer.writeEndDocument();
        file.close();
    }
    else
    {
        std::cout <<"Open File Failure" << std::endl;
        return WRITERXMLERR;
    }
    return 0;
}


