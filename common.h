#ifndef COMMON_H
#define COMMON_H
#include <QString>
#include <QMessageBox>
#include <QByteArray>

//字体样式的定义
#define MESSAGE_RED "<font color = red ><strong>"
#define MESSAGE_GREEN "<font color = blue ><strong>"
#define MESSAGE_END "</strong></font>"

#define MESSAGE_SUC "<font color=green><strong>"
#define MESSAGE_CED "</strong></font>"

/*
 * 关于错误码的解释：
 *      串口出错以1000开头，后面依次累加
 *      UDP出错以2000开头，后面依次累加
 *      TCP出错以3000开头，后面依次累加
 *      HTTP GET请求出错以4000开头，后面依次累加
 *      HTTP POST请求出错以5000开头，后面依次累加
 *      初始化错误以6000开头，后面依次累加
 *      其他错误以7000开头，后面依次累加
*/

namespace Common
{
    //串口错误码定义
    #define GETSERIALPORTERROR 1000

    //UDP通信错误码定义
    #define UDPERRCODE 2000

    //TCP通信错误码定义
    #define TCPERRCODE 3000

    //HTTP GET请求错误码定义、
    #define HTTPGETREQUESTERRCODE 4000

    //HTTP POST请求错误码定义
    #define HTTPPOSTREQUESTERRCODE 5000

    //初始化错误码定义
    #define INITERRCODE 6000

    //其他错误错误码定义
    #define PARSECONFIGXMLERR 7000
    #define WRITERXMLERR      7001
    #define SERIPORTCINITERR  7002

    //弹框处里函数
    #define HANDLEMSGBOXINFO(info_msg)(QMessageBox::information(this, QString::fromLocal8Bit("信息"), MESSAGE_RED + info_msg + MESSAGE_END))
    #define HANDLEMSGBOXWARNING(warning_msg)(QMessageBox::warning(this, QString::fromLocal8Bit("警告"), MESSAGE_RED + warning_msg + MESSAGE_END))
    #define HANDLEMSGBOXCRITICAL(c_msg)(QMessageBox::critical(this, QString::fromLocal8Bit("严重"), MESSAGE_RED + c_msg + MESSAGE_END))
    #define HANDLEMSGBOXQUESTION(quest_msg)(QMessageBox::question(this, QString::fromLocal8Bit("问题"), MESSAGE_RED + quest_msg + MESSAGE_END))

    //乱码处里宏函数
    #define UTF8BIT(utf8_str)(QString::fromLocal8Bit(utf8_str))

    //QString与char*之间的转换函数(本地字符)
#if 0
    char* QStringToCharP8Bit(QString qstring)
    {
        QByteArray to_data = qstring.toLocal8Bit();
        char* cp_data = to_data.data();
        return cp_data;
    }

    //QString与char*之间的转换函数(拉丁字符)
    char *QStringToCharPLatin1(QString qstring)
    {
        QByteArray to_data = qstring.toLatin1();
        char* cp_data = to_data.data();
        return cp_data;
    }
#endif
}

#endif // COMMON_H
