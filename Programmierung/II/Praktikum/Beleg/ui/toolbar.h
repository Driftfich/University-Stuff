/** 
 * Author: Franz Rehschuh
 * Date: 2025-06-20
 * 
 * Description:
 * Qt Widget responsible for the toolbar
 * - Contains a LineEdit for searching the table
 * - contains dropdown for selecting the columns to view
 * - contains dropdown for selecting the sorting order
 * - contains a button to add a new entry
 * - contains a button to delete the selected entries
 */

#ifndef TOOLBAR_H
#define TOOLBAR_H

/********************************************************************************
** Form generated from reading UI file 'designerQSIYFo.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
********************************************************************************/

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QIcon>
#include <QListView>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <variant>

#include "libitemtablemodel.h"
#include "transactiontablemodel.h"
#include "persontablemodel.h"

QT_BEGIN_NAMESPACE

class Ui_toolbar
{
public:
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *searchbar;
    QSpacerItem *horizontalSpacer;
    QComboBox *columns;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *sort;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *add;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *deletec;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *toolbar)
    {
        if (toolbar->objectName().isEmpty())
            toolbar->setObjectName("toolbar");
        toolbar->resize(791, 112);
        verticalLayout_2 = new QVBoxLayout(toolbar);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        searchbar = new QLineEdit(toolbar);
        searchbar->setObjectName("searchbar");
        QSizePolicy sizePolicy(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(searchbar->sizePolicy().hasHeightForWidth());
        searchbar->setSizePolicy(sizePolicy);
        searchbar->setMinimumSize(QSize(150, 0));
        searchbar->setMinimumWidth(200);
        searchbar->setMaximumWidth(600);

        horizontalLayout->addWidget(searchbar);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        columns = new QComboBox(toolbar);
        columns->setObjectName("columns");

        QSizePolicy sizePolicy1(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(columns->sizePolicy().hasHeightForWidth());
        columns->setSizePolicy(sizePolicy1);
        columns->setMinimumSize(QSize(100, 0));
        columns->setMinimumWidth(150);
        columns->setMaximumWidth(200);
        // columns->view()->move(QPoint(0, 0));

        horizontalLayout->addWidget(columns);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        sort = new QComboBox(toolbar);
        sort->setObjectName("sort");
        sizePolicy1.setHeightForWidth(sort->sizePolicy().hasHeightForWidth());
        sort->setSizePolicy(sizePolicy1);
        sort->setMinimumSize(QSize(100, 0));
        sort->setMinimumWidth(150);
        sort->setMaximumWidth(200);

        horizontalLayout->addWidget(sort);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        add = new QPushButton(toolbar);
        add->setObjectName("add");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(add->sizePolicy().hasHeightForWidth());
        add->setSizePolicy(sizePolicy2);
        add->setMinimumSize(QSize(40, 40));
        add->setMaximumSize(QSize(40, 40));

        horizontalLayout->addWidget(add);

        horizontalSpacer_4 = new QSpacerItem(30, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        deletec = new QPushButton(toolbar);
        deletec->setObjectName("deletec");
        sizePolicy2.setHeightForWidth(deletec->sizePolicy().hasHeightForWidth());
        deletec->setSizePolicy(sizePolicy2);
        deletec->setMinimumSize(QSize(40, 40));
        deletec->setMaximumSize(QSize(40, 40));
        
        // Setzen der Icons aus dem .qrc
        add->setIcon(QIcon(":/icons/add.png"));
        // set active trashcan icon
        QIcon trashcanIcon;
        trashcanIcon.addFile(QStringLiteral(":/icons/trashcan_closed.png"), QSize(), QIcon::Normal, QIcon::Off);
        trashcanIcon.addFile(QStringLiteral(":/icons/trashcan_open.png"), QSize(), QIcon::Active, QIcon::On);
        deletec->setIcon(trashcanIcon);

        QGraphicsDropShadowEffect *deleteEffect = new QGraphicsDropShadowEffect();
        deleteEffect->setBlurRadius(15);
        deleteEffect->setColor(QColor(255, 0, 0, 180)); // Rot mit Alpha
        deleteEffect->setOffset(0, 0);
        deleteEffect->setEnabled(false); // Initial deaktiviert
        deletec->setGraphicsEffect(deleteEffect);
        
        QObject::connect(deletec, &QPushButton::pressed, [=]() {
            deletec->setIcon(QIcon(":/icons/trashcan_open.png"));
            deleteEffect->setEnabled(true); // Glow aktivieren
        });

        QObject::connect(deletec, &QPushButton::released, [=]() {
            QTimer::singleShot(150, [=]() {
                deletec->setIcon(QIcon(":/icons/trashcan_closed.png"));
                deleteEffect->setEnabled(false); // Glow deaktivieren
            });
        });

        horizontalLayout->addWidget(deletec);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(toolbar);

        QMetaObject::connectSlotsByName(toolbar);
    } // setupUi

    void retranslateUi(QWidget *toolbar)
    {
        toolbar->setWindowTitle(QCoreApplication::translate("toolbar", "Form", nullptr));
        add->setText(QCoreApplication::translate("toolbar", "", nullptr));
        deletec->setText(QCoreApplication::translate("toolbar", "", nullptr));
    } // retranslateUi

    // get columns
    QVector<QString> getColumns() const {
        QVector<QString> columns;
        for (int i = 0; i < this->columns->count(); i++) {
            columns.push_back(this->columns->itemText(i));
        }
        return columns;
    }
    // get checked columns
    QVector<QString> getCheckedColumns() const {
        QVector<QString> checkedColumns;
        for (int i = 0; i < this->columns->count(); i++) {
            if (this->columns->itemData(i, Qt::CheckStateRole).toBool()) {
                checkedColumns.push_back(this->columns->itemText(i));
            }
        }
        return checkedColumns;
    }
    // set columns
    void setColumns(const QStringList& columns, const QStringList& checkedColumns) {
        this->columns->clear();
        for (const auto& column : columns) {
            this->columns->addItem(column);

            int idx = this->columns->count() - 1;
            
            if (checkedColumns.contains(column)) {
                this->columns->setItemData(idx, Qt::Checked, Qt::CheckStateRole);
            } else {
                this->columns->setItemData(idx, Qt::Unchecked, Qt::CheckStateRole);
            }
        }
    }

    void setColumns(std::variant<PersonTableModel*,
                                       LibItemTableModel*,
                                       TransactionTableModel*>& model)
    {
        // std::visit uses a lambda expression that is called for each possible type of the variant
        std::visit([this](auto* m) {
            // Hier rufen wir die andere setColumns-Methode auf, die mit QStringList arbeitet
            setColumns(QStringList::fromVector(m->getAllColumnNames().values()),
                       m->getDisplayedColumns());
        }, model);
    }

    void setSortColumns(const QStringList& columns) {
        this->sort->clear();
        for (const auto& column : columns) {
            this->sort->addItem(column);
        }
    }

    void setSortColumns(std::variant<PersonTableModel*,
                                       LibItemTableModel*,
                                       TransactionTableModel*>& model)
    {
        // std::visit uses a lambda expression that is called for each possible type of the variant
        std::visit([this](auto* m) {
            // Hier rufen wir die andere setColumns-Methode auf, die mit QStringList arbeitet
            setSortColumns(QStringList::fromVector(m->getAllColumnNames().values()));
        }, model);
    }


};

namespace Ui {
    class toolbar: public Ui_toolbar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // TOOLBAR_H
