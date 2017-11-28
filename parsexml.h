#ifndef PARSEXML_H
#define PARSEXML_H
#include <QStringList>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <iostream>
#include <QList>

#include "common.h"

using namespace Common;

typedef struct _SerialPortConfig
{
   QStringList baud_rate;
   QStringList data_bit;
   QStringList check_dit;
   QStringList stop_bit;
}serPortConfig;

typedef struct _DataConfigInfo
{
    QString baud_rate;
    QString data_bit;
    QString check_dit;
    QString stop_bit;
}dcInfo;

class ParseXml
{
public:
    ParseXml();
    ~ParseXml();
    int ParseConfigListXml(QString xml_path);
    int ParseConfigXml(QString xml_path);
    int WriteConfigXml(QString xml_path);

private:
    ParseXml(const ParseXml &);
    const ParseXml & operator = (const ParseXml &);
public:
   serPortConfig m_sport_con;
   dcInfo m_dcinfo;
   QString m_baud_rate;
   QString m_data_bit;
   QString m_check_dit;
   QString m_stop_bit;
};

#endif // PARSEXML_H
