#ifndef BASEHTTPCLIENT_H
#define BASEHTTPCLIENT_H


#include <QSettings>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QVariant>
#include <functional>
#include <QList>
#include <QPair>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

#include <QMap>
#include "common.h"
#include <iostream>

using namespace std;
using namespace Common;

namespace HttpClient
{
    typedef struct _WriteSpecMr
    {
        int mmid;
        QString lip;
        QString nip;
        QString gip;
        QString dip;
        QString domain;
        int tp;
        int up;
        QString version;
        int nm;
        int ari;
    }write_spec_mr;

    class BaseHttpClient : public QObject
    {
        Q_OBJECT
    public:
        BaseHttpClient();
        ~BaseHttpClient();
        void get(const QString url);
        void post(const QString url, const QByteArray &data);

    protected:
        virtual void requestFinished(QNetworkReply *reply, const QByteArray data, const int statusCode) = 0;

    public slots:
        void serviceRequestFinished(QNetworkReply *reply);

    private:
       QNetworkRequest httpRequest;
       QNetworkAccessManager networkAccessManager;

    private:
       BaseHttpClient(const BaseHttpClient &);
       const BaseHttpClient & operator = (const BaseHttpClient &);
    };

    class UdpControlServerInfoHttpReq : public BaseHttpClient
    {
        Q_OBJECT
    public:
        UdpControlServerInfoHttpReq();
        UdpControlServerInfoHttpReq(QString req_url);
        ~UdpControlServerInfoHttpReq();
        void UdpControlServerInfo(std::function<void(bool, QMap<QString, QVariant>)> callback);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    private:
         std::function<void(bool, QMap<QString, QVariant>)> checkCallback;

    public:
         QString M_UDP_COMTROL_RUL;

    private:
        UdpControlServerInfoHttpReq(const UdpControlServerInfoHttpReq &);
        const UdpControlServerInfoHttpReq & operator = (const UdpControlServerInfoHttpReq &);
    };


