#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QDebug>
#include <iostream>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <iostream>
#include <QScrollArea>

#include "serialport.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_version(new Version),
    m_question(new Question), m_doc(new Document),
    m_port_set(new SerialPortSet)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->m_version;
    delete this->m_question;
    delete this->m_doc;
    delete this->m_port_set;
}

void MainWindow::VersionSlot()
{
    this->m_version->ver_start();
}

void MainWindow::QuestionSlot()
{
    this->m_question->qus_start();
}

void MainWindow::DocSlot()
{
    this->m_doc->doc_start();
}

void MainWindow::SerialPortSlot()
{
    this->m_port_set->serPort_start();
}

void MainWindow::TogetherTestSlot()
{
    this->ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::RecordSlot()
{
    this->ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::CheckComSlot()
{
    this->ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::CloseApp()
{
    this->close();
}

void MainWindow::start()
{
    for(int i=0; i < 10; i++)
    {
        QFrame *f = new QFrame();
        QFrame *f2 = new QFrame();
        QLineEdit *a=new QLineEdit( f );
        QLabel *b= new QLabel( f2 );
        ui->verticalLayout_7->addWidget(f);
        //ui->verticalLayout_7->addWidget(f2);
        b->setText(QString::number(i));
        a->setText(QString::number(i));
    }

    this->test();

    /*
    for(int i = 0; i < 20; ++i)
    {
        BaseUi *bgui = new BaseUi("com" + QString::number(i),
                                  "com" + QString::number(i),
                                  "com" + QString::number(i),
                                  "com" + QString::number(i),
                                  "com" + QString::number(i));

        bgui->MulRecordUi(i);
        ui->scrollArea_record->setWidget(bgui->m_page);
        //ui->scrollArea_record->setWidget(bgui->m_frame);
        //ui->verticalLayout_7->addWidget(bgui->m_frame);
    }
    */

    this->setWindowTitle(QString::fromLocal8Bit("超版板上位机测试软件"));
    this->showMaximized();

    //版本管理
    connect(this->ui->action_version, &QAction::triggered, this, &MainWindow::VersionSlot);

    //常见问题解决
    connect(this->ui->action_question, &QAction::triggered, this, &MainWindow::QuestionSlot);

    //操作文档说明
    connect(this->ui->action_doc, &QAction::triggered, this, &MainWindow::DocSlot);

    //串口通信设置
    connect(this->ui->action_serialport_set, &QAction::triggered, this, &MainWindow::SerialPortSlot);

    //合并测试
    connect(this->ui->action_hb, &QAction::triggered, this, &MainWindow::TogetherTestSlot);

    //录入标签
    connect(this->ui->action_record, &QAction::triggered, this, &MainWindow::RecordSlot);

    //检查通信
    connect(this->ui->action_com, &QAction::triggered, this, &MainWindow::CheckComSlot);

    //退出程序
    connect(this->ui->action_eixit, &QAction::triggered, this, &MainWindow::CloseApp);
}

void MainWindow::test()
{
    int frameStyle = QFrame::Sunken | QFrame::Panel;

    integerLabel = new QLabel;
    integerLabel->setFrameStyle(frameStyle);
    integerLabel->setText("com1");
    QLineEdit *integerLine = new QLineEdit;
    integerLine->setText("1001001");
    QPushButton *integerButton = new QPushButton("record");

    doubleLabel = new QLabel;
    doubleLabel->setFrameStyle(frameStyle);
    doubleLabel->setText("com2");

    QLineEdit *doubleLine = new QLineEdit;
    doubleLine->setText("1001002");
    QPushButton *doubleButton = new QPushButton("record");

    itemLabel = new QLabel;
    itemLabel->setFrameStyle(frameStyle);
    itemLabel->setText("com3");
    QLineEdit *itemLine = new QLineEdit;
    itemLine->setText("1001003");
    QPushButton *itemButton = new QPushButton("record");

    textLabel = new QLabel;
    textLabel->setFrameStyle(frameStyle);
    textLabel->setText("com3");
    QLineEdit *textLine = new QLineEdit;
    textLine->setText("1001004");
    QPushButton *textButton = new QPushButton("record");

    multiLineTextLabel = new QLabel;
    multiLineTextLabel->setFrameStyle(frameStyle);
    multiLineTextLabel->setText("com5");
    QLineEdit *multiLineLine = new QLineEdit;
    multiLineLine->setText("1001005");
    QPushButton *multiLineTextButton = new QPushButton("record");

    integerLabelx = new QLabel;
    integerLabelx->setFrameStyle(frameStyle);
    integerLabelx->setText("com1");
    QLineEdit *integerLinex = new QLineEdit;
    integerLinex->setText("1001001");
    QPushButton *integerButtonx = new QPushButton("record");

    doubleLabelx = new QLabel;
    doubleLabelx->setFrameStyle(frameStyle);
    doubleLabelx->setText("com2");
    QLineEdit *doubleLinex = new QLineEdit;
    doubleLinex->setText("1001002");
    QPushButton *doubleButtonx = new QPushButton("record");

    itemLabelx = new QLabel;
    itemLabelx->setFrameStyle(frameStyle);
    itemLabelx->setText("com3");
    QLineEdit *itemLinex = new QLineEdit;
    itemLinex->setText("1001003");
    QPushButton *itemButtonx = new QPushButton("record");

    textLabelx = new QLabel;
    textLabelx->setFrameStyle(frameStyle);
    textLabelx->setText("com3");
    QLineEdit *textLinex = new QLineEdit;
    textLinex->setText("1001004");
    QPushButton *textButtonx = new QPushButton("record");

    multiLineTextLabelx = new QLabel;
    multiLineTextLabelx->setFrameStyle(frameStyle);
    multiLineTextLabelx->setText("com5");
    QLineEdit *multiLineLinex = new QLineEdit;
    multiLineLinex->setText("1001005");
    QPushButton *multiLineTextButtonx = new QPushButton("record");

    integerLabely = new QLabel;
    integerLabely->setFrameStyle(frameStyle);
    integerLabely->setText("com1");
    QLineEdit *integerLiney = new QLineEdit;
    integerLiney->setText("1001001");
    QPushButton *integerButtony = new QPushButton("record");

    doubleLabely = new QLabel;
    doubleLabely->setFrameStyle(frameStyle);
    doubleLabely->setText("com2");

    QLineEdit *doubleLiney = new QLineEdit;
    doubleLiney->setText("1001002");
    QPushButton *doubleButtony = new QPushButton("record");

    itemLabely = new QLabel;
    itemLabely->setFrameStyle(frameStyle);
    itemLabely->setText("com3");
    QLineEdit *itemLiney = new QLineEdit;
    itemLiney->setText("1001003");
    QPushButton *itemButtony = new QPushButton("record");

    textLabely = new QLabel;
    textLabely->setFrameStyle(frameStyle);
    textLabely->setText("com3");
    QLineEdit *textLiney = new QLineEdit;
    textLiney->setText("1001004");
    QPushButton *textButtony = new QPushButton("record");

    multiLineTextLabely = new QLabel;
    multiLineTextLabely->setFrameStyle(frameStyle);
    multiLineTextLabely->setText("com5");
    QLineEdit *multiLineLiney = new QLineEdit;
    multiLineLiney->setText("1001005");
    QPushButton *multiLineTextButtony = new QPushButton("record");

    integerLabelz = new QLabel;
    integerLabelz->setFrameStyle(frameStyle);
    integerLabelz->setText("com1");
    QLineEdit *integerLinez = new QLineEdit;
    integerLinez->setText("1001001");
    QPushButton *integerButtonz = new QPushButton("record");

    doubleLabelz = new QLabel;
    doubleLabelz->setFrameStyle(frameStyle);
    doubleLabelz->setText("com2");

    QLineEdit *doubleLinez = new QLineEdit;
    doubleLinez->setText("1001002");
    QPushButton *doubleButtonz = new QPushButton("record");

    itemLabelz = new QLabel;
    itemLabelz->setFrameStyle(frameStyle);
    itemLabelz->setText("com3");
    QLineEdit *itemLinez = new QLineEdit;
    itemLinez->setText("1001003");
    QPushButton *itemButtonz = new QPushButton("record");

    textLabelz = new QLabel;
    textLabelz->setFrameStyle(frameStyle);
    textLabelz->setText("com3");
    QLineEdit *textLinez = new QLineEdit;
    textLinez->setText("1001004");
    QPushButton *textButtonz = new QPushButton("record");

    multiLineTextLabelz = new QLabel;
    multiLineTextLabelz->setFrameStyle(frameStyle);
    multiLineTextLabelz->setText("com5");
    QLineEdit *multiLineLinez = new QLineEdit;
    multiLineLinez->setText("1001005");
    QPushButton *multiLineTextButtonz = new QPushButton("record");

    QWidget *page = new QWidget;

    QGridLayout *layout = new QGridLayout(page);
    layout->setColumnStretch(1, 1);
    layout->setColumnMinimumWidth(1, 250);

    layout->addWidget(integerLabel, 0, 0);
    layout->addWidget(integerLine, 0, 1);
    layout->addWidget(integerButton, 0, 2);

    layout->addWidget(doubleLabel, 1, 0);
    layout->addWidget(doubleLine, 1, 1);
    layout->addWidget(doubleButton, 1, 2);

    layout->addWidget(itemLabel, 2, 0);
    layout->addWidget(itemLine, 2, 1);
    layout->addWidget(itemButton, 2, 2);

    layout->addWidget(textLabel, 3, 0);
    layout->addWidget(textLine, 3, 1);
    layout->addWidget(textButton, 3, 2);

    layout->addWidget(multiLineTextLabel, 4, 0);
    layout->addWidget(multiLineLine, 4, 1);
    layout->addWidget(multiLineTextButton, 4, 2);

    layout->addWidget(integerLabelx, 5, 0);
    layout->addWidget(integerLinex, 5, 1);
    layout->addWidget(integerButtonx, 5, 2);

    layout->addWidget(doubleLabelx, 6, 0);
    layout->addWidget(doubleLinex, 6, 1);
    layout->addWidget(doubleButtonx, 6, 2);

    layout->addWidget(itemLabelx, 7, 0);
    layout->addWidget(itemLinex, 7, 1);
    layout->addWidget(itemButtonx, 7, 2);

    layout->addWidget(textLabelx, 8, 0);
    layout->addWidget(textLinex, 8, 1);
    layout->addWidget(textButtonx, 8, 2);

    layout->addWidget(multiLineTextLabelx, 9, 0);
    layout->addWidget(multiLineLinex, 9, 1);
    layout->addWidget(multiLineTextButtonx, 9, 2);

    layout->addWidget(integerLabely, 10, 0);
    layout->addWidget(integerLiney, 10, 1);
    layout->addWidget(integerButtony, 10, 2);

    layout->addWidget(doubleLabely, 11, 0);
    layout->addWidget(doubleLiney, 11, 1);
    layout->addWidget(doubleButtony, 11, 2);

    layout->addWidget(itemLabely, 12, 0);
    layout->addWidget(itemLiney, 12, 1);
    layout->addWidget(itemButtony, 12, 2);

    layout->addWidget(textLabely, 13, 0);
    layout->addWidget(textLiney, 13, 1);
    layout->addWidget(textButtony, 13, 2);

    layout->addWidget(multiLineTextLabely, 14, 0);
    layout->addWidget(multiLineLiney, 14, 1);
    layout->addWidget(multiLineTextButtony, 14, 2);

    layout->addWidget(integerLabelz, 15, 0);
    layout->addWidget(integerLinez, 15, 1);
    layout->addWidget(integerButtonz, 15, 2);

    layout->addWidget(doubleLabelz, 16, 0);
    layout->addWidget(doubleLinez, 16, 1);
    layout->addWidget(doubleButtonz, 16, 2);

    layout->addWidget(itemLabelz, 17, 0);
    layout->addWidget(itemLinez, 17, 1);
    layout->addWidget(itemButtonz, 17, 2);

    layout->addWidget(textLabelz, 18, 0);
    layout->addWidget(textLinez, 18, 1);
    layout->addWidget(textButtonz, 18, 2);

    layout->addWidget(multiLineTextLabelz,19, 0);
    layout->addWidget(multiLineLinez, 19, 1);
    layout->addWidget(multiLineTextButtonz, 19, 2);

    page->setLayout(layout);
    this->ui->scrollArea_record->setWidget(page);
    //this->ui->scrollArea_h_record->setWidget(page);
}

