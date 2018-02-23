#include "version.h"
#include "ui_version.h"

#define MESSAGE_RED "<font size = 200 color = red ><strong>"
#define MESSAGE_GREEN "<font size = 200 color = blue ><strong>"
#define MESSAGE_END "</strong></font>"

Version::Version(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Version)
{
    ui->setupUi(this);
}

Version::~Version()
{
    delete ui;
}

void Version::CloseApp()
{
    this->close();
}

void Version::ver_start()
{
    this->setWindowIcon(QIcon(":/versions.png"));
    this->setWindowTitle(QString::fromLocal8Bit("软件版本说明"));
    this->showNormal();
    this->ver_init();
}

void Version::ver_init()
{
    this->m_ver_str = QString::fromLocal8Bit("软件当前版本为V2.0.1");
    QString ver_info = MESSAGE_RED +m_ver_str+ MESSAGE_END;
    this->ui->label_current_ver->setText(ver_info);
}
