#include "restartmr.h"
#include "ui_restartmr.h"

RestartMr::RestartMr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestartMr)
{
    ui->setupUi(this);
}

RestartMr::~RestartMr()
{
    delete ui;
}

void RestartMr::RestartMrInit()
{
    this->setWindowIcon(QIcon(":/startup.png"));
    this->setWindowTitle(QString::fromLocal8Bit("重启指定终端"));
    this->showNormal();
}

void RestartMr::on_pushButton_clicked()
{

}
