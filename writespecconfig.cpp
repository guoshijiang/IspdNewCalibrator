#include "writespecconfig.h"
#include "ui_writespecconfig.h"

WriteSpecConfig::WriteSpecConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WriteSpecConfig)
{
    ui->setupUi(this);
}

WriteSpecConfig::~WriteSpecConfig()
{
    delete ui;
}

void  WriteSpecConfig::WriteSpecConfig_start()
{
    this->setWindowIcon(QIcon(":/write_mr.png"));
    this->setWindowTitle(QString::fromLocal8Bit("写指定终端配置信息"));
    this->showNormal();
}
