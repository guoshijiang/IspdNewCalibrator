#include "question.h"
#include "ui_question.h"

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

void Question::qus_start()
{
    this->setWindowIcon(QIcon(":/question.png"));
    this->setWindowTitle(QString::fromLocal8Bit("常见问题解决"));
    this->showNormal();
}
