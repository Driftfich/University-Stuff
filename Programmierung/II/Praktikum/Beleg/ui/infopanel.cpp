#include <QWidget>
#include <QTreeWidget>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QIcon>
#include <QPushButton>
#include <QMessageBox>
#include <QToolButton>
#include <QInputDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>

#include <QEventLoop>
#include <QCoreApplication>

#include <QEvent>
#include "infopanel.h"
#include "delegate.h"

InfoPanel::InfoPanel(QWidget *parent) : QWidget(parent), treeWidget(new QTreeWidget(this)), inEditMode(false), m_treeExpandedState(true),
    m_baseFontSize(15), m_fontScaleFactor(0.9), m_minFontSize(7), m_maxFontSize(15) // Initialize font params
{
    // set the header labels
    treeWidget->setHeaderLabels({tr("Attribute"), tr("Value")});
    // set the overall layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Create horizontal layout for buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    // set icon of the save button
    saveButton = new QPushButton(tr("Save"), this);
    saveButton->setIcon(QIcon(":/icons/save.png"));
    saveButton->setVisible(true);
    saveButton->setEnabled(false);
    saveButton->setStyleSheet("QPushButton { background-color: lightgray; }");
    buttonLayout->addWidget(saveButton);

    // set the icon of the edit button
    editButton = new QPushButton(tr("Edit"), this);
    editButton->setIcon(QIcon(":/icons/edit.png"));
    editButton->setVisible(true);
    buttonLayout->addWidget(editButton);

    // add the button layout first, then the tree widget
    layout->addLayout(buttonLayout);
    layout->addWidget(treeWidget);

    // set the custom delegate for the tree widget
    treeWidget->setItemDelegate(new InfoPanelDelegate(this, treeWidget)); 

    // connect the signals and slots
    connect(saveButton, &QPushButton::clicked, this, &InfoPanel::saveChanges);
    connect(editButton, &QPushButton::clicked, this, &InfoPanel::enterEditMode);
    connect(treeWidget, &QTreeWidget::itemChanged, this, &InfoPanel::onItemChanged);
    
    // Enable header clicking and connect header click signal for collapse/expand functionality
    treeWidget->header()->setSectionsClickable(true);
    treeWidget->header()->setDefaultSectionSize(100);
    connect(treeWidget->header(), &QHeaderView::sectionClicked, this, &InfoPanel::onHeaderSectionClicked);

    // load the delete icon
    deleteIcon = QPixmap(":/icons/quit.png");
    if (deleteIcon.isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("The symbol for the delete button could not be loaded."));
    }

    // install the event filter to handle mouse events
    treeWidget->viewport()->installEventFilter(this);
    // enable mouse tracking for the viewport
    treeWidget->viewport()->setMouseTracking(true);
}

InfoPanel::~InfoPanel() {
    // clean up the optional checkboxes
    for (QMap<QTreeWidgetItem*, QCheckBox*>::const_iterator it = optionalCheckboxes.constBegin(); it != optionalCheckboxes.constEnd(); ++it) {
        delete it.value();
    }
    optionalCheckboxes.clear();
    optionalFieldStates.clear();
    
    delete treeWidget;
}

/**
 * @brief Display the json object in the tree widget without a schema.
 * @param jsonObject The json object to display.
 * @param resetEditMode True if the edit mode should be reset.
*/
void InfoPanel::displayInfo(const QJsonObject& jsonObject, bool resetEditMode) {
    
    if (resetEditMode) {
        inEditMode = false;
        resetButtons();
    }

    // Store new data and schema
    setOriginalData(jsonObject);
    setOriginalSchema(QJsonObject());
    setCurrentSchema(QJsonObject()); // no schema provided in this overload, so set to empty object

    // safe cleanup of optional field components to prevent paint engine errors
    clearOptionalFieldComponentsSafely();
    
    // clear the validation state
    invalidRequiredFields.clear();
    
    // Block all tree widget signals during reconstruction
    QSignalBlocker treeBlocker(treeWidget);
    QSignalBlocker viewportBlocker(treeWidget->viewport());
    
    // clear the tree and reset the hover state
    treeWidget->clear();
    if (hoveredItemForDelete.isValid()) {
        treeWidget->update(hoveredItemForDelete);
    }
    hoveredItemForDelete = QPersistentModelIndex();
    
    // reset the edit and save buttons if the edit mode should be reset
    if (resetEditMode) {
        resetButtons();
    }

    // reconstruct the tree with new data
    addJsonToTreeRecursive(jsonObject, treeWidget->invisibleRootItem(), 0);

    // expand all items in the tree
    treeWidget->expandAll();
    // set the tree collapsed
    m_treeExpandedState = true;
    // resize the columns to the contents
    treeWidget->resizeColumnToContents(0);
    treeWidget->resizeColumnToContents(1);
    // set the tree items editable and update the add buttons
    setTreeItemsEditable(inEditMode);
    updateAddButtons(inEditMode);

    // Apply optional field visibility settings
    setOptionalFieldsVisibility();
}

/**
 * @brief Display the json object in the tree widget with a schema.
 * @param jsonObject The json object to display.
 * @param schemaObject The schema to display.
 * @param resetEditMode True if the edit mode should be reset.
*/
void InfoPanel::displayInfo(const QJsonObject& jsonObject, const QJsonObject& schemaObject, bool resetEditMode) {
    if (resetEditMode) {
        inEditMode = false;
        resetButtons();
    }

    // store the new data and schema
    setOriginalData(jsonObject);
    setOriginalSchema(schemaObject);
    setCurrentSchema(schemaObject);

    // safe cleanup of optional field components to prevent paint engine errors
    clearOptionalFieldComponentsSafely();
    
    // clear the validation state
    invalidRequiredFields.clear();
    
    // Block all tree widget signals during reconstruction
    QSignalBlocker treeBlocker(treeWidget);
    QSignalBlocker viewportBlocker(treeWidget->viewport());
    
    // clear the tree and reset the hover state
    treeWidget->clear();
    if (hoveredItemForDelete.isValid()) {
        treeWidget->update(hoveredItemForDelete);
    }
    hoveredItemForDelete = QPersistentModelIndex();
    
    if (resetEditMode) {
        resetButtons();
    }

    // reconstruct the tree with new data
    addJsonToTreeRecursive(jsonObject, treeWidget->invisibleRootItem(), 0, currentSchema);

    treeWidget->expandAll();
    m_treeExpandedState = true;
    treeWidget->resizeColumnToContents(0);
    treeWidget->resizeColumnToContents(1);
    setTreeItemsEditable(inEditMode);
    updateAddButtons(inEditMode);

    // Apply optional field visibility settings
    setOptionalFieldsVisibility();
}

/**
 * @brief Add the json to the tree recursively with schema.
 * @param valueForThisItem The json value to add
 * @param thisItem The item to add the json to
 * @param depth The depth/level of the item
 * @param currentItemSchema The schema of the item
*/
void InfoPanel::addJsonToTreeRecursive(const QJsonValue& valueForThisItem, QTreeWidgetItem* thisItem, int depth, const QJsonObject& currentItemSchema) {
    // calculate the font for the depth
    QFont itemFont = calculateFontForDepth(depth);
    // set the font for the key column of thisItem
    thisItem->setFont(0, itemFont);

    // get the attributes from the schema
    QString nameOverride = currentItemSchema.value("rename").toString();
    QString description = currentItemSchema.value("description").toString();
    bool isReadOnly = currentItemSchema.value("readonly").toBool(false);
    QString originalKey = thisItem->text(0);

    if (!originalKey.isEmpty()) {
        thisItem->setData(0, SchemaOriginalKeyRole, originalKey);
    }

    if (!nameOverride.isEmpty()) {
        // use the rename value from the schema to override the text of the key
        thisItem->setText(0, nameOverride);
    }
    if (!description.isEmpty()) {
        // use the description from the schema to set the tooltip of the key
        thisItem->setToolTip(0, description);
    }

    // create the checkbox if optional is in the schema. The value of optional is used to set the initial state of the checkbox
    if (currentItemSchema.contains("optional")) {
        createOptionalCheckbox(thisItem, currentItemSchema.value("optional").toBool(false));
    }

    // if the value is an object, add the children to the tree
    if (valueForThisItem.isObject()) {
        thisItem->setData(0, Qt::UserRole, "object"); // thisItem represents an object.
        QJsonObject obj = valueForThisItem.toObject();

        QJsonObject propertiesSchema;
        if (currentItemSchema.value("type").toString() == "object" && currentItemSchema.contains("properties")) {
            propertiesSchema = currentItemSchema.value("properties").toObject();
        }
        // qDebug() << "Current item schema for object:" << currentItemSchema << "\n";
        // qDebug() << "propertiesSchema from" << thisItem->text(0) << "->" << propertiesSchema << "\n";
        thisItem->setData(0, SchemaReadonlyRole, isReadOnly);

        // add the children keys to the tree
        for (const QString& key : obj.keys()) {
            QTreeWidgetItem* child = new QTreeWidgetItem(thisItem);
            child->setText(0, key);
            // child->setData(0, SchemaReadonlyRole, isReadOnly);
            QJsonObject childSchema = propertiesSchema.value(key).toObject();
            // qDebug() << key << "->" << childSchema;
            // if (childSchema.isEmpty() && currentItemSchema.contains("additionalProperties") && currentItemSchema.value("additionalProperties").isObject()) {
            //     childSchema = currentItemSchema.value("additionalProperties").toObject();
            // }
            // call recurive to setup the child item
            addJsonToTreeRecursive(obj[key], child, depth + 1, childSchema);
        }
    } else if (valueForThisItem.isArray()) {
        thisItem->setData(0, Qt::UserRole, "array"); // thisItem represents an array.
        thisItem->setData(0, SchemaReadonlyRole, isReadOnly);
        QJsonArray array = valueForThisItem.toArray();

        QJsonObject itemSchema; // schema for individual items in the array
        if (currentItemSchema.value("type").toString() == "array" && currentItemSchema.contains("items")) {
            itemSchema = currentItemSchema.value("items").toObject();
        }

        // add the items to the tree
        for (int i = 0; i < array.size(); ++i) {
            QTreeWidgetItem* child = new QTreeWidgetItem(thisItem);
            // set the text to [index]
            child->setText(0, QString("[%1]").arg(i));
            // call recursively to setup the child item
            addJsonToTreeRecursive(array[i], child, depth + 1,  itemSchema);
            // set the readonly state for the child item to overwrite afterwards
            child->setData(0, SchemaReadonlyRole, isReadOnly);
        }
    } 
    else { // Leaf node
        thisItem->setText(1, valueForThisItem.toVariant().toString()); // set the value of the item
        thisItem->setData(0, Qt::UserRole, "leaf"); // set the type of the item to leaf
        thisItem->setFont(1, calculateFontForDepth(depth)); // set the font of the value column

        // set the schema type and format for the item
        QString itemType = currentItemSchema.value("type").toString();
        QString itemFormat = currentItemSchema.value("format").toString();
        QStringList enumValues = currentItemSchema.value("enum").toVariant().toStringList();
        // bool isReadOnly = currentItemSchema.value("readonly").toBool(false);
        bool isRequired = currentItemSchema.value("required").toBool(false);
        // qDebug() << "Key" << thisItem->text(0) << " -> Current Schema" << currentItemSchema << " String val: " << currentItemSchema.value("optional") << " isOptional: " << isOptional;
        thisItem->setData(1, SchemaTypeRole, itemType);
        thisItem->setData(1, SchemaFormatRole, itemFormat);
        thisItem->setData(1, SchemaEnumValuesRole, enumValues);
        thisItem->setData(1, SchemaReadonlyRole, isReadOnly);
        thisItem->setData(0, SchemaReadonlyRole, isReadOnly);
        thisItem->setData(1, SchemaRequiredRole, isRequired);
        thisItem->setData(1, SchemaPatternRole, currentItemSchema.value("pattern").toString());
        thisItem->setData(1, SchemaPlaceholderRole, currentItemSchema.value("placeholder").toString());
        
        // thisItem->setData(1, SchemaOptionalRole, isOptional);
        QJsonValue minValue = currentItemSchema.value("minimum");
        if (!minValue.isUndefined() && !minValue.isNull()) {
            long long min = minValue.toVariant().toLongLong();
            thisItem->setData(1, SchemaMinRole, min); // Save minimum value for the item
        }

        QJsonValue maxValue = currentItemSchema.value("maximum");
        if (!maxValue.isUndefined() && !maxValue.isNull()) {
            long long max = maxValue.toVariant().toLongLong();
            thisItem->setData(1, SchemaMaxRole, max); // Save maximum value for the item
        }
        // save current value for the item
        thisItem->setData(1, Qt::UserRole + 30, thisItem->text(1));
    }
}

