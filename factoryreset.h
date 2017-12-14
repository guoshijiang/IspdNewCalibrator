#ifndef FACTORYRESET_H
#define FACTORYRESET_H

#include <QDialog>

namespace Ui {
class FactoryReset;
}

class FactoryReset : public QDialog
{
    Q_OBJECT

public:
    explicit FactoryReset(QWidget *parent = 0);
    ~FactoryReset();
    void FactoryResetInit();

private slots:
    void on_pushButton_clicked();

private:
    Ui::FactoryReset *ui;

private:
    FactoryReset(const FactoryReset &);
    const FactoryReset & operator = (const FactoryReset &);
};

#endif // FACTORYRESET_H
