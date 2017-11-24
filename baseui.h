#ifndef BASEUI_H
#define BASEUI_H

#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QFrame>
#include <QGridLayout>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>


class BaseUi : public QObject
{
     Q_OBJECT

public:
    BaseUi(QString label_name, QString edit_name, QString button_name,
           QString edit_value, QString but_record);
    ~BaseUi();

    void AutoRecordUi();

    void MulRecordUi(int layout_num);

public slots:

public:
    QString m_label_name;
    QString m_edit_name;
    QString m_button_name;
    QString m_edit_value;
    QString m_but_record;
    QLabel *m_label;
    QLineEdit *m_edit;
    QPushButton *m_but;
    QWidget *m_page;
    int m_frameStyle;
    QGridLayout *m_layout;
    QFrame *m_frame;
};

#endif // BASEUI_H
