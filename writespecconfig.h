#ifndef WRITESPECCONFIG_H
#define WRITESPECCONFIG_H

#include <QDialog>

namespace Ui {
class WriteSpecConfig;
}

class WriteSpecConfig : public QDialog
{
    Q_OBJECT

public:
    explicit WriteSpecConfig(QWidget *parent = 0);
    ~WriteSpecConfig();
    void WriteSpecConfig_start();

private:
    Ui::WriteSpecConfig *ui;
};

#endif // WRITESPECCONFIG_H
