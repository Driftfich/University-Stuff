/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: 
The InfoPanel class is responsible for displaying the information of a json object in a tree widget.
Furthermore it allows the user to specify the schema of the json object.
The schema enables to define the type (string, number, boolean, object, array), format (date, time, datetime, enum), minimum and maximum values,
pattern of the string, required and optional fields and the default value.

*/

#ifndef _INFOPANEL_H
#define _INFOPANEL_H

#include <QTreeWidget>
#include <QWidget>
#include <QJsonObject>
#include <QPushButton>
#include <QCheckBox>
#include <QMap>

// item attribute roles which can be set
const int SchemaTypeRole = Qt::UserRole + 1;
const int SchemaFormatRole = Qt::UserRole + 2;
const int SchemaEnumValuesRole = Qt::UserRole + 3;
const int SchemaReadonlyRole = Qt::UserRole + 4;
const int SchemaRequiredRole = Qt::UserRole + 5;
const int SchemaPatternRole = Qt::UserRole + 6;
const int SchemaMinRole = Qt::UserRole + 7;
const int SchemaMaxRole = Qt::UserRole + 8;
const int SchemaOriginalKeyRole = Qt::UserRole + 9;
const int SchemaOptionalRole = Qt::UserRole + 10;
const int SchemaPlaceholderRole = Qt::UserRole + 11;

class InfoPanel : public QWidget {
    Q_OBJECT

    public:
        explicit InfoPanel(QWidget *parent = nullptr);
        ~InfoPanel() override;

        // check if a field is valid or invalid
        bool isFieldValid(const QModelIndex& index) const;
        bool isFieldInvalid(const QModelIndex& index) const { return !isFieldValid(index); }

        // update the validation state of a field
        void updateFieldValidationState(const QModelIndex& index);

        // collect the data from the tree widget and build the json object
        QJsonObject collectDataFromTree();

        // get original data, schema and current schema and set them
        QJsonObject getOriginalData() const { return originalData; }
        QJsonObject getCurrentSchema() const { return currentSchema; }
        void setOriginalData(const QJsonObject& data) {
            originalData = data;
        }
        void setCurrentSchema(const QJsonObject& schema) {
            currentSchema = schema;
            validateAllRequiredFieldsOnLoad();
        }
        void setOriginalSchema(const QJsonObject& schema) {
            originalSchema = schema;
        }
        QJsonObject getOriginalSchema() const { return originalSchema; }

    public slots:
        // display the json object in the tree widget
        void displayInfo(const QJsonObject& jsonObject, bool resetEditMode);
        void displayInfo(const QJsonObject& jsonObject, const QJsonObject& schemaObject, bool resetEditMode);

        // enter edit mode to allow the user to edit the json object
        void enterEditMode();
        // save the changes
        void saveChanges();
        // cancel the edit mode
        void cancelEditMode();
        // reset the edit and save buttons
        void resetButtons();

        // paint the delete item button
        void paintDeleteItemButton(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index);

        // setup completer for editor widgets (called by delegate)
        void setupCompleterForEditor(QLineEdit* editor, const QModelIndex& index) {
            // emit the signal to setup the completer for the editor widget
            emit completerSetupRequested(editor, index);
        }

    signals:
        // signal is emitted when the user wants to save the changes
        void saveRequested(const QJsonObject& modifiedData);
        // signal is emitted when the user cancels the edit mode
        void editModeCancelled();
        // signal is emitted when a field is changed
        void fieldChanged(QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue);
        // signal is emitted when trying to setup a completer for an editor widget
        void completerSetupRequested(QLineEdit* editor, const QModelIndex& index);

    private:
        // tree widget to display the json object
        QTreeWidget *treeWidget;
        // save button to save the changes
        QPushButton *saveButton;
        // edit button to enter edit mode
        QPushButton *editButton;
        // current schema
        QJsonObject currentSchema;

