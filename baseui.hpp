#ifndef BASEUI_H
#define BASEUI_H

#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QFrame>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QFont>
#include <QDebug>
#include <QMap>
#include <QAction>
#include "common.h"

namespace InterfaceUI
{
    //UI界面基类
    class BaseUI : public QObject
    {
    public:
        BaseUI(): m_area(new QScrollArea), m_widget(new QWidget()),
            m_main_layout(new QVBoxLayout),
            m_vbox_layout(new QVBoxLayout)
        {

        }

        virtual ~BaseUI()
        {
            delete m_area;
            delete m_widget;
            delete m_main_layout;
            delete m_vbox_layout;
        }

        inline QFont font()
        {
            QFont m_font("宋体", 18, 75);
            return m_font;
        }

        inline void BaseScrollArea()
        {
            m_area = new QScrollArea();
            m_widget = new QWidget();
            m_widget->setStyleSheet("QWidget" "{background:white;}");
            m_vbox_layout->addSpacerItem(new QSpacerItem(100, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
            m_widget->setLayout(m_vbox_layout);
            m_area->setWidget(m_widget);
            m_area->setWidgetResizable(true);
            m_main_layout->addWidget(m_area);
        }
    public:
        virtual void AddAutoChildUI(QString com_name) = 0;
        virtual void AddMulChildUI() = 0;
        virtual void getValueFromUI() = 0;
    private:
       BaseUI(const BaseUI &);
       const BaseUI & operator = (const BaseUI &);
    public:
       QScrollArea *m_area;
       QWidget *m_widget;
       QVBoxLayout *m_vbox_layout;
       QVBoxLayout *m_main_layout;
    };

    //录入ID界面类
    class RecordUI : public BaseUI
    {
    public:
       RecordUI() : BaseUI()
       {

       }
       virtual ~RecordUI()
       {

       }

       inline void init()
       {
           m_record_page = new QWidget;
           m_record_layout = new QGridLayout(m_record_page);
           m_record_label = new QLabel;
           m_record_ledit = new QLineEdit;
           m_record_but = new QPushButton;
       }

       void desource()
       {
           if(m_record_page != NULL)
           {
               delete m_record_page;
               m_record_page = NULL;
           }
           if(m_record_layout != NULL)
           {
               delete m_record_layout;
               m_record_layout = NULL;
           }

           if(m_record_label != NULL)
           {
               delete m_record_label;
               m_record_label = NULL;
           }

           if(m_record_ledit != NULL)
           {
               delete m_record_ledit;
               m_record_ledit = NULL;
           }

           if(m_record_but != NULL)
           {
               delete m_record_but;
               m_record_but = NULL;
           }
       }

       virtual void AddAutoChildUI(QString com_name)
       {
           this->init();
           m_record_label->setText(com_name);
           m_record_label->setFont(font());

           m_record_ledit->setFont(font());

           m_record_layout->addWidget(m_record_label, 0, 1);
           m_record_layout->addWidget(m_record_ledit, 0, 2);

           m_record_page->setLayout(m_record_layout);
           m_vbox_layout->insertWidget(m_vbox_layout->count()-1, m_record_page);
       }

       virtual void AddMulChildUI()
       {
           this->init();
           m_record_label->setText("com1");
           m_record_label->setFont(font());

           m_record_ledit->setFont(font());

           m_record_but->setText(UTF8BIT("录入"));
           m_record_but->setFont(font());

           m_record_layout->addWidget(m_record_label, 0, 1);
           m_record_layout->addWidget(m_record_ledit, 0, 2);
           m_record_layout->addWidget(m_record_but, 0, 3);

           m_record_page->setLayout(m_record_layout);
           m_vbox_layout->insertWidget(m_vbox_layout->count()-1, m_record_page);

       }

       virtual void getValueFromUI()
       {

       }

    public slots:

    private:
       RecordUI(const RecordUI &);
       const RecordUI & operator = (const RecordUI &);
    public:
       QWidget *m_record_page;
       QGridLayout *m_record_layout;
       QLabel *m_record_label;
       QLineEdit *m_record_ledit;
       QPushButton *m_record_but;
    };

    //检测通信界面类
    class CheckUI : public BaseUI
    {
    public:
       CheckUI() : BaseUI(){}
       virtual ~CheckUI(){}
    public:

       virtual void AddAutoChildUI(QString com_name)
       {

       }

       virtual void AddMulChildUI()
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


       virtual void getValueFromUI()
       {

       }
    private:
       CheckUI(const CheckUI &);
       const CheckUI & operator = (const CheckUI &);

    public:

    };

    //合并测试界面类
    class RecordAndCheckUI : public BaseUI
    {
    public:
        RecordAndCheckUI(){}
        virtual ~RecordAndCheckUI(){}
    public:

        virtual void AddAutoChildUI(QString com_name)
        {

        }

        virtual void AddMulChildUI()
        {

        }

        virtual void getValueFromUI()
        {

        }

    private:
       RecordAndCheckUI(const RecordAndCheckUI &);
       const RecordAndCheckUI & operator = (const RecordAndCheckUI &);
    };

    //抽象工厂类
    class AbstractFactory
    {
    public:
        AbstractFactory(){}
        ~AbstractFactory(){}
    public:
        virtual BaseUI* CreatUIRole() = 0;
    private:
        AbstractFactory(const AbstractFactory &);
        const AbstractFactory & operator = (const AbstractFactory &);
    };

    //产生录入ID界面的工厂类
    class RecordUIFactory : public AbstractFactory
    {
    public:
        RecordUIFactory(){}
        ~RecordUIFactory(){}
    public:
        virtual BaseUI* CreatUIRole()
        {
            return new RecordUI;
        }
    private:
        RecordUIFactory(const RecordUIFactory &);
        const RecordUIFactory & operator = (const RecordUIFactory &);
    };

    //产生检测通信的UI界面工厂类
    class CheckUIFactory : public AbstractFactory
    {
    public:
        CheckUIFactory(){}
        ~CheckUIFactory(){}
    public:
        virtual BaseUI* CreatUIRole()
        {
            return new CheckUI;
        }
    private:
        CheckUIFactory(const CheckUIFactory &);
        const CheckUIFactory & operator = (const CheckUIFactory &);
    };

    //产生合并测试UI界面的工厂类
    class RecordAndCheckUIFactory : public AbstractFactory
    {
    public:
        RecordAndCheckUIFactory(){}
        ~RecordAndCheckUIFactory(){}
    public:
        virtual BaseUI* CreatUIRole()
        {
            return new RecordAndCheckUI;
        }
    private:
        RecordAndCheckUIFactory(const RecordAndCheckUIFactory &);
        const RecordAndCheckUIFactory & operator = (const RecordAndCheckUIFactory &);
    };
}

#endif // BASEUI_H
