#ifndef COMMON_H
#define COMMON_H
#include <QString>
#include <QMessageBox>
#include <QByteArray>

#define MESSAGE_RED "<font color=red><strong>"
#define MESSAGE_GREEN "<font color=blue><strong>"
#define MESSAGE_END "</strong></font>"
#define MESSAGE_SUC "<font color=green><strong>"
#define MESSAGE_CED "</strong></font>"

namespace Common
{
    #define HTTP_PREDIX "http://"
    #define UDP_DATA "/udp/data"
    #define UDP_CONTROL "/udp/control"

    #define HTTP_REQEST_PORT "38080"

    #define SEVER_STATUS_INFO 1005
    #define SET_SERVER_AUTO_SYSNC_FUNC 1006
    #define SET_SERVER_MR_AUTO_FACTORYSET 1007
    #define UPDATE_SPEC_MR_SERVER_DOMAIN 1008
    #define SET_SPEC_MR_FACTORYSET 1009
    #define RESTART_SPEC_MR 1010
    #define READ_SPEC_MR_INFO 1011
    #define WRITE_SPEC_MR_CONFIG 1012
    #define JZ_DATA_CODE 2005

    //115200 调式波特率
    #define SUCCESS_CODE 0
    #define NO_AVI_DATA -1002          //服务端报错：没有可用的数据
    #define REQUEST_DATA_ERR -1001     //服务端报错：请求的数据格式错误
    #define CAN_NOT_FIND_MASTERMR -901 //服务端报错：找不到主机站
    #define CAN_NOT_FINE_MR -902       //服务端报错：找不到基站
    #define MR_INNER_TIMEOUT -903      //服务端报错：基站内部超时

    #define GETSERIALPORTERROR 1000
    #define UDPERRCODE 2000
    #define TCPERRCODE 3000
    #define HTTPGETREQUESTERRCODE 4000
    #define HTTPPOSTREQUESTERRCODE 5000
    #define INITERRCODE 6000
    #define PARSECONFIGXMLERR 7000
    #define WRITERXMLERR      7001
    #define SERIPORTCINITERR  7002

    #define HANDLEMSGBOXINFO(info_msg)(QMessageBox::information(this, QString::fromLocal8Bit("信息"), MESSAGE_RED + info_msg + MESSAGE_END))
    #define HANDLEMSGBOXWARNING(warning_msg)(QMessageBox::warning(this, QString::fromLocal8Bit("警告"), MESSAGE_RED + warning_msg + MESSAGE_END))
    #define HANDLEMSGBOXCRITICAL(c_msg)(QMessageBox::critical(this, QString::fromLocal8Bit("严重"), MESSAGE_RED + c_msg + MESSAGE_END))
    #define HANDLEMSGBOXQUESTION(quest_msg)(QMessageBox::question(this, QString::fromLocal8Bit("问题"), MESSAGE_RED + quest_msg + MESSAGE_END))
    #define UTF8BIT(utf8_str)(QString::fromLocal8Bit(utf8_str))

    class HandleError
    {
    public:
        HandleError();
        HandleError(QString http_req_error, QString database_error, QString sport_error);
        void HandleHttpReqError(int err_code);
        ~HandleError();
    public:
        QString m_http_req_error;
        QString m_database_error;
        QString m_sport_error;
    private:
        HandleError(const HandleError &);
        const HandleError & operator = (const HandleError &);
    };
}

#endif
