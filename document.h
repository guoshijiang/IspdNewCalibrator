#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QWidget>

namespace Ui {
class Document;
}

class Document : public QWidget
{
    Q_OBJECT

public:
    explicit Document(QWidget *parent = 0);
    ~Document();
    void doc_start();
    void CloseApp();

private slots:

private:
    Ui::Document *ui;
};

#endif // DOCUMENT_H
