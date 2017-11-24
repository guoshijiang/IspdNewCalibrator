#ifndef SERIALPORTSET_H
#define SERIALPORTSET_H

#include <QWidget>
#include <QFile>
#include <QString>
#include "pugiconfig.hpp"
#include "pugixml.hpp"
#include "parsexml.h"

namespace Ui {
class SerialPortSet;
}

class SerialPortSet : public QWidget
{
    Q_OBJECT

public:
    explicit SerialPortSet(QWidget *parent = 0);
    ~SerialPortSet();
    void serPort_start();
    int init();

private slots:
    void on_pushButton_sport_set_clicked();

    void on_tabWidget_tabBarClicked();

    void on_pushButton_clicked();

private:
    Ui::SerialPortSet *ui;
    ParseXml *m_sport_xml;

};

#endif // SERIALPORTSET_H
