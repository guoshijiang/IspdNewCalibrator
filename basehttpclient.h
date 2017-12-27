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

#include <iostream>

using namespace std;

namespace HttpClient
{
    const QString OpenDomainUrl = QString("http://192.168.90.113:8088/api/calibrator/openclosedomain");
    const QString UdpControlUrl = QString("http://192.168.90.113:8088/api/calibrator/getudpsvrdata");
    const QString SpecMrFactoryResetUrl = QString("http://192.168.90.113:8088/api/calibrator/openclosedomain");
    const QString SyncSpecMrDomainUrl = QString("http://192.168.90.113:8088/api/calibrator/syncdomain");
    const QString FactoryResetUrl = QString("http://192.168.90.113:8088/api/calibrator/resetmrfactory");
    const QString RestartSpecUrl = QString("http://192.168.90.113:8088/api/calibrator/restartspecmr");
    const QString ReadSpecMrInfoUrl = QString("http://192.168.90.113:8088/api/calibrator/getspecmrcondata");
    const QString WriteSpecMrInfoUrl = QString("http://192.168.90.113:8088/api/calibrator/setspecmrcondata");
    const QString GetMrResultData = QString("http://192.168.90.113:8088/api/calibrator/mrresultdata");

    typedef struct _WriteSpecMr
    {
        int pro;
        int mmid;
        QString lip;
        QString nip;
        QString gip;
        QString dip;
        QString domain;
        int tp;
        int up;
        int nsm;
        int ar;
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
        ~UdpControlServerInfoHttpReq();
        void UdpControlServerInfo(std::function<void(bool, QMap<QString, QVariant>)> callback);
        void GetReqProNumber(QString req_pro);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    private:
         std::function<void(bool, QMap<QString, QVariant>)> checkCallback;
         QString m_get_req_pro;

    private:
        UdpControlServerInfoHttpReq(const UdpControlServerInfoHttpReq &);
        const UdpControlServerInfoHttpReq & operator = (const UdpControlServerInfoHttpReq &);
    };


    class StartUpMrAutoDomainHttpRequest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        StartUpMrAutoDomainHttpRequest();
        ~StartUpMrAutoDomainHttpRequest();
        void StartUpMrAutoDomain(std::function<void(bool, QMap<QString, int>)> callback);
        void GetReqProNumber(QString req_pro);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    signals:
        void SendProNumber(QString req_pro);
        void SendRequestFinish(QString f_info);
        void RequestSuccess(QString succ_info);
        void RequestFail(QString err_info);

    private:
         std::function<void(bool, QMap<QString, int>)> checkCallback;
         QString m_get_req_pro;
    private:
        StartUpMrAutoDomainHttpRequest(const StartUpMrAutoDomainHttpRequest &);
        const StartUpMrAutoDomainHttpRequest & operator = (const StartUpMrAutoDomainHttpRequest &);
    };

    class OpenCloseMrAutoFactoryResetHttpRequest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        OpenCloseMrAutoFactoryResetHttpRequest();
        ~OpenCloseMrAutoFactoryResetHttpRequest();
        void OpenCloseMrAutoFactoryReset(std::function<void(bool, QMap<QString, int>)> callback);
        void GetReqProNumber(QString req_pro);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    private:
         std::function<void(bool, QMap<QString, int>)> checkCallback;
         QString m_get_req_pro;

    private:
        OpenCloseMrAutoFactoryResetHttpRequest(const OpenCloseMrAutoFactoryResetHttpRequest &);
        const OpenCloseMrAutoFactoryResetHttpRequest & operator = (const OpenCloseMrAutoFactoryResetHttpRequest &);
    };

    class SyncSpecMrDomainHttpRequest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        SyncSpecMrDomainHttpRequest();
        ~SyncSpecMrDomainHttpRequest();
        void SyncSpecMrDomain(std::function<void(bool, QMap<QString, int>)> callback);
        void GetReqestDataFromUI(QString req_pro, QString req_id, QString req_host);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    private:
         std::function<void(bool, QMap<QString, int>)> checkCallback;
         QString m_get_req_pro;
         QString m_get_req_id;
         QString m_get_req_host;

    private:
        SyncSpecMrDomainHttpRequest(const SyncSpecMrDomainHttpRequest &);
        const SyncSpecMrDomainHttpRequest & operator = (const SyncSpecMrDomainHttpRequest &);
    };

    class FactoryResetHttpReqest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        FactoryResetHttpReqest();
        ~FactoryResetHttpReqest();
        void FactoryReset(std::function<void(bool, QMap<QString, int>)> callback);
        void GetReqestDataFromUI(QString req_pro, QString req_id);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    private:
         std::function<void(bool, QMap<QString, int>)> checkCallback;
         QString m_get_req_pro;
         QString m_get_req_id;

    private:
        FactoryResetHttpReqest(const FactoryResetHttpReqest &);
        const FactoryResetHttpReqest & operator = (const FactoryResetHttpReqest &);
    };

    class  RestartSpecMrHttpReqest : public BaseHttpClient
    {
        Q_OBJECT
    public:
         RestartSpecMrHttpReqest();
         ~RestartSpecMrHttpReqest();
         void RestartSpecMr(std::function<void(bool, QMap<QString, int>)> callback);
         void GetReqestDataFromUI(QString req_pro, QString req_id);

     protected:
         void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

     private:
          std::function<void(bool, QMap<QString, int>)> checkCallback;
          QString m_get_req_pro;
          QString m_get_req_id;

    private:
         RestartSpecMrHttpReqest(const RestartSpecMrHttpReqest &);
         const RestartSpecMrHttpReqest & operator = (const RestartSpecMrHttpReqest &);
    };

    class ReadSpecMrConfigInfoHttpReqest : public BaseHttpClient
    {
        Q_OBJECT
    public:
        ReadSpecMrConfigInfoHttpReqest();
        ~ReadSpecMrConfigInfoHttpReqest(); 
        void ReadSpecMrConfigInfo(std::function<void(bool, QMap<QString, QVariant>)> callback);
        void GetReqestDataFromUI(QString req_pro);

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);

    private:
         std::function<void(bool, QMap<QString, QVariant>)> checkCallback;
         QString m_get_req_pro;

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

    protected:
        void requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode);
    public:
        QString m_get_tag_id;
    private:
         std::function<void(bool, QMap<QString, QVariant>)> checkCallback;

    private:
        GetMrReslutHttpReqest(const GetMrReslutHttpReqest &);
        const GetMrReslutHttpReqest & operator = (const GetMrReslutHttpReqest &);
    };
}
#endif // BASEHTTPCLIENT_H

