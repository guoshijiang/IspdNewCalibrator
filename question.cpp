#include "question.h"
#include "ui_question.h"
#include <QDir>

Question::Question(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Question)
{
    ui->setupUi(this);
}

Question::~Question()
{
    delete ui;
}

void Question::CloseApp()
{
    this->close();
}

void Question::qus_start()
{
    QDir dir;
    this->setWindowIcon(QIcon(":/question.png"));
    this->setWindowTitle(QString::fromLocal8Bit("常见问题解决"));
    this->showNormal();

    QString program_path = dir.currentPath();
    QString path = program_path + "/doc/question.html";
    QString urlName(path);
    QUrl url = QUrl::fromUserInput(urlName);
    ui->webView->load(url);
}
