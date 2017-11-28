#ifndef BASEUI_H
#define BASEUI_H

#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QFrame>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QFont>
#include <QDebug>

#include "common.h"

class BaseUi : public QObject
{
     Q_OBJECT

public:
    BaseUi();
    ~BaseUi();
    void BaseScrollArea();
    inline QFont font()
    {
        QFont m_font("宋体", 18, 75);
        return m_font;
    }

public slots:
    void addAutoRecordUi(QString lab_neme, QString ledit_name);
    void addMulRecordUi(QString lab_neme, QString ledit_name, QString but_name);
    void addCheckComUi();
private:
    void CreateConfigUI();
    void CreateHeartRateUI();
    void CreateComDataUI();
public:
    QVBoxLayout *m_vbox_layout;
    QVBoxLayout *m_main_layout;
    QGroupBox *m_con_group;
};

#endif // BASEUI_H
