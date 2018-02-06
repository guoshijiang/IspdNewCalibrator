#include "document.h"
#include "ui_document.h"
#include <QDir>

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
    QDir dir;
    this->setWindowIcon(QIcon(":/operationfile.png"));
    this->setWindowTitle(QString::fromLocal8Bit("软件操作说明文档"));
    this->showNormal();

    QString program_path = dir.currentPath();
    QString path = program_path + "/doc/index.html";
    QString urlName(path);
    QUrl url = QUrl::fromUserInput(urlName);
    ui->webView->load(url);
}

void Document::CloseApp()
{
    this->close();
}



