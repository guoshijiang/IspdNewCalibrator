#ifndef QUESTION_H
#define QUESTION_H

#include <QWidget>

namespace Ui {
class Question;
}

class Question : public QWidget
{
    Q_OBJECT

public:
    explicit Question(QWidget *parent = 0);
    ~Question();
    void qus_start();
    void CloseApp();

private:
    Ui::Question *ui;
};

#endif // QUESTION_H
