#include "basehttpclient.h"

namespace HttpClient
{
    //class BaseHttpClien
    BaseHttpClient::BaseHttpClient()
    {
        httpRequest.setRawHeader("Accept-Encoding", "gzip, deflate");
        httpRequest.setRawHeader("Content-Type", "application/json");
        QObject::connect(&networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(serviceRequestFinished(QNetworkReply*)));
    }

    BaseHttpClient::~BaseHttpClient()
    {
        networkAccessManager.disconnect();
    }

    void BaseHttpClient::get(const QString url)
    {
        httpRequest.setUrl(QUrl(url));
        networkAccessManager.get(httpRequest);
    }

    void BaseHttpClient::post(const QString url, const QByteArray &data)
    {
        httpRequest.setUrl(QUrl(url));
        networkAccessManager.post(httpRequest, data);
    }

    void BaseHttpClient::serviceRequestFinished(QNetworkReply *reply)
    {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(reply->error() == QNetworkReply::NoError)
        {
            requestFinished(reply, reply->readAll(), statusCode);
        }
        else
        {
            requestFinished(reply, "", statusCode);
        }
        reply->deleteLater();
    }
    //UdpControlServerInfo

    UdpControlServerInfoHttpReq::UdpControlServerInfoHttpReq()
    {

    }

    UdpControlServerInfoHttpReq::~UdpControlServerInfoHttpReq()
    {

    }

    void UdpControlServerInfoHttpReq::UdpControlServerInfo(std::function<void(bool, QMap<QString, QVariant>)> callback)
    {
        QString pro = this->m_get_req_pro;
        QString url = UdpControlUrl +"?pro=" + pro;
        this->checkCallback = callback;
        this->get(url);
    }

    void UdpControlServerInfoHttpReq::GetReqProNumber(QString req_pro)
    {
         this->m_get_req_pro = req_pro;
    }

    void UdpControlServerInfoHttpReq::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        QString pro;
        int ec;
        QString host;
        int sync;
        int reset;
        QString mr;
        if(statusCode == 200)
        {
            QJsonParseError jsonError;
            QJsonDocument json_doc = QJsonDocument::fromJson(data, &jsonError);
            if(jsonError.error == QJsonParseError::NoError)
            {
                QMap<QString, QVariant> udp_svr_data;
                if(json_doc.isObject())
                {
                    QJsonObject json_obj = json_doc.object();
                    if(json_obj.contains("pro"))
                    {
                        QJsonValue value = json_obj.value("pro");
                        if(value.isString())
                        {
                            pro = value.toString();
                            udp_svr_data.insert("pro", pro);
                        }
                    }

                    if(json_obj.contains("ec"))
                    {
                        QJsonValue value = json_obj.value("ec");
                        if(value.isDouble())
                        {
                            ec = value.toDouble();
                            QString ec_str = QString::number(ec);
                            udp_svr_data.insert("ec", ec_str);
                        }
                    }

                    if(json_obj.contains("host"))
                    {
                         QJsonValue value = json_obj.value("host");
                         if(value.isString())
                         {
                             host = value.toString();
                             udp_svr_data.insert("host", host);
                         }
                    }

                    if(json_obj.contains("sync"))
                    {
                         QJsonValue value = json_obj.value("sync");
                         if(value.isDouble())
                         {
                             sync = value.toDouble();
                             QString sync_str = QString::number(sync);
                             udp_svr_data.insert("sync", sync_str);
                         }
                    }

                    if(json_obj.contains("reset"))
                    {
                         QJsonValue value = json_obj.value("reset");
                         if(value.isDouble())
                         {
                             reset = value.toDouble();
                             QString reset_str = QString::number(reset);
                             udp_svr_data.insert("reset", reset_str);
                         }
                    }

                    if(json_obj.contains("mr"))
                    {
                         QJsonValue value = json_obj.value("mr");
                         udp_svr_data.insert("mr", value);
                    }
                }
                this->checkCallback(true, udp_svr_data);
                return;
            }
        }
        QMap<QString, QVariant> udp_svr_data;
        this->checkCallback(false, udp_svr_data);
    }


    //class StartUpMrAutoDomainHttpRequest
    StartUpMrAutoDomainHttpRequest::StartUpMrAutoDomainHttpRequest()
    {

    }

    StartUpMrAutoDomainHttpRequest::~StartUpMrAutoDomainHttpRequest()
    {

    }
    void StartUpMrAutoDomainHttpRequest::GetReqProNumber(QString req_pro)
    {
        this->m_get_req_pro = req_pro;
    }

    void StartUpMrAutoDomainHttpRequest::StartUpMrAutoDomain(std::function<void(bool, QMap<QString, int>)> callback)
    {
        emit SendProNumber(m_get_req_pro);
        QString pro = this->m_get_req_pro;
        QString url = OpenDomainUrl +"?pro=" + pro + "&status=0";
        this->checkCallback = callback;
        this->get(url);
        emit SendRequestFinish(url);
    }

    void StartUpMrAutoDomainHttpRequest::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        int pro, ec;
        if(statusCode == 200)
        {
            emit RequestSuccess(QString::fromLocal8Bit("请求成功"));
            QJsonParseError jsonError;
            QJsonDocument json_doc = QJsonDocument::fromJson(data, &jsonError);
            if(jsonError.error == QJsonParseError::NoError)
            {
                QMap<QString, int> open_domain;
                if(json_doc.isObject())
                {
                    QJsonObject json_obj = json_doc.object();
                    if(json_obj.contains("pro"))
                    {
                        QJsonValue value = json_obj.value("pro");
                        if (value.isDouble())
                        {
                            pro = value.toDouble();
                            open_domain.insert("pro", pro);
                        }
                    }
                    if(json_obj.contains("ec"))
                    {
                        QJsonValue value = json_obj.value("ec");
                        if (value.isDouble())
                        {
                            ec = value.toDouble();
                            open_domain.insert("ec", ec);
                        }
                    }
                }
                this->checkCallback(true, open_domain);
                return;
            }
        }
        emit RequestFail(QString::fromLocal8Bit("请求失败"));
        QMap<QString, int>open_domain;
        this->checkCallback(false, open_domain);
    }
}