/**
 * @brief Add the json to the tree recursively without a schema.
 * @param valueForThisItem The json value to add
 * @param thisItem The item to add the json to
 * @param depth The depth/level of the item
*/
void InfoPanel::addJsonToTreeRecursive(const QJsonValue& valueForThisItem, QTreeWidgetItem* thisItem, int depth) {
    // set the font for the key column with the given depth
    QFont itemFont = calculateFontForDepth(depth);
    thisItem->setFont(0, itemFont);

    // if the value is an object, add the children to the tree
    if (valueForThisItem.isObject()) {
        thisItem->setData(0, Qt::UserRole, "object");
        QJsonObject obj = valueForThisItem.toObject();
        for (const QString& key : obj.keys()) {
            QTreeWidgetItem* child = new QTreeWidgetItem(thisItem);
            child->setText(0, key);
            addJsonToTreeRecursive(obj[key], child, depth + 1);
        }
    } 
    // if the value is an array, add the items to the tree
    else if (valueForThisItem.isArray()) {
        thisItem->setData(0, Qt::UserRole, "array");
        QJsonArray array = valueForThisItem.toArray();
        for (int i = 0; i < array.size(); ++i) {
            QTreeWidgetItem* child = new QTreeWidgetItem(thisItem);
            child->setText(0, QString("[%1]").arg(i));
            addJsonToTreeRecursive(array[i], child, depth + 1);
        }
    } 
    // if the value is a leaf node, add the value to the tree
    else { 
        // set the text of the value column
        thisItem->setText(1, valueForThisItem.toVariant().toString());
        // set the type of the item to leaf
        thisItem->setData(0, Qt::UserRole, "leaf");
        // set the font of the value column
        thisItem->setFont(1, calculateFontForDepth(depth));
        // save current value for the item
        thisItem->setData(1, Qt::UserRole + 30, thisItem->text(1));
    }
}

/**
 * @brief Check if the item is at the lowest collection level of the tree (lowest object or array)
 * @param item The item to check
 * @return True if the item is a lowest collection, false otherwise
*/
bool InfoPanel::isLowestCollection(QTreeWidgetItem* item) const {
    if (!item) return false;
    QString itemType = item->data(0, Qt::UserRole).toString();

    if (itemType != "array" && itemType != "object") {
        return false; // Not a collection type
    }

    if (item->childCount() == 0) {
        return true; // Empty array/object is a "lowest" collection
    }

    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem* child = item->child(i);
        if (child) {
            QString childType = child->data(0, Qt::UserRole).toString();
            if (childType == "array" || childType == "object") {
                return false; // Contains another collection, so not the lowest
            }
        }
    }
    return true; // All children are leaves (or it was empty)
}

/**
 * @brief Check if the item is at the highest level of the tree (top-level item)
 * @param item The item to check
 * @return True if the item is at the highest level, false otherwise
*/
bool InfoPanel::isHighestItem(QTreeWidgetItem* item) const {
    if (!item || !item->parent()) {
        return true;
    }
    // A "highest item" (top-level item) is a direct child of the tree widget's invisible root item.
    return item->parent() == treeWidget->invisibleRootItem();
    
}

/**
 * @brief Enters edit mode with comprehensive UI state reset
 * 
 * Ensures all UI components are properly reset and configured for editing:
 * - Resets hover states and delete button interactions
 * - Clears any stale validation states
 * - Properly manages optional field checkboxes and visibility
 * - Uses signal blocking to prevent race conditions during state transitions
 * - Validates all required fields and updates button states
 * 
 * @exception None
 * @purpose Provides safe transition to edit mode with complete UI reset
 */
void InfoPanel::enterEditMode() {
    // qDebug() << "Entering edit mode with comprehensive reset";
    
    // Block signals during state transition to prevent race conditions
    QSignalBlocker treeBlocker(treeWidget);
    QSignalBlocker viewportBlocker(treeWidget->viewport());
    
    // Reset hover state for delete buttons to prevent stale UI interactions
    if (hoveredItemForDelete.isValid()) {
        treeWidget->update(hoveredItemForDelete);
        hoveredItemForDelete = QPersistentModelIndex();
    }
    
    // Clear any stale validation state from previous edit sessions
    invalidRequiredFields.clear();
    
    // Set edit mode state
    inEditMode = true;
    
    // make the tree widget editable
    setTreeItemsEditable(true);

    // Apply optional field visibility settings with proper state management
    setOptionalFieldsVisibility();

    // Re-enable signal processing after state setup is complete
    treeBlocker.unblock();
    viewportBlocker.unblock();
    
    // Update save button state based on current data validity
    updateSaveButtonState();
    
    // change the state of the save button
    saveButton->setEnabled(true);
    saveButton->setStyleSheet("QPushButton { background-color: lightblue; }");
    
    // change the edit button to cancel button
    editButton->setText(tr("Cancel"));
    editButton->setIcon(QIcon(":/icons/quit.png"));

    // disconnect the edit button from the enterEditMode signal and connect it to the cancelEditMode signal
    disconnect(editButton, &QPushButton::clicked, this, &InfoPanel::enterEditMode);
    connect(editButton, &QPushButton::clicked, this, &InfoPanel::cancelEditMode);

    // Enable add buttons for dynamic content modification
    updateAddButtons(true);
    
    // Validate all required fields to ensure proper initial state
    validateAllRequiredFieldsOnLoad();
    
    // qDebug() << "Edit mode entered successfully with complete UI reset";
}

/**
 * @brief Saves changes with comprehensive UI state cleanup
 * 
 * Ensures complete UI reset after saving changes:
 * - Validates all required fields before saving
 * - Clears validation state and optional field components
 * - Resets all button states and hover interactions
 * - Applies proper optional field visibility
 * - Uses proper signal management for state transitions
 * 
 * @exception None - Shows warning for validation errors
 * @purpose Provides safe save operation with complete UI cleanup
 */
