#ifndef RESTARTMR_H
#define RESTARTMR_H

#include <QDialog>

namespace Ui {
class RestartMr;
}

class RestartMr : public QDialog
{
    Q_OBJECT

public:
    explicit RestartMr(QWidget *parent = 0);
    ~RestartMr();
    void RestartMrInit();

private slots:
    void on_pushButton_clicked();

private:
    Ui::RestartMr *ui;

private:
    RestartMr(const RestartMr &);
    const RestartMr & operator = (const RestartMr &);
};

#endif // RESTARTMR_H
