#include "document.h"
#include "ui_document.h"

Document::Document(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Document)
{
    ui->setupUi(this);
}

Document::~Document()
{
    delete ui;
}

void Document::doc_start()
{
    this->setWindowIcon(QIcon(":/operationfile.png"));
    this->setWindowTitle(QString::fromLocal8Bit("软件操作说明文档"));
    this->showNormal();
}
