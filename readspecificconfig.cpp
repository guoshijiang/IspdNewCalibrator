#include "readspecificconfig.h"
#include "ui_readspecificconfig.h"

ReadSpecificConfig::ReadSpecificConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadSpecificConfig)
{
    ui->setupUi(this);
}

ReadSpecificConfig::~ReadSpecificConfig()
{
    delete ui;
}

void ReadSpecificConfig::ReadSpecConfig_start()
{
    this->setWindowIcon(QIcon(":/read_mr.png"));
    this->setWindowTitle(QString::fromLocal8Bit("读指定终端配置信息"));
    this->showNormal();
}
