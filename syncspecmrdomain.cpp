#include "syncspecmrdomain.h"
#include "ui_syncspecmrdomain.h"

SyncSpecMrDomain::SyncSpecMrDomain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SyncSpecMrDomain)
{
    ui->setupUi(this);
}

SyncSpecMrDomain::~SyncSpecMrDomain()
{
    delete ui;
}

void SyncSpecMrDomain::SyncSpecMrDomainInit()
{
    this->setWindowIcon(QIcon(":/synchronization.png"));
    this->setWindowTitle(QString::fromLocal8Bit("同步指定终端的Domain"));
    this->showNormal();
}
