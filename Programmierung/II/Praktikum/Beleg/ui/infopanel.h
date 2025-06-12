#ifndef _INFOPANEL_H
#define _INFOPANEL_H

#include <QTreeWidget>
#include <QWidget>
#include <QJsonObject>
#include <QPushButton>
#include <QCheckBox>
#include <QMap>

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

class InfoPanel : public QWidget {
    Q_OBJECT

    public:
        explicit InfoPanel(QWidget *parent = nullptr);
        ~InfoPanel() override;

        bool isFieldValid(const QModelIndex& index) const;
        bool isFieldInvalid(const QModelIndex& index) const { return !isFieldValid(index); }
        void updateFieldValidationState(const QModelIndex& index);
        QJsonObject collectDataFromTree();
        QJsonObject getOriginalData() const { return originalData; }
        QJsonObject getCurrentSchema() const { return currentSchema; }
        void setOriginalData(const QJsonObject& data) {
            qDebug() << "Setting original data in InfoPanel";
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
        void displayInfo(const QJsonObject& jsonObject, bool resetEditMode);
        void displayInfo(const QJsonObject& jsonObject, const QJsonObject& schemaObject, bool resetEditMode);
        // void addJsonToTree(const QJsonValue& value, QTreeWidgetItem* parent);
        void enterEditMode();
        void saveChanges();
        void cancelEditMode();
        void resetButtons();

        void paintDeleteItemButton(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index);

        // void setFieldValidationState(const QModelIndex& index, bool isValid);
    signals:
        // Signal wird ausgelöst, wenn der User Änderungen speichern will
        void saveRequested(const QJsonObject& modifiedData);
        // Signal wird ausgelöst, wenn Cancel-Button im Edit-Modus gedrückt wird
        void editModeCancelled();
        // Signal wird ausgelöst, wenn ein beliebiges Feld geändert wird
        void fieldChanged(QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue);

    private:
        QTreeWidget *treeWidget;
        QPushButton *saveButton;
        QPushButton *editButton;
        QJsonObject currentSchema;

        QJsonObject originalData; // original data for quit edit mode without saving
        QJsonObject originalSchema;
        bool inEditMode;

        // Tree collapse/expand state management
        bool m_treeExpandedState; ///< Tracks whether tree is currently expanded (true) or collapsed (false)

        // for delete button at item level
        QPixmap deleteIcon;
        QPersistentModelIndex hoveredItemForDelete;

        // Font configuration
        int m_baseFontSize;
        double m_fontScaleFactor;
        int m_minFontSize;
        int m_maxFontSize;

        // Optional field management
        QMap<QTreeWidgetItem*, QCheckBox*> optionalCheckboxes;
        QMap<QTreeWidgetItem*, bool> optionalFieldStates;

        void setTreeItemsEditable(bool editable);
        void setTreeItemEditable(QTreeWidgetItem* item, bool editable);
        QJsonValue getValueFromItem(QTreeWidgetItem* item);
        void restoreOriginalData();

        void updateAddButtons(bool enable);
        void onAddArrayItem();
        void onAddObjectItem();

        bool isItemDeletable(QTreeWidgetItem* item) const;
        QRect getDeleteButtonRectForItem(const QTreeWidgetItem* item, const QStyleOptionViewItem& option) const;

        void addJsonToTreeRecursive(const QJsonValue& valueForThisItem, QTreeWidgetItem* thisItem, int depth = 0);
        void addJsonToTreeRecursive(const QJsonValue& valueForThisItem, QTreeWidgetItem* thisItem, int depth, const QJsonObject& currentItemSchema);
        bool isLowestCollection(QTreeWidgetItem* item) const;
        bool isHighestItem(QTreeWidgetItem* item) const;

        QFont calculateFontForDepth(int depth) const;
        int getItemDepth(QTreeWidgetItem* item) const;

        QSet<QPersistentModelIndex> invalidRequiredFields;
        void updateSaveButtonState();
        void validateAllRequiredFieldsOnLoad();
        void validateRequiredFieldsRecursive(QTreeWidgetItem* item);
        bool validateRequiredField(const QTreeWidgetItem* item) const;
        bool validateFieldPattern(const QTreeWidgetItem* item) const;
        // bool JsonChanged();

        // Optional field management
        void createOptionalCheckbox(QTreeWidgetItem* item);
        void onOptionalCheckboxToggled(bool checked);
        void updateOptionalFieldVisibility(QTreeWidgetItem* item, bool visible, int depth = 0);
        void setOptionalFieldsVisibility();
        bool isOptionalFieldEnabled(QTreeWidgetItem* item) const;
        void clearOptionalFieldComponentsSafely();

    private slots:
        void onItemChanged(QTreeWidgetItem* item, int column);
        void handleDeleteAction(QTreeWidgetItem* item);
        void onHeaderSectionClicked(int logicalIndex); ///< Handles clicks on header sections, specifically column 0 for collapse/expand all

    protected:
        bool eventFilter(QObject* watched, QEvent *event) override;
};

#endif