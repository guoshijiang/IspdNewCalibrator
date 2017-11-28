#include "baseui.h"
#include <QFrame>

using namespace Common;

BaseUi::BaseUi(): m_con_group(new QGroupBox)
{

}

BaseUi::~BaseUi()
{
    delete m_con_group;
}

void BaseUi::BaseScrollArea()
{
    QScrollArea *pArea = new QScrollArea();
    QWidget *pWidget = new QWidget();
    pWidget->setStyleSheet("QWidget" "{background:white;}");
    m_vbox_layout = new QVBoxLayout();
    m_vbox_layout->addSpacerItem(new QSpacerItem(100, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    pWidget->setLayout(m_vbox_layout);
    pArea->setWidget(pWidget);
    pArea->setWidgetResizable(true);
    m_main_layout = new QVBoxLayout();
    m_main_layout->addWidget(pArea);
}

void BaseUi::addAutoRecordUi(QString lab_neme, QString ledit_name)
{
    QWidget *page = new QWidget;
    QGridLayout *layout = new QGridLayout(page);

    QLabel *label = new QLabel;
    label->setText(lab_neme);
    label->setFont(font());

    QLineEdit *ledit = new QLineEdit;
    ledit->setText(ledit_name);
    ledit->setFont(font());

    layout->addWidget(label, 0, 1);
    layout->addWidget(ledit, 0, 2);

    page->setLayout(layout);

    m_vbox_layout->insertWidget(m_vbox_layout->count()-1, page);

}

void BaseUi::addMulRecordUi(QString lab_neme, QString ledit_name, QString but_name)
{
    QWidget *page = new QWidget;
    QGridLayout *layout = new QGridLayout(page);

    QLabel *label = new QLabel;
    label->setText(lab_neme);
    label->setFont(font());

    QLineEdit *ledit = new QLineEdit;
    ledit->setText(ledit_name);
    ledit->setFont(font());

    QPushButton *but = new QPushButton(but_name);
    but->setFont(font());

    layout->addWidget(label, 0, 1);
    layout->addWidget(ledit, 0, 2);
    layout->addWidget(but, 0, 3);

    page->setLayout(layout);

    m_vbox_layout->insertWidget(m_vbox_layout->count()-1, page);
}

void BaseUi::CreateConfigUI()
{

}

void BaseUi::CreateHeartRateUI()
{

}

void BaseUi::CreateComDataUI()
{

}

void BaseUi::addCheckComUi()
{

    QWidget *page = new QWidget;

    QVBoxLayout *vbox = new QVBoxLayout(page);

    QGroupBox *con_group = new QGroupBox;

    QGridLayout *layout_config = new QGridLayout(con_group);

    con_group->setTitle(UTF8BIT("配置信息"));
    con_group->setFont(font());

    QLabel *ispd_id = new QLabel;
    ispd_id->setText(UTF8BIT("标签"));

    QLabel *sam_rate = new QLabel;
    sam_rate->setText(UTF8BIT("采用率"));

    QLabel *version = new QLabel;
    version->setText(UTF8BIT("版本号"));

    QLineEdit *ispd_id_edit = new QLineEdit;
    QLineEdit *sam_rate_edit = new QLineEdit;
    QLineEdit *version_edit = new QLineEdit;

    layout_config->addWidget(ispd_id, 0, 1);
    layout_config->addWidget(ispd_id_edit, 0, 2);

    layout_config->addWidget(sam_rate, 1, 1);
    layout_config->addWidget(sam_rate_edit, 1, 2);

    layout_config->addWidget(version, 2, 1);
    layout_config->addWidget(version_edit, 2, 2);



    QGroupBox *hr_group = new QGroupBox;

    QGridLayout *layout_hr = new QGridLayout(hr_group);

    hr_group->setTitle(UTF8BIT("心率"));
    hr_group->setFont(font());

    QLabel *hr_label = new QLabel;
    hr_label->setText(UTF8BIT("读取心率"));

    QLabel *hr_one = new QLabel;
    hr_one->setText(UTF8BIT("第一次"));

    QLabel *hr_two = new QLabel;
    hr_two->setText(UTF8BIT("第二次"));

    QLabel *hr_three = new QLabel;
    hr_three->setText(UTF8BIT("第三次"));

    QLabel *hr_avg = new QLabel;
    hr_avg->setText(UTF8BIT("平均值"));

    QLineEdit *hr_edit_one = new QLineEdit;
    QLineEdit *hr_edit_two = new QLineEdit;
    QLineEdit *hr_edit_three = new QLineEdit;
    QLineEdit *hr_edit_avg = new QLineEdit;


    layout_hr->addWidget(hr_label, 2, 1);

    layout_hr->addWidget(hr_one, 1, 2);
    layout_hr->addWidget(hr_two, 1, 3);
    layout_hr->addWidget(hr_three, 1, 4);
    layout_hr->addWidget(hr_avg, 1, 5);

    layout_hr->addWidget(hr_edit_one, 2, 2);
    layout_hr->addWidget(hr_edit_two, 2, 3);
    layout_hr->addWidget(hr_edit_three, 2, 4);
    layout_hr->addWidget(hr_edit_avg, 2, 5);

    QGroupBox *compare_group = new QGroupBox;

    QGridLayout *layout_datapare = new QGridLayout(compare_group);

    compare_group->setTitle(UTF8BIT("对比数据"));
    compare_group->setFont(font());

    QLabel *seriport_ret = new QLabel;
    seriport_ret->setText(UTF8BIT("串口结果"));

    QLabel *mr_ret = new QLabel;
    mr_ret->setText(UTF8BIT("基站结果"));

    QLabel *comp_ret = new QLabel;
    comp_ret->setText(UTF8BIT("对比结果"));


    QLabel *a_speed = new QLabel;
    a_speed->setText(UTF8BIT("加速度"));

    QLineEdit *edit_a_seripor = new QLineEdit;
    QLineEdit *edit_a_mr = new QLineEdit;
    QLineEdit *edit_a_comp = new QLineEdit;


    QLabel *gyro = new QLabel;
    gyro->setText(UTF8BIT("陀螺仪"));

    QLineEdit *edit_gyro_seripor = new QLineEdit;
    QLineEdit *edit_gyro_mr = new QLineEdit;
    QLineEdit *edit_gyro_comp = new QLineEdit;


    QLabel *heart_rate = new QLabel;
    heart_rate->setText(UTF8BIT("心率"));

    QLineEdit *edit_hr_seripor = new QLineEdit;
    QLineEdit *edit_hr_mr = new QLineEdit;
    QLineEdit *edit_hr_comp = new QLineEdit;

    QLabel *bat = new QLabel;
    bat->setText(UTF8BIT("电量"));

    QLineEdit *edit_bat_seripor = new QLineEdit;
    QLineEdit *edit_bat_mr = new QLineEdit;
    QLineEdit *edit_bat_comp = new QLineEdit;


    QLabel *charge = new QLabel;
    charge->setText(UTF8BIT("充电状态"));

    QLineEdit *edit_charge = new QLineEdit;


    layout_datapare->addWidget(seriport_ret, 0, 2);
    layout_datapare->addWidget(mr_ret, 0, 3);
    layout_datapare->addWidget(comp_ret, 0, 4);


    layout_datapare->addWidget(a_speed, 1, 1);
    layout_datapare->addWidget(edit_a_seripor, 1, 2);
    layout_datapare->addWidget(edit_a_mr, 1, 3);
    layout_datapare->addWidget(edit_a_comp, 1, 4);

    layout_datapare->addWidget(gyro, 2, 1);
    layout_datapare->addWidget(edit_gyro_seripor, 2, 2);
    layout_datapare->addWidget(edit_gyro_mr, 2, 3);
    layout_datapare->addWidget(edit_gyro_comp, 2, 4);

    layout_datapare->addWidget(heart_rate, 3, 1);
    layout_datapare->addWidget(edit_hr_seripor, 3, 2);
    layout_datapare->addWidget(edit_hr_mr, 3, 3);
    layout_datapare->addWidget(edit_hr_comp, 3, 4);

    layout_datapare->addWidget(bat, 4, 1);
    layout_datapare->addWidget(edit_bat_seripor, 4, 2);
    layout_datapare->addWidget(edit_bat_mr, 4, 3);
    layout_datapare->addWidget(edit_bat_comp, 4, 4);


    layout_datapare->addWidget(charge, 5, 1);
    layout_datapare->addWidget(edit_charge, 5, 2);

    vbox->addWidget(con_group);
    vbox->addWidget(hr_group);
    vbox->addWidget(compare_group);

    page->setLayout(vbox);

    m_vbox_layout->insertWidget(m_vbox_layout->count()-1, page);
}

