#ifndef STARTUPMRAUTODOMAIN_H
#define STARTUPMRAUTODOMAIN_H

#include <QString>
#include <QDialog>
#include "basehttpclient.h"
#include "common.h"
#include <QDebug>

namespace Ui {
class StartUpMrAutoDomain;
}

class StartUpMrAutoDomain : public QDialog
{
    Q_OBJECT

public:
    explicit StartUpMrAutoDomain(QWidget *parent = 0);
    ~StartUpMrAutoDomain();
    void StartUpMrAutoDomain_start();
    void CloseApp();

private slots:
    void on_pushButton_clicked();

public slots:
     void onProNum(QString pro_num);
     void onRequestFinish(QString finish_info);
     void onRequestSuccess(QString succ_info);
     void onRequestFail(QString err_info);

public:
    QString m_req_pro;
    int m_sync;
    bool isOpen;
    QString m_sync_rul;

private:
    Ui::StartUpMrAutoDomain *ui;
    HttpClient::StartUpMrAutoDomainHttpRequest *m_sumad_req;
    Common::HandleError* m_handle_err;

private:
    StartUpMrAutoDomain(const StartUpMrAutoDomain &);
    const StartUpMrAutoDomain & operator = (const StartUpMrAutoDomain &);
};

#endif // STARTUPMRAUTODOMAIN_H
