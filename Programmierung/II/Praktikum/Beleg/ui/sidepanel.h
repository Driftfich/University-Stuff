#ifndef _SIDEPANEL_H
#define _SIDEPANEL_H

/********************************************************************************
** Form generated from reading UI file 'designerIDvfSl.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/


#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *infopanel;
    QWidget *addpanel;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(996, 634);
        horizontalLayout = new QHBoxLayout(Form);
        horizontalLayout->setObjectName("horizontalLayout");
        stackedWidget = new QStackedWidget(Form);
        stackedWidget->setObjectName("stackedWidget");
        infopanel = new QWidget();
        infopanel->setObjectName("infopanel");
        stackedWidget->addWidget(infopanel);
        addpanel = new QWidget();
        addpanel->setObjectName("addpanel");
        stackedWidget->addWidget(addpanel);

        horizontalLayout->addWidget(stackedWidget);


        retranslateUi(Form);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif