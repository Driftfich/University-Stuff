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

#include "delegate.h"
#include "infopanel.h"

QWidget* InfoPanelDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // no editor for the key column
    if (index.column() == 0)
        return nullptr;

    bool isReadOnly = index.data(SchemaReadonlyRole).toBool();
    if(isReadOnly) {
        return nullptr; // No editor for read-only items
    }

    // read out item attributes
    QString itemType = index.data(SchemaTypeRole).toString();
    QString itemFormat = index.data(SchemaFormatRole).toString();
    QVariant enumValuesVar = index.data(SchemaEnumValuesRole);

    QVariant minValueVar = index.data(SchemaMinRole);
    QVariant maxValueVar = index.data(SchemaMaxRole);

    // create the correct editor for the item type
    if (itemType == "integer") {
        QSpinBox* editor = new QSpinBox(parent);
        editor->setFrame(false);
        if (minValueVar.isValid() && !minValueVar.isNull()) {
            editor->setMinimum(minValueVar.toInt());
        }
        if (maxValueVar.isValid() && !maxValueVar.isNull()) {
            editor->setMaximum(maxValueVar.toInt());
        }
        return editor;
    }
    else if (itemType == "string" && (itemFormat.contains("date") || itemFormat.contains("time"))) {
        QDateEdit* editor = new QDateEdit(parent);
        editor->setFrame(false);
        editor->setCalendarPopup(true);
        if (itemFormat == "datetime") {
            editor->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
        }
        else if (itemFormat == "date") {
            editor->setDisplayFormat("yyyy-MM-dd");
        }
        else if (itemFormat == "time") {
            editor->setDisplayFormat("HH:mm:ss");
        }
        return editor;
    }
    else if (itemType == "boolean") {
        QComboBox* editor = new QComboBox(parent);
        editor->addItem(tr("true"), true);
        editor->addItem(tr("false"), false);
        editor->setFrame(false);
        return editor;
    }
    else if (itemType == "string" && enumValuesVar.canConvert<QStringList>() && !enumValuesVar.toStringList().isEmpty()) {

        QStringList enumValues = enumValuesVar.toStringList();
        if (!enumValues.isEmpty()) {
            QComboBox* editor = new QComboBox(parent);
            editor->addItems(enumValues);
            editor->setFrame(false);
            return editor;
        }
    }
    
    // fallback for other item types
    QLineEdit* editor = new QLineEdit(parent);
    editor->setFrame(false);
    // use the current text from the model as initial value
    QString initialText = index.model()->data(index, Qt::EditRole).toString();
    editor->setText(initialText);
    return editor;
}

void InfoPanelDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    // read out item attributes
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QString itemType = index.data(SchemaTypeRole).toString();
    QString itemFormat = index.data(SchemaFormatRole).toString();
    QVariant enumValuesVar = index.data(SchemaEnumValuesRole);

    // set the editor data for the item type
    if (itemType == "integer") {
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
        if (spinBox) spinBox->setValue(value.toInt());
    } else if (itemType == "string" && (itemFormat.contains("date") || itemFormat.contains("time"))) {
        QDateEdit* dateEdit = qobject_cast<QDateEdit*>(editor);
        if (dateEdit) {
            if (itemFormat == "datetime") {
                dateEdit->setDateTime(QDateTime::fromString(value, "yyyy-MM-dd HH:mm:ss"));
            } else if (itemFormat == "date") {
                dateEdit->setDate(QDate::fromString(value, "yyyy-MM-dd"));
            } else if (itemFormat == "time") {
                dateEdit->setTime(QTime::fromString(value, "HH:mm:ss"));
            }
        }
    } else if (itemType == "boolean") {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        if (comboBox) {
            if (value.toLower() == "true") comboBox->setCurrentIndex(0);
            else comboBox->setCurrentIndex(1);
        }
    } else if (itemType == "string" && index.data(SchemaEnumValuesRole).canConvert<QStringList>() && !enumValuesVar.toStringList().isEmpty()) {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        if (comboBox) {
            int enumIdx = comboBox->findText(value);
            if (enumIdx != -1) comboBox->setCurrentIndex(enumIdx);
        }
    }
    else {
        // fallback for other item types
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        if (lineEdit) {
            lineEdit->setText(value);
        }
    }
}


void InfoPanelDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    // read out item attributes
    QString itemType = index.data(SchemaTypeRole).toString();
    QString itemFormat = index.data(SchemaFormatRole).toString();
    QVariant enumValuesVar = index.data(SchemaEnumValuesRole);
    QString newValue;

    // set the model data for the item type
    if (itemType == "integer") {
        QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
        newValue = QString::number(spinBox->value());
        if (spinBox) model->setData(index, newValue, Qt::EditRole);
    } 
    else if (itemType == "string" && (itemFormat.contains("date") || itemFormat.contains("time"))) {
        QDateEdit* dateEdit = qobject_cast<QDateEdit*>(editor);
        if (dateEdit) {
            if (itemFormat == "datetime") {
                newValue = dateEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
                model->setData(index, newValue, Qt::EditRole);
            } else if (itemFormat == "date") {
                newValue = dateEdit->date().toString("yyyy-MM-dd");
                model->setData(index, newValue, Qt::EditRole);
            } else if (itemFormat == "time") {
                newValue = dateEdit->time().toString("HH:mm:ss");
                model->setData(index, newValue, Qt::EditRole);
            }
        }
    } else if (itemType == "boolean") {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        if (comboBox) {
            newValue = comboBox->currentData().toBool() ? "true" : "false";
            model->setData(index, comboBox->currentData().toBool(), Qt::EditRole);
        }
    } else if (itemType == "string" && index.data(SchemaEnumValuesRole).canConvert<QStringList>() && !enumValuesVar.toStringList().isEmpty()) {
        QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
        if (comboBox) {
            newValue = comboBox->currentText();
            model->setData(index, newValue, Qt::EditRole);
        }
    } else {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
        if (lineEdit) {
            newValue = lineEdit->text();
            model->setData(index, newValue, Qt::EditRole);
        }
    }

    if (infoPanelPtr) {
        infoPanelPtr->updateFieldValidationState(index);
    }
}

void InfoPanelDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyleOptionViewItem modifiedOption = option;

    // check if the field is invalid
    if (index.column() == 1 && infoPanelPtr) {
        bool isInvalid = infoPanelPtr->isFieldInvalid(index);
        if (isInvalid) {
            // draw the red background first
            painter->save();
            painter->fillRect(option.rect, QColor(255, 200, 200)); // Light red background
            painter->restore();
        }
    }

    // paint the item
    QStyledItemDelegate::paint(painter, modifiedOption, index);

    // Draw red border after painting the item for invalid fields
    if (index.column() == 1 && infoPanelPtr) {
        bool isInvalid = infoPanelPtr->isFieldInvalid(index);
        if (isInvalid) {
            painter->save();
            painter->setPen(QPen(QColor(255, 0, 0), 2));
            painter->drawRect(option.rect.adjusted(1, 1, -1, -1));
            painter->restore();
        }
    }

    // paint the delete item button
    if (infoPanelPtr && index.column() == 1) {
        infoPanelPtr->paintDeleteItemButton(painter, option, index);
    }
}