#ifndef SYNCSPECMRDOMAIN_H
#define SYNCSPECMRDOMAIN_H

#include <QDialog>
#include <QString>
#include "basehttpclient.h"

namespace Ui {
class SyncSpecMrDomain;
}

class SyncSpecMrDomain : public QDialog
{
    Q_OBJECT

public:
    explicit SyncSpecMrDomain(QWidget *parent = 0);
    ~SyncSpecMrDomain();
    void SyncSpecMrDomainInit();

private slots:
    void on_pushButton_clicked();

public:
    QString m_req_pro;
    QString m_req_id;
    QString m_req_host;

private:
    Ui::SyncSpecMrDomain *ui;
    HttpClient::SyncSpecMrDomainHttpRequest* m_sync_specmr_domain;
private:
    SyncSpecMrDomain(const SyncSpecMrDomain &);
    const SyncSpecMrDomain & operator = (const SyncSpecMrDomain &);
};

#endif // SYNCSPECMRDOMAIN_H
