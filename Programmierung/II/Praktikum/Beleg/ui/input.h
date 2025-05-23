#ifndef _INPUTFORM_H
#define _INPUTFORM_H

/********************************************************************************
** Form generated from reading UI file 'designerrArsWi.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InputWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QTreeView *FormTree;

    void setupUi(QWidget *InputWidget)
    {
        if (InputWidget->objectName().isEmpty())
            InputWidget->setObjectName("InputWidget");
        InputWidget->resize(1190, 717);
        horizontalLayout = new QHBoxLayout(InputWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        FormTree = new QTreeView(InputWidget);
        FormTree->setObjectName("FormTree");

        horizontalLayout_2->addWidget(FormTree);


        horizontalLayout->addLayout(horizontalLayout_2);


        retranslateUi(InputWidget);

        QMetaObject::connectSlotsByName(InputWidget);
    } // setupUi

    void retranslateUi(QWidget *InputWidget)
    {
        InputWidget->setWindowTitle(QCoreApplication::translate("InputWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InputWidget: public Ui_InputWidget {};
} // namespace Ui

QT_END_NAMESPACE


#endif