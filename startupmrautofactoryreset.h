#ifndef STARTUPMRAUTOFACTORYRESET_H
#define STARTUPMRAUTOFACTORYRESET_H

#include <QDialog>
#include <QString>
#include "basehttpclient.h"

namespace Ui {
class StartUpMrAutoFactoryReset;
}

class StartUpMrAutoFactoryReset : public QDialog
{
    Q_OBJECT

public:
    explicit StartUpMrAutoFactoryReset(QWidget *parent = 0);
    ~StartUpMrAutoFactoryReset();
    void StartUpMrAutoFactoryReset_start();
    void CloseApp();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StartUpMrAutoFactoryReset *ui;
    HttpClient::OpenCloseMrAutoFactoryResetHttpRequest* m_oc_mr_auto_freset;
public:
    QString m_req_pro;
private:
    StartUpMrAutoFactoryReset(const StartUpMrAutoFactoryReset &);
    const StartUpMrAutoFactoryReset & operator = (const StartUpMrAutoFactoryReset &);
};

#endif // STARTUPMRAUTOFACTORYRESET_H
