#ifndef READSPECIFICCONFIG_H
#define READSPECIFICCONFIG_H

#include <QDialog>
#include <QString>
#include <QVariant>
#include "basehttpclient.h"

namespace Ui {
class ReadSpecificConfig;
}

class ReadSpecificConfig : public QDialog
{
    Q_OBJECT

public:
    explicit ReadSpecificConfig(QWidget *parent = 0);
    ~ReadSpecificConfig();
    void ReadSpecConfig_start();
    void CloseApp();

private slots:
    void on_pushButton_clicked();

public:
    QString m_mid;
    QList<int> m_list_mid;

private:
    Ui::ReadSpecificConfig *ui;
    HttpClient::ReadSpecMrConfigInfoHttpReqest* m_read_spec_mrinfo;

private:
    ReadSpecificConfig(const ReadSpecificConfig &);
    const ReadSpecificConfig & operator = (const ReadSpecificConfig &);
};

#endif // READSPECIFICCONFIG_H
