#ifndef STARTUPMRAUTOFACTORYRESET_H
#define STARTUPMRAUTOFACTORYRESET_H

#include <QDialog>
#include <QString>
#include "basehttpclient.h"
#include "common.h"

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
    Common::HandleError* m_handle_err;

public:
    int m_reset;
    bool isOpen;
    QString m_open_close_url;
private:
    StartUpMrAutoFactoryReset(const StartUpMrAutoFactoryReset &);
    const StartUpMrAutoFactoryReset & operator = (const StartUpMrAutoFactoryReset &);
};

#endif // STARTUPMRAUTOFACTORYRESET_H