void InfoPanel::saveChanges() {
    if (!inEditMode) return;

    if (!invalidRequiredFields.isEmpty()) {
        QMessageBox::warning(this, tr("Validation error"), 
                           tr("Please fill in all required fields."));
        return;
    }
    
    // qDebug() << "Saving changes with comprehensive UI cleanup";
    
    // collect the data from the tree
    QJsonObject modifiedData = collectDataFromTree();

    // update the original data with the modified data
    setOriginalData(modifiedData);
    setOriginalSchema(currentSchema);
    
    // Clear validation state to prevent stale error indicators
    invalidRequiredFields.clear();
    
    // Safe cleanup of optional field components to prevent memory leaks
    clearOptionalFieldComponentsSafely();
    
    // exit edit mode
    inEditMode = false;
    setTreeItemsEditable(false);
    
    // Apply optional field visibility settings for view mode
    setOptionalFieldsVisibility();
    
    // reset the state of the save button
    saveButton->setEnabled(false);
    saveButton->setStyleSheet("QPushButton { background-color: lightgray; }");
    editButton->setText(tr("Edit"));
    editButton->setIcon(QIcon(":/icons/edit.png"));
    
    // reset the signal connections of the edit button
    disconnect(editButton, &QPushButton::clicked, this, &InfoPanel::cancelEditMode);
    connect(editButton, &QPushButton::clicked, this, &InfoPanel::enterEditMode);
    
    // Hide add buttons and reset hover state
    updateAddButtons(false); // Hide add buttons after saving
    if (hoveredItemForDelete.isValid()) {
        // If a delete button was hovered it will be reset
        treeWidget->update(hoveredItemForDelete);
        hoveredItemForDelete = QPersistentModelIndex(); // Reset the hovered item for delete button
    }

    // emit the saveRequested signal
    emit saveRequested(modifiedData);
    
    // qDebug() << "Changes saved successfully with complete UI cleanup";
}


/**
 * @brief Cancels edit mode with comprehensive UI state cleanup
 * 
 * Ensures complete UI reset after canceling edit mode:
 * - Resets all button states and hover interactions
 * - Applies proper optional field visibility
 * - Uses proper signal management for state transitions
 * 
 * @exception None - Shows warning for validation errors
 * @purpose Provides safe cancel operation with complete UI cleanup
 */

void InfoPanel::cancelEditMode() {
    // qDebug() << "Canceling edit mode in InfoPanel";
    
    // Clear validation state
    invalidRequiredFields.clear();

    // Safe cleanup before restoring data
    clearOptionalFieldComponentsSafely();
    
    // Restore original data
    restoreOriginalData();
    
    // Exit edit mode
    inEditMode = false;
    setTreeItemsEditable(false);
    
    // Apply optional field visibility settings
    setOptionalFieldsVisibility();
    
    updateAddButtons(false);
    resetButtons();

    // Clear hover state safely
    if (hoveredItemForDelete.isValid()) {
        treeWidget->update(hoveredItemForDelete);
    }
    hoveredItemForDelete = QPersistentModelIndex();

    emit editModeCancelled();
    // qDebug() << "Edit mode canceled successfully";
}

// reset the save and edit button to non edit state
void InfoPanel::resetButtons() {
    // Reset the buttons to their initial state
    saveButton->setEnabled(false);
    saveButton->setStyleSheet("QPushButton { background-color: lightgray; }");
    editButton->setText(tr("Edit"));
    editButton->setIcon(QIcon(":/icons/edit.png"));

    // Signal-Verbindungen zurücksetzen
    disconnect(editButton, &QPushButton::clicked, this, &InfoPanel::cancelEditMode);
    connect(editButton, &QPushButton::clicked, this, &InfoPanel::enterEditMode);
}

// set the tree items editable
void InfoPanel::setTreeItemsEditable(bool editable) {
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
        setTreeItemEditable(treeWidget->topLevelItem(i), editable);
    }
}

// set the tree item editable
void InfoPanel::setTreeItemEditable(QTreeWidgetItem* item, bool editable) {
    if (!item) return;
    
    // make the leaf nodes (values) editable, not keys or group names
    bool isLeaf = item->data(0, Qt::UserRole).toString() == "leaf";
    QString itemKey = item->text(0).toLower();
    
    if (isLeaf && editable) {
        // make the value column editable
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    } else {
        // make the value column not editable
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
    
    // recursively for all children
    for (int i = 0; i < item->childCount(); ++i) {
        setTreeItemEditable(item->child(i), editable);
    }
}

// react when a value in a tree item changes
void InfoPanel::onItemChanged(QTreeWidgetItem* item, int column) {
    if (!inEditMode) return; // Only react to changes if in edit mode
    
    // Extract field information from the item
    QString originalKey = item ? item->data(0, SchemaOriginalKeyRole).toString() : QString();
    QVariant oldValue = item ? item->data(1, Qt::UserRole + 30) : QVariant(); // Store old value in custom role
    QVariant newValue = item ? item->text(1) : QVariant();
    
    // Store new value for next comparison
    if (item) {
        // Block signals on the tree widget to prevent recursion
        treeWidget->blockSignals(true);

        // Update the item with the new value
        item->setData(1, Qt::UserRole + 30, newValue);

        // Unblock signals after updating
        treeWidget->blockSignals(false);
    }
    
    // Emit the field change signal with all necessary information
    emit fieldChanged(item, column, originalKey, oldValue, newValue);
    
    updateSaveButtonState();
}

// collect the data from the tree
QJsonObject InfoPanel::collectDataFromTree() {
    return getValueFromItem(treeWidget->invisibleRootItem()).toObject(); // Collect data from the invisible root item
}

// get the value from the item
QJsonValue InfoPanel::getValueFromItem(QTreeWidgetItem* item) {
    QString itemType = item->data(0, Qt::UserRole).toString();

    if (itemType == "array") {
        QJsonArray array = QJsonArray();
        for (int i = 0; i < item->childCount(); ++i) {
            QTreeWidgetItem* child = item->child(i);
            // Skip disabled optional fields
            if (!isOptionalFieldEnabled(child)) {
                continue;
            }
            array.append(getValueFromItem(child));
        }
        return array;
    } else if (itemType == "object") {
        QJsonObject obj = QJsonObject();
        for (int i = 0; i < item->childCount(); ++i) {
            QTreeWidgetItem* child = item->child(i);
            // Skip disabled optional fields
            if (!isOptionalFieldEnabled(child)) {
                continue;
            }
            QString originalKey = child->data(0, SchemaOriginalKeyRole).toString();
            QString keyToUse = originalKey.isEmpty() ? child->text(0) : originalKey;
            obj[keyToUse] = getValueFromItem(child);
        }
        return obj;
    }
    else if (itemType == "leaf" || item->childCount() == 0) { // Leaf or no children implies direct value
        QString text = item->text(1);
        bool ok;
        int intVal = text.toInt(&ok);
        if (ok) return intVal;
        
        double doubleVal = text.toDouble(&ok);
        if (ok) return doubleVal;
        
        if (text.toLower() == "true") return true;
        if (text.toLower() == "false") return false;
        
        return text;
    }
    return QJsonValue(); // Fallback, should not happen
}

void InfoPanel::restoreOriginalData() {
    // display the original data with the original schema and reset the edit mode
    displayInfo(originalData, originalSchema, true);
}

/**
 * @brief Update the add buttons for the tree
 * @param show True if the add buttons should be shown, false otherwise
 * 
 * @purpose Updates the add buttons visibility in the tree widget
 */
void InfoPanel::updateAddButtons(bool show) {
    // iterate over all items in the tree
    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        QTreeWidgetItem* item = *it;
        
        // When the item is a lowest collection item (doesnt contain a children which can have again a add button) 
        // and not readonly and isnt highest item (top level attributes shouldnt be able to have further attributes)
        QWidget* currentWidget = treeWidget->itemWidget(item, 1);
        QVariant readonlyVariant = item->data(0, SchemaReadonlyRole);
        bool readonly = readonlyVariant.isValid() ? readonlyVariant.toBool() : false;
        bool shouldHaveButton = show && isLowestCollection(item) && !isHighestItem(item) && !readonly;

        // qDebug() << "Adding add button for item:" << item->text(0) << "Readonly:" << readonly;
        if (shouldHaveButton) {
            if (!currentWidget) { // Add button only if one doesn't exist
                QToolButton* addButton = new QToolButton();
                addButton->setIcon(QIcon(":/icons/add2.png")); // Ensure this icon exists
                QString itemType = item->data(0, Qt::UserRole).toString();
                addButton->setToolTip(itemType == "array" ? tr("Add new item to array") : tr("Add new attribute to object"));
                addButton->setAutoRaise(true);
                addButton->setFocusPolicy(Qt::NoFocus);
                addButton->setProperty("treeItem", QVariant::fromValue<void*>(static_cast<void*>(item)));

                if (itemType == "array") {
                    connect(addButton, &QToolButton::clicked, this, &InfoPanel::onAddArrayItem);
                } else { // object
                    connect(addButton, &QToolButton::clicked, this, &InfoPanel::onAddObjectItem);
                }
                treeWidget->setItemWidget(item, 1, addButton);
            }
            // else the buttons should already exist
        } else { 
            // Should NOT have a button
            if (currentWidget) {
                treeWidget->removeItemWidget(item, 1);
                delete currentWidget;
            }
        }
        ++it;
    }
}

