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

#include <QMap>

#include <iostream>

using namespace std;

namespace HttpClient
{
    const QString OpenDomainUrl = QString("http://192.168.32.104:8088/api/calibrator/openclosedomain");
    const QString UdpControlUrl = QString("http://192.168.32.104:8088/api/calibrator/getudpsvrdata");

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
}
#endif // BASEHTTPCLIENT_H

