#include "startupmrautofactoryreset.h"
#include "ui_startupmrautofactoryreset.h"

StartUpMrAutoFactoryReset::StartUpMrAutoFactoryReset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartUpMrAutoFactoryReset)
{
    ui->setupUi(this);
}

StartUpMrAutoFactoryReset::~StartUpMrAutoFactoryReset()
{
    delete ui;
}

void StartUpMrAutoFactoryReset::StartUpMrAutoFactoryReset_start()
{
    this->setWindowIcon(QIcon(":/openreset.png"));
    this->setWindowTitle(QString::fromLocal8Bit("开启接入终端自动恢复出厂设置功能"));
    this->showNormal();
}

void StartUpMrAutoFactoryReset::on_pushButton_clicked()
{

}