// add a new item to an array
void InfoPanel::onAddArrayItem() {
    // only allow adding items in edit mode
    if (!inEditMode) return; 

    // get the button that triggered the signal
    QToolButton* button = qobject_cast<QToolButton*>(sender());
    if (!button) return;

    // get the parent array item
    QTreeWidgetItem* parentArrayItem = static_cast<QTreeWidgetItem*>(button->property("treeItem").value<void*>());
    if (!parentArrayItem) return;

    // get the new index
    int newIndex = parentArrayItem->childCount();
    QString newItemKey = QString("[%1]").arg(newIndex);

    // create a new item
    QTreeWidgetItem* newItem = new QTreeWidgetItem(parentArrayItem);
    newItem->setText(0, newItemKey);
    newItem->setText(1, ""); // Default empty value
    newItem->setData(0, Qt::UserRole, "leaf");

    // get the depth of the new item
    int depth = getItemDepth(newItem);
    QFont itemFont = calculateFontForDepth(depth);
    newItem->setFont(0, itemFont);
    newItem->setFont(1, itemFont);

    // if in edit mode, make the new item editable and set it as the current item and edit it
    if (inEditMode) {
        setTreeItemEditable(newItem, true);
        treeWidget->setCurrentItem(newItem, 1);
        treeWidget->editItem(newItem, 1);
    }
    // notify that data changed
    onItemChanged(newItem, 1); 
}

// add a new item to an object
void InfoPanel::onAddObjectItem() {
    // only allow adding items in edit mode
    if (!inEditMode) return; 

    // get the button that triggered the signal
    QToolButton* button = qobject_cast<QToolButton*>(sender());
    if (!button) return;

    // get the parent object item
    QTreeWidgetItem* parentObjectItem = static_cast<QTreeWidgetItem*>(button->property("treeItem").value<void*>());
    if (!parentObjectItem) return;

    // get the new key
    bool ok;
    QString newKey = QInputDialog::getText(this, tr("New key for object"),
                                           tr("Key name:"), QLineEdit::Normal,
                                           "", &ok);
    if (ok && !newKey.isEmpty()) {
        // check if the new key already exists
        for (int i = 0; i < parentObjectItem->childCount(); ++i) {
            if (parentObjectItem->child(i)->text(0) == newKey) {
                QMessageBox::warning(this, tr("Key already exists"),
                                     tr("A attribute with the key '%1' already exists in this object.").arg(newKey));
                return;
            }
        }

        // create a new item
        QTreeWidgetItem* newItem = new QTreeWidgetItem(parentObjectItem);
        newItem->setText(0, newKey);
        newItem->setText(1, ""); // Default empty value
        newItem->setData(0, Qt::UserRole, "leaf");

        // get the depth of the new item and set the font
        int depth = getItemDepth(newItem);
        QFont itemFont = calculateFontForDepth(depth);
        newItem->setFont(0, itemFont);
        newItem->setFont(1, itemFont);

        // if in edit mode, make the new item editable and set it as the current item and edit it
        if (inEditMode) {
            setTreeItemEditable(newItem, true);
            treeWidget->setCurrentItem(newItem, 1);
            treeWidget->editItem(newItem, 1);
        }
        // notify that data changed
        onItemChanged(newItem, 1); 
    }
}

// calculate the font for the item based on the depth
QFont InfoPanel::calculateFontForDepth(int depth) const {
    double scaledSize = static_cast<double>(m_baseFontSize);
    if (depth > 0) { 
        scaledSize *= std::pow(m_fontScaleFactor, depth);
    }

    // clamp the font size
    double finalSize = std::max(static_cast<double>(m_minFontSize),
                                std::min(scaledSize, static_cast<double>(m_maxFontSize)));

    // start with the tree widget's default font to inherit family etc.
    QFont font = treeWidget->font(); 
    // use setPointSizeF for potentially fractional sizes
    font.setPointSizeF(finalSize);   
    return font;
}

// get the depth of the item in the tree
int InfoPanel::getItemDepth(QTreeWidgetItem* item) const {
    if (!item) {
        return 0; // Or handle as an error, though 0 is safe for font calculation
    }
    int depth = 0;
    QTreeWidgetItem* currentParent = item->parent();
    while (currentParent && currentParent != treeWidget->invisibleRootItem()) {
        depth++;
        currentParent = currentParent->parent();
    }
    return depth;
}

