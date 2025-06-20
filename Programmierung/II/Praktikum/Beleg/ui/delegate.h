/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: 
The InfoPanelDelegate class is responsible for creating the correct editor for the item type.
It also handles the validation of the fields and the painting of the item.
*/

#include <QSpinBox>
#include <QDateEdit>
#include <QComboBox>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>

#include "infopanel.h"

class InfoPanelDelegate : public QStyledItemDelegate {
    Q_OBJECT

    public:
        InfoPanelDelegate(InfoPanel* panel, QObject* parent = nullptr): QStyledItemDelegate(parent), infoPanelPtr(panel)
        {}

        using QStyledItemDelegate::QStyledItemDelegate;
        // create the correct editor for the item type
        QWidget* createEditor(QWidget* parent,
                            const QStyleOptionViewItem& option,
                            const QModelIndex& index) const override;


        // set the editor data
        void setEditorData(QWidget* editor, const QModelIndex& index) const override;

        // set the model data
        void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

        // Custom paint function to highlight invalid fields
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    private:
        InfoPanel* infoPanelPtr; // pointer to the InfoPanel instance
};