#ifndef FACTORYRESET_H
#define FACTORYRESET_H

#include <QDialog>
#include <QString>
#include "basehttpclient.h"

namespace Ui {
class FactoryReset;
}

class FactoryReset : public QDialog
{
    Q_OBJECT

public:
    explicit FactoryReset(QWidget *parent = 0);
    ~FactoryReset();
    void FactoryResetInit();

private slots:
    void on_pushButton_clicked();

public:
    QString m_req_pro;
    QString m_req_id;

private:
    Ui::FactoryReset *ui;
    HttpClient::FactoryResetHttpReqest *m_factory_reset;

private:
    FactoryReset(const FactoryReset &);
    const FactoryReset & operator = (const FactoryReset &);
};

#endif // FACTORYRESET_H