// check if the item is deletable
bool InfoPanel::isItemDeletable(QTreeWidgetItem* item) const {
    // item is not in edit mode or has no parent, it cannot be deleted via hover
    if (!inEditMode || !item || !item->parent()) {
        return false;
    }
    // Only "leaf" items (individual values) are deletable via this hover mechanism.
    if (item->data(0, Qt::UserRole).toString() != "leaf") {
        return false;
    }
    // The parent of this leaf item must be a "lowest collection" (a simple array or object) and parent should not be readonly
    QVariant readonlyVariant = item->parent()->data(0, SchemaReadonlyRole);
    bool readonly = readonlyVariant.isValid() ? readonlyVariant.toBool() : false;
    // qDebug() << "Checking if item is deletable:" << item->text(0) << "Readonly:" << readonly << "Parent Key:" << item->parent()->text(0) << "Parent Readonly:" << readonlyVariant;
    // qDebug() << "Parent of parent" << item->parent()->parent()->text(0);
    return isLowestCollection(item->parent()) && !isHighestItem(item->parent()) && !readonly;
}

// get the rect for the delete button
QRect InfoPanel::getDeleteButtonRectForItem(const QTreeWidgetItem* item, const QStyleOptionViewItem& option) const {
    Q_UNUSED(item);
    
    int buttonSize = 16; 
    int margin = 2; 
    QRect itemRect = option.rect; 

    return QRect(itemRect.right() - buttonSize - margin,
                 itemRect.top() + (itemRect.height() - buttonSize) / 2,
                 buttonSize, buttonSize);
}

// paint the delete button
void InfoPanel::paintDeleteItemButton(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) {
    // if the index is valid and the index is the hovered item for delete, paint the delete button
    if (index.isValid() && index == hoveredItemForDelete) {
        // get the item
        QTreeWidgetItem* item = static_cast<QTreeWidgetItem*>(index.internalPointer());
        // if the item is deletable, paint the delete button
        if (isItemDeletable(item)) {
            // get the rect for the delete button
            QRect buttonRect = getDeleteButtonRectForItem(item, option);
            // paint the delete button
            painter->drawPixmap(buttonRect, deleteIcon);
        }
    }
}

// handle the delete action
void InfoPanel::handleDeleteAction(QTreeWidgetItem* itemToDelete) {
    // if the item to delete is not valid or has no parent, return
    if (!itemToDelete || !itemToDelete->parent()) return;

    // get the parent item
    QTreeWidgetItem* parentItem = itemToDelete->parent();
    // // get the index of the item to delete
    // int removedAtIndex = parentItem->indexOfChild(itemToDelete); 

    // remove the item from the parent
    parentItem->removeChild(itemToDelete);
    // delete the item
    delete itemToDelete; 

    // if the parent is an array, re-index subsequent siblings
    if (parentItem->data(0, Qt::UserRole).toString() == "array") {
        for (int i = 0; i < parentItem->childCount(); ++i) {
            // re-index items that were at or after the removed item's original position
            parentItem->child(i)->setText(0, QString("[%1]").arg(i));
        }
    }

    onItemChanged(nullptr, -1); // Indicate a general change
    updateAddButtons(true);     // Parent might now be empty or its status changed
    
    // If the deleted item was hovered, clear the hover state
    if (hoveredItemForDelete.isValid() && static_cast<QTreeWidgetItem*>(hoveredItemForDelete.internalPointer()) == itemToDelete) {
        hoveredItemForDelete = QPersistentModelIndex();
    }
    treeWidget->update(); // Force repaint of the tree view area
}

