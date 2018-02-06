#ifndef WRITESPECCONFIG_H
#define WRITESPECCONFIG_H

#include <QDialog>
#include "common.h"
#include <QString>
#include "basehttpclient.h"

namespace Ui {
class WriteSpecConfig;
}

class WriteSpecConfig : public QDialog
{
    Q_OBJECT

public:
    explicit WriteSpecConfig(QWidget *parent = 0);
    ~WriteSpecConfig();
    void WriteSpecConfig_start();
    void CloseApp();

private slots:
    void on_pushButton_clicked();

private:
    Ui::WriteSpecConfig *ui;
    HttpClient::WriteSpecMrConfigInfoHttpRequest* m_write_specmr_info;

private:
    WriteSpecConfig(const WriteSpecConfig &);
    const WriteSpecConfig & operator = (const WriteSpecConfig &);
};

#endif // WRITESPECCONFIG_H
