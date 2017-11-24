#include "baseui.h"
#include <QFrame>

BaseUi::BaseUi(QString label_name, QString edit_name, QString button_name,
       QString edit_value, QString but_record) : m_page(new QWidget),
       m_frame(new QFrame)
{
    this->m_label_name = label_name;
    this->m_edit_name = edit_name;
    this->m_button_name = button_name;
    this->m_edit_value = edit_value;
    this->m_but_record = but_record;
}

BaseUi::~BaseUi()
{
    delete this->m_page;
    delete this->m_frame;
}

void BaseUi::AutoRecordUi()
{

}

void BaseUi::MulRecordUi(int layout_num)
{

    this->m_frameStyle = QFrame::Sunken | QFrame::Panel;

    this->m_label = new QLabel();
    this->m_label->setFrameStyle(this->m_frameStyle);
    this->m_label->setText(this->m_label_name);

    this->m_edit = new QLineEdit();
    this->m_edit->setText(this->m_edit_value);

    this->m_but = new QPushButton(this->m_button_name);

    m_layout = new QGridLayout;

    m_layout->setColumnStretch(1, 1);
    m_layout->setColumnMinimumWidth(1, 250);

    m_layout->addWidget(this->m_label, layout_num, 0);
    m_layout->addWidget(this->m_edit, layout_num, 1);
    m_layout->addWidget(this->m_but, layout_num, 2);

    //m_layout->addWidget(this->m_frame);
    //m_frame->setLayout(m_layout);
    this->m_page->setLayout(m_layout);

}

