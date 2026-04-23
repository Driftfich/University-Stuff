/*
Author: Franz Rehschuh
Date: 2025-06-20

Description:
Qt Widget responsible for the table widget
- contains a tab widget for the different tables
- contains a table view for the different tables
*/

/********************************************************************************
** Form generated from reading UI file 'tablewidgetaAVvLa.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef _TABLEWIDGET_H
#define _TABLEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TableWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *TableLayout;
    QTabWidget *TabSelector;
    QWidget *personwid;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *persgrid;
    QTableView *persontab;
    QWidget *itemwid;
    QHBoxLayout *horizontalLayout;
    QGridLayout *itemgrid;
    QTableView *itemtab;
    QWidget *transwid;
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *transgrid;
    QTableView *transtab;

    void setupUi(QWidget *TableWidget)
    {
        if (TableWidget->objectName().isEmpty())
            TableWidget->setObjectName("TableWidget");
        TableWidget->resize(849, 307);
        verticalLayout = new QVBoxLayout(TableWidget);
        verticalLayout->setObjectName("verticalLayout");
        TableLayout = new QGridLayout();
        TableLayout->setObjectName("TableLayout");
        TabSelector = new QTabWidget(TableWidget);
        TabSelector->setObjectName("TabSelector");
        personwid = new QWidget();
        personwid->setObjectName("personwid");
        horizontalLayout_2 = new QHBoxLayout(personwid);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        persgrid = new QGridLayout();
        persgrid->setObjectName("persgrid");
        persontab = new QTableView(personwid);
        persontab->setObjectName("persontab");

        persgrid->addWidget(persontab, 0, 0, 1, 1);


        horizontalLayout_2->addLayout(persgrid);

        TabSelector->addTab(personwid, QString());
        itemwid = new QWidget();
        itemwid->setObjectName("itemwid");
        horizontalLayout = new QHBoxLayout(itemwid);
        horizontalLayout->setObjectName("horizontalLayout");
        itemgrid = new QGridLayout();
        itemgrid->setObjectName("itemgrid");
        itemtab = new QTableView(itemwid);
        itemtab->setObjectName("itemtab");

        itemgrid->addWidget(itemtab, 0, 0, 1, 1);


        horizontalLayout->addLayout(itemgrid);

        TabSelector->addTab(itemwid, QString());
        transwid = new QWidget();
        transwid->setObjectName("transwid");
        horizontalLayout_3 = new QHBoxLayout(transwid);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        transgrid = new QGridLayout();
        transgrid->setObjectName("transgrid");
        transtab = new QTableView(transwid);
        transtab->setObjectName("transtab");

        transgrid->addWidget(transtab, 0, 0, 1, 1);


        horizontalLayout_3->addLayout(transgrid);

        TabSelector->addTab(transwid, QString());

        TableLayout->addWidget(TabSelector, 0, 0, 1, 1);


        verticalLayout->addLayout(TableLayout);


        retranslateUi(TableWidget);

        TabSelector->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TableWidget);
    } // setupUi

    void retranslateUi(QWidget *TableWidget)
    {
        TableWidget->setWindowTitle(QCoreApplication::translate("TableWidget", "Form", nullptr));
        TabSelector->setTabText(TabSelector->indexOf(personwid), QCoreApplication::translate("TableWidget", "Persons", nullptr));
        TabSelector->setTabText(TabSelector->indexOf(itemwid), QCoreApplication::translate("TableWidget", "Items", nullptr));
        TabSelector->setTabText(TabSelector->indexOf(transwid), QCoreApplication::translate("TableWidget", "Transactions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TableWidget: public Ui_TableWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TABLEWIDGETAAVVLA_H
