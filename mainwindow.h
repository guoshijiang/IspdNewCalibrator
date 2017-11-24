#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include "version.h"
#include "question.h"
#include "document.h"
#include "serialportset.h"
#include <QToolBox>
#include "baseui.h"

/*
typedef struct _ControlKit
{
    QString label_name;
    QString ledit_name;
    QString but_name;
    QString ledit_value;
}controlkit;
*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void start();
    void test();
    //void testList(QLabel *label, QLineEdit *lineEdit, QPushButton *button);
public slots:
    void VersionSlot();
    void QuestionSlot();
    void DocSlot();
    void SerialPortSlot();
    void TogetherTestSlot();
    void RecordSlot();
    void CheckComSlot();
    void CloseApp();
private:
    Ui::MainWindow *ui;
    Version *m_version;
    Question *m_question;
    Document *m_doc;
    SerialPortSet *m_port_set;

public:
    //controlkit m_con_kit;
    QLabel *integerLabel;
    QLabel *doubleLabel;
    QLabel *itemLabel;
    QLabel *textLabel;
    QLabel *multiLineTextLabel;


    QLabel *integerLabelx;
    QLabel *doubleLabelx;
    QLabel *itemLabelx;
    QLabel *textLabelx;
    QLabel *multiLineTextLabelx;

    QLabel *integerLabely;
    QLabel *doubleLabely;
    QLabel *itemLabely;
    QLabel *textLabely;
    QLabel *multiLineTextLabely;

    QLabel *integerLabelz;
    QLabel *doubleLabelz;
    QLabel *itemLabelz;
    QLabel *textLabelz;
    QLabel *multiLineTextLabelz;

};

#endif // MAINWINDOW_H
