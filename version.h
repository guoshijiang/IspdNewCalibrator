#ifndef VERSION_H
#define VERSION_H

#include <QWidget>

namespace Ui {
class Version;
}

class Version : public QWidget
{
    Q_OBJECT

public:
    explicit Version(QWidget *parent = 0);
    ~Version();
    void ver_start();
    void ver_init();

private:
    Ui::Version *ui;
    QString m_ver_str;
};

#endif // VERSION_H
