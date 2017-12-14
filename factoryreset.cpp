#include "factoryreset.h"
#include "ui_factoryreset.h"

FactoryReset::FactoryReset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FactoryReset)
{
    ui->setupUi(this);
}

FactoryReset::~FactoryReset()
{
    delete ui;
}

void FactoryReset::FactoryResetInit()
{
    this->setWindowIcon(QIcon(":/resetup.png"));
    this->setWindowTitle(QString::fromLocal8Bit("恢复指定终端出厂设置"));
    this->showNormal();
}

void FactoryReset::on_pushButton_clicked()
{

}
