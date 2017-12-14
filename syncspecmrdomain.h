#ifndef SYNCSPECMRDOMAIN_H
#define SYNCSPECMRDOMAIN_H

#include <QDialog>

namespace Ui {
class SyncSpecMrDomain;
}

class SyncSpecMrDomain : public QDialog
{
    Q_OBJECT

public:
    explicit SyncSpecMrDomain(QWidget *parent = 0);
    ~SyncSpecMrDomain();
    void SyncSpecMrDomainInit();

private:
    Ui::SyncSpecMrDomain *ui;

private:
    SyncSpecMrDomain(const SyncSpecMrDomain &);
    const SyncSpecMrDomain & operator = (const SyncSpecMrDomain &);
};

#endif // SYNCSPECMRDOMAIN_H
