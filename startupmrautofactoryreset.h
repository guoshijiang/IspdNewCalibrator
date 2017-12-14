#ifndef STARTUPMRAUTOFACTORYRESET_H
#define STARTUPMRAUTOFACTORYRESET_H

#include <QDialog>

namespace Ui {
class StartUpMrAutoFactoryReset;
}

class StartUpMrAutoFactoryReset : public QDialog
{
    Q_OBJECT

public:
    explicit StartUpMrAutoFactoryReset(QWidget *parent = 0);
    ~StartUpMrAutoFactoryReset();
    void StartUpMrAutoFactoryReset_start();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StartUpMrAutoFactoryReset *ui;

private:
    StartUpMrAutoFactoryReset(const StartUpMrAutoFactoryReset &);
    const StartUpMrAutoFactoryReset & operator = (const StartUpMrAutoFactoryReset &);
};

#endif // STARTUPMRAUTOFACTORYRESET_H