// Handle events for the tree widget viewport, including mouse hover and click interactions
bool InfoPanel::eventFilter(QObject* watched, QEvent* event) {
    if (watched == treeWidget->viewport()) {
        // If not in edit mode, clear any existing hover states and pass through
        if (!inEditMode) { 
            // Clear hover state for delete buttons when not in edit mode
            if (hoveredItemForDelete.isValid()) { 
                QModelIndex oldHover = hoveredItemForDelete;
                hoveredItemForDelete = QPersistentModelIndex();
                treeWidget->update(oldHover);
            }
            return QWidget::eventFilter(watched, event);
        }

        // Handle mouse movement events to show/hide delete buttons on hover
        if (event->type() == QEvent::MouseMove || event->type() == QEvent::HoverMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QModelIndex index = treeWidget->indexAt(mouseEvent->pos());
            QTreeWidgetItem* item = nullptr;
            if (index.isValid()) {
                item = static_cast<QTreeWidgetItem*>(index.internalPointer());
            }

            QModelIndex oldHoveredIndex = hoveredItemForDelete;
            QPersistentModelIndex newHoverPersistentIndex; // Default to invalid

            // Show delete button only for deletable items
            if (item && isItemDeletable(item)) {
                newHoverPersistentIndex = index;
            }
            // If item is not deletable or index is invalid, newHoverPersistentIndex remains invalid

            // Update hover state and repaint affected items if hover changed
            if (oldHoveredIndex != newHoverPersistentIndex) {
                hoveredItemForDelete = newHoverPersistentIndex;
                if (oldHoveredIndex.isValid()) {
                    treeWidget->update(oldHoveredIndex); // Repaint old hovered item (remove button)
                }
                if (hoveredItemForDelete.isValid()) {
                    treeWidget->update(hoveredItemForDelete); // Repaint new hovered item (add button)
                }
            }
            return true; // Event handled
        }
        // Handle mouse leaving the tree widget area
        else if (event->type() == QEvent::HoverLeave || event->type() == QEvent::Leave) {
             if (hoveredItemForDelete.isValid()) {
                QModelIndex oldHover = hoveredItemForDelete;
                hoveredItemForDelete = QPersistentModelIndex();
                treeWidget->update(oldHover); // Repaint to remove button
            }
            return true; // Event handled
        }
        // Handle mouse clicks on delete buttons
        else if (event->type() == QEvent::MouseButtonPress) {
            if (hoveredItemForDelete.isValid()) {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                QTreeWidgetItem* item = static_cast<QTreeWidgetItem*>(hoveredItemForDelete.internalPointer());
                // Create style option to get proper rect calculation for delete button positioning
                QStyleOptionViewItem option;
                option.rect = treeWidget->visualRect(hoveredItemForDelete); // Get rect for the specific cell
                // Note: For column-specific delegates, this might need index for column 1
                // Current implementation assumes visualRect(hoveredItemForDelete) provides sufficient rect

                QRect buttonRect = getDeleteButtonRectForItem(item, option);
                if (buttonRect.contains(mouseEvent->pos())) {
                    handleDeleteAction(item);
                    // hoveredItemForDelete will be cleared in handleDeleteAction if it was the deleted item
                    return true; // Event handled
                }
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

// update the save button state
void InfoPanel::updateSaveButtonState() {
    if (!inEditMode) return;
    
    bool hasInvalidFields = !invalidRequiredFields.isEmpty();

    // on invalid fields, disable the save button and color it red
    if (hasInvalidFields) {
        saveButton->setEnabled(false);
        saveButton->setStyleSheet("QPushButton { background-color: #ffcccc; color: #666; }");
        saveButton->setToolTip(tr("Please fill in all required fields"));
        return;
    }
    // if the original data is not the same as the collected data, enable the save button and color it orange
    else if (getOriginalData() != collectDataFromTree()) {
        saveButton->setEnabled(true);
        saveButton->setStyleSheet("QPushButton { background-color: orange; }");
        saveButton->setToolTip(tr("Save changes"));
        return;
    }
    // if the original data is the same as the collected data, disable the save button and color it light gray
    else {
        saveButton->setEnabled(false);
        saveButton->setStyleSheet("QPushButton { background-color: lightgray; }");
        saveButton->setToolTip(tr("No changes to save"));
    }
}

// validate all required fields on load
void InfoPanel::validateAllRequiredFieldsOnLoad() {
    if (!inEditMode) return;
    
    // clear the invalid required fields
    invalidRequiredFields.clear();
    // validate all required fields recursively
    validateRequiredFieldsRecursive(treeWidget->invisibleRootItem());
    updateSaveButtonState();
}

// validate all required fields recursively
void InfoPanel::validateRequiredFieldsRecursive(QTreeWidgetItem* item) {
    if (!item) return;
    
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem* child = item->child(i);
        
        if (child->data(0, Qt::UserRole).toString() == "leaf") {
            QModelIndex childIndex = treeWidget->indexFromItem(child, 1);
            updateFieldValidationState(childIndex); 
        } else {
            validateRequiredFieldsRecursive(child);
        }
    }
}

// check if the field is valid
bool InfoPanel::isFieldValid(const QModelIndex& index) const {
    if (!inEditMode || !index.isValid()) return true; // Not in edit mode or invalid index

    QTreeWidgetItem* item = static_cast<QTreeWidgetItem*>(index.internalPointer());
    // bool isRequired = item->data(1, SchemaRequiredRole).toBool();
    
    if (!validateRequiredField(item)) {
        return false; // Required field is empty
    }
    if (!validateFieldPattern(item)) {
        return false; // Pattern validation failed
    }

    return true;
}

// validate the required field
bool InfoPanel::validateRequiredField(const QTreeWidgetItem* item) const {
    if (!item) return true; // No item to validate

    bool isRequired = item->data(1, SchemaRequiredRole).toBool();
    QString value = item->text(1).trimmed();
    
    if (isRequired && value.isEmpty()) {
        return false; // Required field is empty
    }
    
    return true;
}

// validate the field pattern
bool InfoPanel::validateFieldPattern(const QTreeWidgetItem* item) const {
    QString pattern = item->data(1, SchemaPatternRole).toString(); // Add new role
    if (pattern.isEmpty()) return true;

    QString value = item->text(1).trimmed();

    QRegularExpression regex(pattern);
    return regex.match(value).hasMatch();
}

// update the field validation state
void InfoPanel::updateFieldValidationState(const QModelIndex& index) {
    if (!index.isValid()) return;
    
    bool isValid = isFieldValid(index);
    QPersistentModelIndex persistentIndex(index);
    
    if (isValid) {
        invalidRequiredFields.remove(persistentIndex);
    } else {
        invalidRequiredFields.insert(persistentIndex);
    }
    
    updateSaveButtonState();
    
    // update the tree widget
    treeWidget->update(index);
}

/**
 * @brief Creates a checkbox for an optional field
 * @param item The tree widget item representing the optional field
 * 
 * Creates a checkbox that controls whether the optional field is enabled.
 * When checked, the field and its subcomponents are available for editing.
 * When unchecked, the field is hidden/unavailable.
 */
void InfoPanel::createOptionalCheckbox(QTreeWidgetItem* item, bool checked) {
    if (!item) {
        qDebug() << "Cannot create checkbox for null item";
        return;
    }
    
    // create a container widget to hold both checkbox and label
    QWidget* containerWidget = new QWidget(treeWidget); 
    containerWidget->setAttribute(Qt::WA_DeleteOnClose, false); 
    
    QHBoxLayout* layout = new QHBoxLayout(containerWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);
    
    // Create the checkbox with proper parent
    QCheckBox* checkbox = new QCheckBox(containerWidget);
    checkbox->setChecked(checked);
    checkbox->setToolTip("Enable/disable this optional field (only in edit mode)");
    
    // Create a label with the item's text
    QLabel* textLabel = new QLabel(item->text(0), containerWidget);
    textLabel->setStyleSheet("QLabel { background: transparent; }");
    
    // Add both to the layout
    layout->addWidget(checkbox);
    layout->addWidget(textLabel);
    layout->addStretch();
    
    // Store the checkbox reference BEFORE setting the widget
    optionalCheckboxes[item] = checkbox;
    optionalFieldStates[item] = checked;
    
    // Clear the item's text because shown in the label
    item->setText(0, "");
    
    // Set the container widget for column 0 (key column)
    treeWidget->setItemWidget(item, 0, containerWidget);
    
    // Connect checkbox toggle to handler with proper connection type
    connect(checkbox, &QCheckBox::toggled, this, &InfoPanel::onOptionalCheckboxToggled, Qt::DirectConnection);
    
    // qDebug() << "Successfully created checkbox for optional field:" << textLabel->text();
}

/**
 * @brief Handles checkbox toggle events for optional fields
 * @param checked Whether the checkbox is checked
 * 
 * When an optional field checkbox is toggled, this method updates the visibility
 * of the associated field and all its subcomponents. If not in edit mode, 
 * the toggle is reverted.
 */
void InfoPanel::onOptionalCheckboxToggled(bool checked) {
    QCheckBox* checkbox = qobject_cast<QCheckBox*>(sender());
    if (!checkbox) return;
    
    // If not in edit mode, revert the checkbox state and return
    if (!inEditMode) {
        // Temporarily disconnect the signal to avoid infinite recursion
        disconnect(checkbox, &QCheckBox::toggled, this, &InfoPanel::onOptionalCheckboxToggled);
        checkbox->setChecked(!checked); // Revert the toggle
        // Reconnect the signal
        connect(checkbox, &QCheckBox::toggled, this, &InfoPanel::onOptionalCheckboxToggled);
        return;
    }
    
    // find the item associated with this checkbox
    QTreeWidgetItem* item = nullptr;
    for (QMap<QTreeWidgetItem*, QCheckBox*>::const_iterator it = optionalCheckboxes.constBegin(); it != optionalCheckboxes.constEnd(); ++it) {
        if (it.value() == checkbox) {
            item = it.key();
            break;
        }
    }
    
    if (!item) return;
    
    // Update the field state
    optionalFieldStates[item] = checked;
    
    // Update visibility of the field and its children
    updateOptionalFieldVisibility(item, checked, 0);
}

/**
 * @brief Updates the visibility of an optional field and its children
 * @param item The tree widget item to update
 * @param visible Whether the field should be visible/enabled
 * 
 * Recursively updates the visibility of the field and all its subcomponents.
 * Hidden fields are excluded from data collection.
 */
void InfoPanel::updateOptionalFieldVisibility(QTreeWidgetItem* item, bool visible, int depth) {
    if (!item) return;
    
    // Update visibility of the value column (column 1)
    if (visible) {
        item->setHidden(false);
        item->setFlags(item->flags() | Qt::ItemIsEnabled);
        
        // Restore original text if it was cleared
        QString originalValue = item->data(1, Qt::UserRole + 20).toString(); // Use a custom role for backup
        if (!originalValue.isEmpty()) {
            item->setText(1, originalValue);
            item->setData(1, Qt::UserRole + 20, QVariant()); // Clear backup
        }
    } else if (depth > 0) { // Only hide if not the root item
        // Backup current value before hiding
        QString currentValue = item->text(1);
        if (!currentValue.isEmpty()) {
            item->setData(1, Qt::UserRole + 20, currentValue); // Backup the value
        }
        
        item->setHidden(true);
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    }
    
    // Recursively update all children
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem* child = item->child(i);
        updateOptionalFieldVisibility(child, visible, depth + 1);
    }
}

/**
 * @brief Sets the visibility of all optional fields based on their checkbox states
 * 
 * This method is called during mode switches (edit/view) to ensure all optional
 * field visibilities are correctly applied.
 */
void InfoPanel::setOptionalFieldsVisibility() {
    for (QMap<QTreeWidgetItem*, bool>::const_iterator it = optionalFieldStates.constBegin(); it != optionalFieldStates.constEnd(); ++it) {
        QTreeWidgetItem* item = it.key();
        bool enabled = it.value();
        updateOptionalFieldVisibility(item, enabled, 0);
    }
}

/**
 * @brief Checks if an optional field is currently enabled
 * @param item The tree widget item to check
 * @return True if the field is enabled or not optional, false if disabled
 * 
 * Returns the current state of an optional field. Non-optional fields
 * always return true.
 */
bool InfoPanel::isOptionalFieldEnabled(QTreeWidgetItem* item) const {
    if (!item) return true;
    
    // Check if this item has an optional checkbox
    if (optionalFieldStates.contains(item)) {
        return optionalFieldStates.value(item);
    }
    
    // Check if any parent is an optional field that's disabled
    QTreeWidgetItem* parent = item->parent();
    while (parent) {
        if (optionalFieldStates.contains(parent)) {
            return optionalFieldStates.value(parent);
        }
        parent = parent->parent();
    }
    
    return true; // Default to enabled for non-optional fields
}

/**
 * @brief Handles header section clicks, specifically for column 0 to toggle collapse/expand all
 * @param logicalIndex The logical index of the clicked header section (0 = "Attribut", 1 = "Wert")
 * 
 * When column 0 ("Attribut") header is clicked, toggles between expanding all tree branches 
 * and collapsing all tree branches. Clicking on other columns has no effect.
 * 
 * @exception None
 * @purpose Provides user-friendly way to collapse/expand entire tree with header click
 */
void InfoPanel::onHeaderSectionClicked(int logicalIndex) {
    // qDebug() << "Header section clicked! Index:" << logicalIndex;
    
    // Only respond to clicks on column 0 (the "Attribut" column)
    if (logicalIndex != 0) {
        qDebug() << "Ignoring click on column" << logicalIndex << "(only column 0 supported)";
        return;
    }
    // Toggle the expanded state
    m_treeExpandedState = !m_treeExpandedState;
    
    if (m_treeExpandedState) {
        treeWidget->expandAll();
    } else {
        treeWidget->collapseAll();
    }
    
    // Resize columns to fit content after expand/collapse
    treeWidget->resizeColumnToContents(0);
    treeWidget->resizeColumnToContents(1);
}


/**
 * @brief Clears all optional field components safely
 * 
 * This method is used to safely remove all optional field components from the tree widget.
 * It ensures that all references are properly removed and the widgets are deleted in a safe manner.
 * 
 * @exception None
 * @purpose Provides a safe way to remove optional field components from the tree widget
 * 
 * @note This method was created with the help of AI, as I had some issues when switching between edit and view mode or displaying new information in between.
 */
void InfoPanel::clearOptionalFieldComponentsSafely() {
    // qDebug() << "Starting safe cleanup of optional field components";
    
    // Phase 1: Remove all item widgets from the tree to prevent Qt from trying to paint them
    for (QMap<QTreeWidgetItem*, QCheckBox*>::const_iterator it = optionalCheckboxes.constBegin(); it != optionalCheckboxes.constEnd(); ++it) {
        QTreeWidgetItem* item = it.key();
        if (item) {
            // remove the widget from the tree widget before deleting it
            QWidget* containerWidget = treeWidget->itemWidget(item, 0);
            if (containerWidget) {
                treeWidget->removeItemWidget(item, 0);
                // deletion happens later as qt might still have references
            }
        }
    }
    
    // Phase 2: Force Qt to process any pending paint events with the removed widgets
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    
    // Phase 3: Now safely delete the container widgets
    for (QMap<QTreeWidgetItem*, QCheckBox*>::const_iterator it = optionalCheckboxes.constBegin(); it != optionalCheckboxes.constEnd(); ++it) {
        QTreeWidgetItem* item = it.key();
        if (item) {
            // The widget should already be removed from the tree, now safe to delete
            QWidget* containerWidget = qobject_cast<QWidget*>(it.value()->parent());
            if (containerWidget) {
                containerWidget->deleteLater(); // Use deleteLater for safer cleanup
            }
        }
    }
    
    // Phase 4: Clear all tracking data structures
    optionalCheckboxes.clear();
    optionalFieldStates.clear();
    
    // Phase 5: Force another event processing cycle to ensure deleteLater is processed
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    
    // qDebug() << "Completed safe cleanup of optional field components";
}