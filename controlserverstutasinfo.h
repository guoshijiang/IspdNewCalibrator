#ifndef CONTROLSERVERSTUTASINFO_H
#define CONTROLSERVERSTUTASINFO_H

#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QObject>
#include <QLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QFont>
#include <QDebug>
#include <QMap>
#include <QAction>
#include <QByteArray>
#include <QToolBox>
#include <QTimer>
#include <QDialog>

#include "common.h"
#include "basehttpclient.h"
#include "startupmrautodomain.h"

namespace Ui {
class ControlServerStutasInfo;
}

class ControlServerStutasInfo : public QDialog
{
    Q_OBJECT

public:
    explicit ControlServerStutasInfo(QWidget *parent = 0);
    ~ControlServerStutasInfo();
    void CSSInfoStart();
    inline QFont font()
    {
        QFont m_font("宋体", 18, 75);
        return m_font;
    }
    void UdpSvrDataUi(QString key_name, QString value_name);
    void CloseApp();
    void getUdpControl();

private slots:

private:
    Ui::ControlServerStutasInfo* ui;
    HttpClient::UdpControlServerInfoHttpReq* m_udp_con_svr_info;
public:
    /***************************/
    /*界面相关的变量              */
    /***************************/
    QScrollArea *p_area;
    QWidget *p_widget;
    QVBoxLayout *m_vbox_layout;
    QVBoxLayout *m_main_layout;
    QWidget *m_page;
    QGridLayout *m_layout;
    QLineEdit *m_ledit;
    QLabel *m_label;

    /***************************/
    /*HTTP请求相关的变量          */
    /***************************/
    QString m_req_pro;
private:
    ControlServerStutasInfo(const ControlServerStutasInfo &);
    const ControlServerStutasInfo & operator = (const ControlServerStutasInfo &);
};

#endif // CONTROLSERVERSTUTASINFO_H
