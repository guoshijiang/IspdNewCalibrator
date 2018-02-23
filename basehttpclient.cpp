#include "basehttpclient.h"

namespace HttpClient
{
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

    //获取服务端控制信息
    UdpControlServerInfoHttpReq::UdpControlServerInfoHttpReq()
    {

    }

    UdpControlServerInfoHttpReq::UdpControlServerInfoHttpReq(QString req_url)
    {
        this->M_UDP_COMTROL_RUL = req_url;
    }

    UdpControlServerInfoHttpReq::~UdpControlServerInfoHttpReq()
    {

    }

    void UdpControlServerInfoHttpReq::UdpControlServerInfo(std::function<void(bool, QMap<QString, QVariant>)> callback)
    {
        qDebug() << "M_UDP_COMTROL_RUL = " << M_UDP_COMTROL_RUL;
        QString url = M_UDP_COMTROL_RUL;
        this->checkCallback = callback;
        QJsonObject obj;
        obj.insert("pro", SEVER_STATUS_INFO);
        this->post(url, QJsonDocument(obj).toJson());
    }


    void UdpControlServerInfoHttpReq::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        QString pro, host, mr;
        int ec, sync, reset;
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

                    if(json_obj.contains("ipv4"))
                    {
                        QJsonValue value = json_obj.value("ipv4");
                        udp_svr_data.insert("ipv4", value);
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

    StartUpMrAutoDomainHttpRequest::StartUpMrAutoDomainHttpRequest()
    {

    }

    StartUpMrAutoDomainHttpRequest::StartUpMrAutoDomainHttpRequest(int req_sync, QString req_url)
    {
        this->m_get_req_sync = req_sync;
        this->M_START_UP_RUL = req_url;
    }

    StartUpMrAutoDomainHttpRequest::~StartUpMrAutoDomainHttpRequest()
    {

    }

    void StartUpMrAutoDomainHttpRequest::StartUpMrAutoDomain(std::function<void(bool, QMap<QString, int>)> callback)
    {
        QString url = M_START_UP_RUL;
        this->checkCallback = callback;
        QJsonObject obj;
        obj.insert("pro", SET_SERVER_AUTO_SYSNC_FUNC);
        qDebug() << "request::" << this->m_get_req_sync;
        obj.insert("sync", this->m_get_req_sync);
        this->post(url, QJsonDocument(obj).toJson());
    }

    void StartUpMrAutoDomainHttpRequest::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        int ec;
        if(statusCode == 200)
        {
            QJsonParseError jsonError;
            QJsonDocument json_doc = QJsonDocument::fromJson(data, &jsonError);
            if(jsonError.error == QJsonParseError::NoError)
            {
                QMap<QString, int> open_domain;
                if(json_doc.isObject())
                {
                    QJsonObject json_obj = json_doc.object();                  
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
        QMap<QString, int>open_domain;
        this->checkCallback(false, open_domain);
    }

    //设置控制服务终端自动恢复出厂设置状态
    OpenCloseMrAutoFactoryResetHttpRequest::OpenCloseMrAutoFactoryResetHttpRequest()
    {

    }

    OpenCloseMrAutoFactoryResetHttpRequest::OpenCloseMrAutoFactoryResetHttpRequest(int req_reset, QString req_url)
    {
        this->m_get_req_reset = req_reset;
        this->M_OPEN_CLOSE_RUL = req_url;
    }

    OpenCloseMrAutoFactoryResetHttpRequest::~OpenCloseMrAutoFactoryResetHttpRequest()
    {

    }

    void OpenCloseMrAutoFactoryResetHttpRequest::OpenCloseMrAutoFactoryReset(std::function<void(bool, QMap<QString, int>)> callback)
    {
        QString url = M_OPEN_CLOSE_RUL;
        this->checkCallback = callback;
        QJsonObject obj;
        obj.insert("pro", SET_SERVER_MR_AUTO_FACTORYSET);
        obj.insert("reset", this->m_get_req_reset);
        this->post(url, QJsonDocument(obj).toJson());
    }

    void OpenCloseMrAutoFactoryResetHttpRequest::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        int ec;
        if(statusCode == 200)
        {
            QJsonParseError jsonError;
            QJsonDocument json_doc = QJsonDocument::fromJson(data, &jsonError);
            if(jsonError.error == QJsonParseError::NoError)
            {
                QMap<QString, int> ocmfr_freset;
                if(json_doc.isObject())
                {
                    QJsonObject json_obj = json_doc.object();
                    if(json_obj.contains("ec"))
                    {
                        QJsonValue value = json_obj.value("ec");
                        if (value.isDouble())
                        {
                            ec = value.toDouble();
                            ocmfr_freset.insert("ec", ec);
                        }
                    }
                }
                this->checkCallback(true, ocmfr_freset);
                return;
            }
        }
        QMap<QString, int>ocmfr_freset;
        this->checkCallback(false, ocmfr_freset);
    }

    //更新指定终端目的服务域名
    SyncSpecMrDomainHttpRequest::SyncSpecMrDomainHttpRequest()
    {

    }

    SyncSpecMrDomainHttpRequest::SyncSpecMrDomainHttpRequest(QString req_id, QString req_host, QString req_url)
    {
        this->m_get_req_id  = req_id;
        this->m_get_req_host = req_host;
        this->M_SYNC_RUL = req_url;
    }

    SyncSpecMrDomainHttpRequest::~SyncSpecMrDomainHttpRequest()
    {

    }

    void SyncSpecMrDomainHttpRequest::SyncSpecMrDomain(std::function<void(bool, QMap<QString, int>)> callback)
    {
        QString url = M_SYNC_RUL;
        this->checkCallback = callback;
        QJsonObject obj;
        obj.insert("pro", UPDATE_SPEC_MR_SERVER_DOMAIN);
        obj.insert("mid", this->m_get_req_id.toInt());
        obj.insert("domain", this->m_get_req_host);
        this->post(url, QJsonDocument(obj).toJson());
    }

    void SyncSpecMrDomainHttpRequest::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        int pro, ec;
        if(statusCode == 200)
        {
            QJsonParseError jsonError;
            QJsonDocument json_doc = QJsonDocument::fromJson(data, &jsonError);
            if(jsonError.error == QJsonParseError::NoError)
            {
                QMap<QString, int> sync_domain;
                if(json_doc.isObject())
                {
                    QJsonObject json_obj = json_doc.object();
                    if(json_obj.contains("pro"))
                    {
                        QJsonValue value = json_obj.value("pro");
                        if (value.isDouble())
                        {
                            pro = value.toDouble();
                            sync_domain.insert("pro", pro);
                        }
                    }
                    if(json_obj.contains("ec"))
                    {
                        QJsonValue value = json_obj.value("ec");
                        if (value.isDouble())
                        {
                            ec = value.toDouble();
                            sync_domain.insert("ec", ec);
                        }
                    }
                }
                this->checkCallback(true, sync_domain);
                return;
            }
        }
        QMap<QString, int>sync_domain;
        this->checkCallback(false, sync_domain);
    }

    //恢复指定终端出厂设置功能
    FactoryResetHttpReqest::FactoryResetHttpReqest()
    {

    }

    FactoryResetHttpReqest::FactoryResetHttpReqest(QString req_id, QString req_url)
    {
        this->m_get_req_id = req_id;
        this->M_FAXTORY_RESET_URL = req_url;
    }

    FactoryResetHttpReqest::~FactoryResetHttpReqest()
    {

    }

    void FactoryResetHttpReqest::FactoryReset(std::function<void(bool, QMap<QString, int>)> callback)
    {
        QString url = M_FAXTORY_RESET_URL;
        this->checkCallback = callback;
        QJsonObject obj;
        obj.insert("pro", SET_SPEC_MR_FACTORYSET);
        obj.insert("mid", this->m_get_req_id.toInt());
        this->post(url, QJsonDocument(obj).toJson());
    }

    void FactoryResetHttpReqest::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        int ec;
        if(statusCode == 200)
        {
            QJsonParseError jsonError;
            QJsonDocument json_doc = QJsonDocument::fromJson(data, &jsonError);
            if(jsonError.error == QJsonParseError::NoError)
            {
                QMap<QString, int> factory_reset;
                if(json_doc.isObject())
                {
                    QJsonObject json_obj = json_doc.object();
                    if(json_obj.contains("ec"))
                    {
                        QJsonValue value = json_obj.value("ec");
                        if(value.isDouble())
                        {
                            ec = value.toDouble();
                            factory_reset.insert("ec", ec);
                        }
                    }
                }
                this->checkCallback(true, factory_reset);
                return;
            }
        }
        QMap<QString, int>factory_reset;
        this->checkCallback(false, factory_reset);
    }

