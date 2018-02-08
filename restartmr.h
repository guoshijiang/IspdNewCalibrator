#ifndef RESTARTMR_H
#define RESTARTMR_H

#include <QDialog>
#include <QString>
#include "basehttpclient.h"

namespace Ui {
class RestartMr;
}

class RestartMr : public QDialog
{
    Q_OBJECT

public:
    explicit RestartMr(QWidget *parent = 0);
    ~RestartMr();
    void RestartMrInit();
    void CloseApp();

private slots:
    void on_pushButton_clicked();

public:
    QString m_req_id;
    QList<int> m_mid;
    QString m_restart_url;

private:
    Ui::RestartMr *ui;
    HttpClient::RestartSpecMrHttpReqest* m_restart_specmr;

private:
    RestartMr(const RestartMr &);
    const RestartMr & operator = (const RestartMr &);
};

#endif // RESTARTMR_H