        QJsonObject originalData; // original data for quit edit mode without saving
        QJsonObject originalSchema; // original schema for quit edit mode without saving

        bool inEditMode; // true if the user is in edit mode

        // Tree collapse/expand state management
        bool m_treeExpandedState; // true if the tree is currently expanded, false if it is collapsed

        // for delete button at item level
        QPixmap deleteIcon; // icon for the delete button
        QPersistentModelIndex hoveredItemForDelete; // item which is currently hovered for the delete button

        // Font configuration
        int m_baseFontSize; // base font size
        double m_fontScaleFactor; // font scale factor
        int m_minFontSize; // minimum font size
        int m_maxFontSize; // maximum font size

        // Optional field management
        QMap<QTreeWidgetItem*, QCheckBox*> optionalCheckboxes; // map of optional checkboxes
        QMap<QTreeWidgetItem*, bool> optionalFieldStates; // map of optional field states

        // set the tree items editable
        void setTreeItemsEditable(bool editable);
        // set the tree item editable
        void setTreeItemEditable(QTreeWidgetItem* item, bool editable);
        // get the value from the item
        QJsonValue getValueFromItem(QTreeWidgetItem* item);
        // restore the original data
        void restoreOriginalData();

        // update the add buttons
        void updateAddButtons(bool enable);
        void onAddArrayItem();
        void onAddObjectItem();

        // check if the item is deletable
        bool isItemDeletable(QTreeWidgetItem* item) const;
        // get the rectangle for the delete button
        QRect getDeleteButtonRectForItem(const QTreeWidgetItem* item, const QStyleOptionViewItem& option) const;

        // add the json to the tree recursively with/without schema
        void addJsonToTreeRecursive(const QJsonValue& valueForThisItem, QTreeWidgetItem* thisItem, int depth = 0);
        void addJsonToTreeRecursive(const QJsonValue& valueForThisItem, QTreeWidgetItem* thisItem, int depth, const QJsonObject& currentItemSchema);
        
        // check if the item is the lowest collection
        bool isLowestCollection(QTreeWidgetItem* item) const;
        bool isHighestItem(QTreeWidgetItem* item) const;

        // calculate the font for the depth
        QFont calculateFontForDepth(int depth) const;
        // get the depth of the item
        int getItemDepth(QTreeWidgetItem* item) const;

        QSet<QPersistentModelIndex> invalidRequiredFields; // set of invalid required fields
        // update the save button state
        void updateSaveButtonState();
        // validate all required fields on load
        void validateAllRequiredFieldsOnLoad();
        // validate the required fields recursively
        void validateRequiredFieldsRecursive(QTreeWidgetItem* item);
        // validate the required field
        bool validateRequiredField(const QTreeWidgetItem* item) const;
        // validate the field pattern
        bool validateFieldPattern(const QTreeWidgetItem* item) const;

        // create the optional checkbox
        void createOptionalCheckbox(QTreeWidgetItem* item, bool checked = false);
        // handle the toggling of the optional checkbox
        void onOptionalCheckboxToggled(bool checked);
        // update the visibility of the optional fields
        void updateOptionalFieldVisibility(QTreeWidgetItem* item, bool visible, int depth = 0);
        // set the visibility of the optional fields
        void setOptionalFieldsVisibility();
        // check if the optional field is enabled
        bool isOptionalFieldEnabled(QTreeWidgetItem* item) const;
        // clear the optional field components safely
        void clearOptionalFieldComponentsSafely();

    private slots:
        // handle the change of an item
        void onItemChanged(QTreeWidgetItem* item, int column);
        // handle the delete action when the delete button of an item is clicked
        void handleDeleteAction(QTreeWidgetItem* item);
        // handle the click on the header section to collapse/expand the tree
        void onHeaderSectionClicked(int logicalIndex);

    protected:
        bool eventFilter(QObject* watched, QEvent *event) override;
};

#endif