    //重启指定终端
    RestartSpecMrHttpReqest::RestartSpecMrHttpReqest()
    {

    }

    RestartSpecMrHttpReqest::RestartSpecMrHttpReqest(QString req_id, QString req_url)
    {
        this->m_get_req_id  = req_id;
        this->M_RESTART_RUL = req_url;
    }

    RestartSpecMrHttpReqest::~RestartSpecMrHttpReqest()
    {

    }

    void RestartSpecMrHttpReqest::RestartSpecMr(std::function<void(bool, QMap<QString, int>)> callback)
    {
        QString url = M_RESTART_RUL;
        this->checkCallback = callback;
        QJsonObject obj;
        obj.insert("pro", RESTART_SPEC_MR);
        obj.insert("mid",this->m_get_req_id.toInt());
        this->post(url, QJsonDocument(obj).toJson());
    }

    void RestartSpecMrHttpReqest::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        int pro, ec;
        if(statusCode == 200)
        {
            QJsonParseError jsonError;
            QJsonDocument json_doc = QJsonDocument::fromJson(data, &jsonError);
            if(jsonError.error == QJsonParseError::NoError)
            {
                QMap<QString, int> restart_specmr;
                if(json_doc.isObject())
                {
                    QJsonObject json_obj = json_doc.object();
                    if(json_obj.contains("ec"))
                    {
                        QJsonValue value = json_obj.value("ec");
                        if (value.isDouble())
                        {
                            ec = value.toDouble();
                            restart_specmr.insert("ec", ec);
                        }
                    }
                }
                this->checkCallback(true, restart_specmr);
                return;
            }
        }
        QMap<QString, int>restart_specmr;
        this->checkCallback(false, restart_specmr);
    }

    //读指定终端配置信息
    ReadSpecMrConfigInfoHttpReqest::ReadSpecMrConfigInfoHttpReqest()
    {

    }

    ReadSpecMrConfigInfoHttpReqest::ReadSpecMrConfigInfoHttpReqest(QString req_id, QString read_url)
    {
        this->m_get_mid = req_id;
        this->M_READ_RUL = read_url;
    }

    ReadSpecMrConfigInfoHttpReqest::~ReadSpecMrConfigInfoHttpReqest()
    {

    }

    void ReadSpecMrConfigInfoHttpReqest::ReadSpecMrConfigInfo(std::function<void(bool, QMap<QString, QVariant>)> callback)
    {
        QString url = this->M_READ_RUL;
        this->checkCallback = callback;
        QJsonObject obj;
        obj.insert("pro", READ_SPEC_MR_INFO);
        obj.insert("mid", this->m_get_mid.toInt());
        this->post(url, QJsonDocument(obj).toJson());
    }

    void ReadSpecMrConfigInfoHttpReqest::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        int ver, mid, ec, tp, up, nm, ari, dv;
        QString lip, nip, gip, dip, lip2, nip2, gip2, dip2, domain, mac, ud;
        if(statusCode == 200)
        {
            QJsonParseError jsonError;
            QJsonDocument json_doc = QJsonDocument::fromJson(data, &jsonError);
            qDebug() << "data = " << data;
            if(jsonError.error == QJsonParseError::NoError)
            {
                QMap<QString, QVariant> read_spec_mr_info;
                if(json_doc.isObject())
                {
                    QJsonObject json_obj = json_doc.object();
                    if(json_obj.contains("ec"))
                    {
                        QJsonValue value = json_obj.value("ec");
                        if (value.isDouble())
                        {
                            ec = value.toDouble();
                            read_spec_mr_info.insert("ec", ec);
                        }
                    }

                    if(json_obj.contains("mid"))
                    {
                        QJsonValue value = json_obj.value("mid");
                        if (value.isDouble())
                        {
                            mid = value.toDouble();
                            read_spec_mr_info.insert("mid", mid);
                        }
                    }

                    if(json_obj.contains("ver"))
                    {
                        QJsonValue value = json_obj.value("ver");
                        if (value.isDouble())
                        {
                            ver = value.toDouble();
                            read_spec_mr_info.insert("ver", ver);
                        }
                    }

                    if(json_obj.contains("lip"))
                    {
                        QJsonValue value = json_obj.value("lip");
                        if (value.isString())
                        {
                            lip = value.toString();
                            read_spec_mr_info.insert("lip", lip);
                        }
                    }

                    if(json_obj.contains("nip"))
                    {
                        QJsonValue value = json_obj.value("nip");
                        if (value.isString())
                        {
                            nip = value.toString();
                            read_spec_mr_info.insert("nip", nip);
                        }
                    }

                    if(json_obj.contains("gip"))
                    {
                        QJsonValue value = json_obj.value("gip");
                        if (value.isString())
                        {
                            gip = value.toString();
                            read_spec_mr_info.insert("gip", gip);
                        }
                    }

                    if(json_obj.contains("dip"))
                    {
                        QJsonValue value = json_obj.value("dip");
                        if (value.isString())
                        {
                            dip = value.toString();
                            read_spec_mr_info.insert("dip", dip);
                        }
                    }

                    if(json_obj.contains("lip2"))
                    {
                        QJsonValue value = json_obj.value("lip2");
                        if (value.isString())
                        {
                            lip2 = value.toString();
                            read_spec_mr_info.insert("lip2", lip2);
                        }
                    }

                    if(json_obj.contains("nip2"))
                    {
                        QJsonValue value = json_obj.value("nip2");
                        if (value.isString())
                        {
                            nip2 = value.toString();
                            read_spec_mr_info.insert("nip2", nip2);
                        }
                    }

                    if(json_obj.contains("gip2"))
                    {
                        QJsonValue value = json_obj.value("gip2");
                        if (value.isString())
                        {
                            gip2 = value.toString();
                            read_spec_mr_info.insert("gip2", gip2);
                        }
                    }

                    if(json_obj.contains("dip2"))
                    {
                        QJsonValue value = json_obj.value("dip2");
                        if (value.isString())
                        {
                            dip2 = value.toString();
                            read_spec_mr_info.insert("dip2", dip2);
                        }
                    }

                    if(json_obj.contains("domain"))
                    {
                        QJsonValue value = json_obj.value("domain");
                        if (value.isString())
                        {
                            domain = value.toString();
                            read_spec_mr_info.insert("domain", domain);
                        }
                    }

                    if(json_obj.contains("mac"))
                    {
                        QJsonValue value = json_obj.value("mac");
                        if (value.isString())
                        {
                            mac = value.toString();
                            read_spec_mr_info.insert("mac", mac);
                        }
                    }

                    if(json_obj.contains("tp"))
                    {
                        QJsonValue value = json_obj.value("tp");
                        if (value.isDouble())
                        {
                            tp = value.toDouble();
                            read_spec_mr_info.insert("tp", tp);
                        }
                    }

                    if(json_obj.contains("up"))
                    {
                        QJsonValue value = json_obj.value("up");
                        if (value.isDouble())
                        {
                            up = value.toDouble();
                            read_spec_mr_info.insert("up", up);
                        }
                    }

                    if(json_obj.contains("nm"))
                    {
                        QJsonValue value = json_obj.value("nm");
                        if (value.isDouble())
                        {
                            nm = value.toDouble();
                            read_spec_mr_info.insert("nm", nm);
                        }
                    }



                    if(json_obj.contains("ari"))
                    {
                        QJsonValue value = json_obj.value("ari");
                        if (value.isDouble())
                        {
                            ari = value.toDouble();
                            read_spec_mr_info.insert("ari", ari);
                        }
                    }

                    if(json_obj.contains("ud"))
                    {
                        QJsonValue value = json_obj.value("ud");
                        if(value.isString())
                        {
                            ud = value.toString();
                            read_spec_mr_info.insert("ud", ud);
                        }
                    }

                    if(json_obj.contains("dv"))
                    {
                        QJsonValue value = json_obj.value("dv");
                        if (value.isDouble())
                        {
                            dv = value.toDouble();
                            read_spec_mr_info.insert("dv", dv);
                        }
                    }

                }
                this->checkCallback(true, read_spec_mr_info);
                return;
            }
        }
        QMap<QString, QVariant>read_spec_mr_info;
        this->checkCallback(false, read_spec_mr_info);
    }

    WriteSpecMrConfigInfoHttpRequest::WriteSpecMrConfigInfoHttpRequest()
    {

    }

    WriteSpecMrConfigInfoHttpRequest::~WriteSpecMrConfigInfoHttpRequest()
    {

    }

    void WriteSpecMrConfigInfoHttpRequest::WriteSpecMrConfigInfo(std::function<void(bool, QMap<QString, int>)> callback)
    {
        QString url = M_WRITE_RUL;
        this->checkCallback = callback;
        QJsonObject obj;
        obj.insert("pro", WRITE_SPEC_MR_CONFIG);
        obj.insert("mid", m_wsmr.mmid);
        obj.insert("lip", m_wsmr.lip);
        obj.insert("nip", m_wsmr.nip);
        obj.insert("gip", m_wsmr.gip);
        obj.insert("dip", m_wsmr.dip);
        obj.insert("domain", m_wsmr.domain);
        obj.insert("tp", m_wsmr.tp);
        obj.insert("up", m_wsmr.up);
        obj.insert("ver", m_wsmr.version);
        obj.insert("nm", m_wsmr.nm);
        obj.insert("ari", m_wsmr.ari);
        this->post(url, QJsonDocument(obj).toJson());
    }

    void WriteSpecMrConfigInfoHttpRequest::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        int pro, ec;
        if(statusCode == 200)
        {
            QJsonParseError jsonError;
            QJsonDocument json_doc = QJsonDocument::fromJson(data, &jsonError);
            if(jsonError.error == QJsonParseError::NoError)
            {
                QMap<QString, int> write_specmr;
                if(json_doc.isObject())
                {
                    QJsonObject json_obj = json_doc.object();
                    if(json_obj.contains("ec"))
                    {
                        QJsonValue value = json_obj.value("ec");
                        if (value.isDouble())
                        {
                            ec = value.toDouble();
                            write_specmr.insert("ec", ec);
                        }
                    }
                }
                this->checkCallback(true, write_specmr);
                return;
            }
        }
        QMap<QString, int>write_specmr;
        this->checkCallback(false, write_specmr);
    }

    GetMrReslutHttpReqest::GetMrReslutHttpReqest()
    {

    }
    GetMrReslutHttpReqest::~GetMrReslutHttpReqest()
    {

    }

    void GetMrReslutHttpReqest::httpRequestUrl(QString url)
    {
        this->M_RUL = url;
    }

    void GetMrReslutHttpReqest::getIDFromUI(int id)
    {
        this->m_get_tag_id = id;
    }

    void GetMrReslutHttpReqest::GetMrResult(std::function<void(bool, QMap<QString, QVariant>)> callback)
    {
        QString url = this->M_RUL;
        this->checkCallback = callback;
        QJsonObject obj;
        obj.insert("pro", JZ_DATA_CODE);
        obj.insert("id", this->m_get_tag_id);
        this->post(url, QJsonDocument(obj).toJson());
    }

    void GetMrReslutHttpReqest::requestFinished(QNetworkReply* reply, const QByteArray data, const int statusCode)
    {
        int ec, tag_id, hr, power, charge;
        QJsonArray ac, gy;
        if(reply->error() != QNetworkReply::NoError)
        {
            qDebug() << "Error:" << reply->errorString();
            return;
        }

        if(statusCode == 200)
        {
            qDebug() << "data=" << data;
            QJsonParseError jsonError;
            QJsonDocument json_doc = QJsonDocument::fromJson(data, &jsonError);
            if(jsonError.error == QJsonParseError::NoError)
            {
                QMap<QString, QVariant> mr_result;
                if(json_doc.isObject())
                {
                    QJsonObject json_obj = json_doc.object();
                    if(json_obj.contains("ec"))
                    {
                        QJsonValue value = json_obj.value("ec");
                        if (value.isDouble())
                        {
                            ec = value.toDouble();
                            mr_result.insert("ec", ec);
                        }
                    }
                    if(json_obj.contains("id"))
                    {
                        QJsonValue value = json_obj.value("id");
                        if (value.isDouble())
                        {
                            tag_id = value.toDouble();
                            mr_result.insert("id", tag_id);
                        }
                    }

                    if(json_obj.contains("hr"))
                    {
                        QJsonValue value = json_obj.value("hr");
                        if (value.isDouble())
                        {
                            hr = value.toDouble();
                            mr_result.insert("hr", hr);
                        }
                    }

                    if(json_obj.contains("power"))
                    {
                        QJsonValue value = json_obj.value("power");
                        if (value.isDouble())
                        {
                            power = value.toDouble();
                            mr_result.insert("power", power);
                        }
                    }

                    if(json_obj.contains("charge"))
                    {
                        QJsonValue value = json_obj.value("charge");
                        if (value.isDouble())
                        {
                            charge = value.toDouble();
                            mr_result.insert("charge", charge);
                        }
                    }

                    if(json_obj.contains("ac"))
                    {
                        QJsonValue value = json_obj.value("ac");
                        if (value.isArray())
                        {
                            ac = value.toArray();
                            mr_result.insert("ac", ac);
                        }
                    }

                    if(json_obj.contains("gy"))
                    {
                        QJsonValue value = json_obj.value("gy");
                        if (value.isArray())
                        {
                            gy = value.toArray();
                            mr_result.insert("gy", gy);
                        }
                    }
                }
                this->checkCallback(true, mr_result);
                return;
            }
        }
        QMap<QString, QVariant>mr_result;
        this->checkCallback(false, mr_result);
    }
}
