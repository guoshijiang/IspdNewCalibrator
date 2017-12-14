#ifndef READSPECIFICCONFIG_H
#define READSPECIFICCONFIG_H

#include <QDialog>

namespace Ui {
class ReadSpecificConfig;
}

class ReadSpecificConfig : public QDialog
{
    Q_OBJECT

public:
    explicit ReadSpecificConfig(QWidget *parent = 0);
    ~ReadSpecificConfig();
    void ReadSpecConfig_start();

private:
    Ui::ReadSpecificConfig *ui;
};

#endif // READSPECIFICCONFIG_H