    class StartUpMrAutoDomainHttpRequest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        StartUpMrAutoDomainHttpRequest();
        StartUpMrAutoDomainHttpRequest(int req_sync, QString req_url);
        ~StartUpMrAutoDomainHttpRequest();
        void StartUpMrAutoDomain(std::function<void(bool, QMap<QString, int>)> callback);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);


    signals:
        void SendProNumber(QString req_pro);
        void SendRequestFinish(QString f_info);
        void RequestSuccess(QString succ_info);
        void RequestFail(QString err_info);

    private:
        std::function<void(bool, QMap<QString, int>)> checkCallback;

    public:
        int m_get_req_sync;
        QString M_START_UP_RUL;

    private:
        StartUpMrAutoDomainHttpRequest(const StartUpMrAutoDomainHttpRequest &);
        const StartUpMrAutoDomainHttpRequest & operator = (const StartUpMrAutoDomainHttpRequest &);
    };

    class OpenCloseMrAutoFactoryResetHttpRequest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        OpenCloseMrAutoFactoryResetHttpRequest();
        OpenCloseMrAutoFactoryResetHttpRequest(int req_reset, QString req_url);
        ~OpenCloseMrAutoFactoryResetHttpRequest();
        void OpenCloseMrAutoFactoryReset(std::function<void(bool, QMap<QString, int>)> callback);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    private:
        std::function<void(bool, QMap<QString, int>)> checkCallback;

    public:
        int m_get_req_reset;
        QString M_OPEN_CLOSE_RUL;

    private:
        OpenCloseMrAutoFactoryResetHttpRequest(const OpenCloseMrAutoFactoryResetHttpRequest &);
        const OpenCloseMrAutoFactoryResetHttpRequest & operator = (const OpenCloseMrAutoFactoryResetHttpRequest &);
    };

    class SyncSpecMrDomainHttpRequest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        SyncSpecMrDomainHttpRequest();
        SyncSpecMrDomainHttpRequest(QString req_id, QString req_host, QString req_url);
        ~SyncSpecMrDomainHttpRequest();
        void SyncSpecMrDomain(std::function<void(bool, QMap<QString, int>)> callback);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);     

    private:
        std::function<void(bool, QMap<QString, int>)> checkCallback;


    public:
        QString m_get_req_id;
        QString m_get_req_host;
        QString M_SYNC_RUL;

    private:
        SyncSpecMrDomainHttpRequest(const SyncSpecMrDomainHttpRequest &);
        const SyncSpecMrDomainHttpRequest & operator = (const SyncSpecMrDomainHttpRequest &);
    };

    class FactoryResetHttpReqest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        FactoryResetHttpReqest();
        FactoryResetHttpReqest(QString req_id, QString req_url);
        ~FactoryResetHttpReqest();
        void FactoryReset(std::function<void(bool, QMap<QString, int>)> callback);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    private:
        std::function<void(bool, QMap<QString, int>)> checkCallback;

    public:
        QString m_get_req_id;
        QString M_FAXTORY_RESET_URL;

    private:
        FactoryResetHttpReqest(const FactoryResetHttpReqest &);
        const FactoryResetHttpReqest & operator = (const FactoryResetHttpReqest &);
    };


    class RestartSpecMrHttpReqest : public BaseHttpClient
    {
        Q_OBJECT
    public:
         RestartSpecMrHttpReqest();
         RestartSpecMrHttpReqest(QString req_id, QString req_url);
         ~RestartSpecMrHttpReqest();
         void RestartSpecMr(std::function<void(bool, QMap<QString, int>)> callback);

     protected:
         void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);        

     private:
          std::function<void(bool, QMap<QString, int>)> checkCallback;
          QString m_get_req_id;

    public:
          QString M_RESTART_RUL;

    private:
         RestartSpecMrHttpReqest(const RestartSpecMrHttpReqest &);
         const RestartSpecMrHttpReqest & operator = (const RestartSpecMrHttpReqest &);
    };

    class ReadSpecMrConfigInfoHttpReqest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        ReadSpecMrConfigInfoHttpReqest();
        ReadSpecMrConfigInfoHttpReqest(QString req_id, QString read_url);
        ~ReadSpecMrConfigInfoHttpReqest(); 
        void ReadSpecMrConfigInfo(std::function<void(bool, QMap<QString, QVariant>)> callback);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);       

    private:
         std::function<void(bool, QMap<QString, QVariant>)> checkCallback;

    public:
         QString m_get_mid;
         QString M_READ_RUL;

    private:
        ReadSpecMrConfigInfoHttpReqest(const ReadSpecMrConfigInfoHttpReqest &);
        const ReadSpecMrConfigInfoHttpReqest & operator = (const ReadSpecMrConfigInfoHttpReqest &);
    };

    class WriteSpecMrConfigInfoHttpRequest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        WriteSpecMrConfigInfoHttpRequest();
        ~WriteSpecMrConfigInfoHttpRequest();
        void WriteSpecMrConfigInfo(std::function<void(bool, QMap<QString, int>)> callback);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    public:
        write_spec_mr m_wsmr;
        QString M_WRITE_RUL;

    private:
         std::function<void(bool, QMap<QString, int>)> checkCallback;

    private:
        WriteSpecMrConfigInfoHttpRequest(const WriteSpecMrConfigInfoHttpRequest &);
        const WriteSpecMrConfigInfoHttpRequest & operator = (const WriteSpecMrConfigInfoHttpRequest &);
    };

    class GetMrReslutHttpReqest : public BaseHttpClient
    {
    public:
        GetMrReslutHttpReqest();
        ~GetMrReslutHttpReqest();
        void GetMrResult(std::function<void(bool, QMap<QString, QVariant>)> callback);
        void httpRequestUrl(QString url);
        void getIDFromUI(int id);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    public:
        int m_get_tag_id;
        QString M_RUL;

    private:
         std::function<void(bool, QMap<QString, QVariant>)> checkCallback;

    private:
        GetMrReslutHttpReqest(const GetMrReslutHttpReqest &);
        const GetMrReslutHttpReqest & operator = (const GetMrReslutHttpReqest &);
    };
}
#endif // BASEHTTPCLIENT_H

