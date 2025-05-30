#include <QWidget>
#include <QTreeWidget>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QIcon>
#include <QPushButton>
#include <QMessageBox>
#include <QStyledItemDelegate>
#include <QToolButton>
#include <QInputDialog>
#include <QLineEdit>
#include <QPainter>
#include <QMouseEvent>
#include <QStyleOptionViewItem>
#include <QSpinBox>
#include <QDateEdit>
#include <QComboBox>
#include <QDebug>

#include <QEvent>
#include "infopanel.h"

class ReadOnlyDelegate : public QStyledItemDelegate {
public:
    ReadOnlyDelegate(InfoPanel* panel, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), infoPanelPtr(panel) {}

    using QStyledItemDelegate::QStyledItemDelegate;
    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override
    {
        if (index.column() == 0)
            return nullptr;

        QString itemType = index.data(SchemaTypeRole).toString();
        QString itemFormat = index.data(SchemaFormatRole).toString();
        QVariant enumValuesVar = index.data(SchemaEnumValuesRole);
        
        // Debug output to check what we get from the index
        qDebug() << "Debug createEditor: row=" << index.row() << "col=" << index.column();
        qDebug() << "  itemType=" << itemType;
        qDebug() << "  itemFormat=" << itemFormat;
        qDebug() << "  enumValues=" << enumValuesVar;

        if (itemType == "integer") {
            QSpinBox* editor = new QSpinBox(parent);
            editor->setFrame(false);
            editor->setMinimum(-2147483647); // Or from schema
            editor->setMaximum(2147483647);  // Or from schema
            return editor;
        }
        else if (itemType == "string" && itemFormat == "date") {
            QDateEdit* editor = new QDateEdit(parent);
            editor->setFrame(false);
            editor->setCalendarPopup(true);
            editor->setDisplayFormat("yyyy-MM-dd"); // Or from schema/locale
            return editor;
        }
        else if (itemType == "boolean") {
            QComboBox* editor = new QComboBox(parent);
            editor->addItem(tr("true"), true);
            editor->addItem(tr("false"), false);
            editor->setFrame(false);
            return editor;
        }
        else if (itemType == "string" && enumValuesVar.canConvert<QStringList>()) {

            QStringList enumValues = enumValuesVar.toStringList();
            if (!enumValues.isEmpty()) {
                QComboBox* editor = new QComboBox(parent);
                editor->addItems(enumValues);
                editor->setFrame(false);
                return editor;
            }
        }
        
        // Fallback
        return QStyledItemDelegate::createEditor(parent, option, index);
    }


    void setEditorData(QWidget* editor, const QModelIndex& index) const override {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QString itemType = index.data(SchemaTypeRole).toString();
        QString itemFormat = index.data(SchemaFormatRole).toString();

        if (itemType == "integer") {
            QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
            if (spinBox) spinBox->setValue(value.toInt());
        } else if (itemType == "string" && itemFormat == "date") {
            QDateEdit* dateEdit = qobject_cast<QDateEdit*>(editor);
            if (dateEdit) dateEdit->setDate(QDate::fromString(value, "yyyy-MM-dd"));
        } else if (itemType == "boolean") {
            QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
            if (comboBox) {
                if (value.toLower() == "true") comboBox->setCurrentIndex(0);
                else comboBox->setCurrentIndex(1);
            }
        } else if (itemType == "string" && index.data(SchemaEnumValuesRole).canConvert<QStringList>()) {
            QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
            if (comboBox) {
                int enumIdx = comboBox->findText(value);
                if (enumIdx != -1) comboBox->setCurrentIndex(enumIdx);
            }
        }
        else {
            QStyledItemDelegate::setEditorData(editor, index);
        }
    }


    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override {
        QString itemType = index.data(SchemaTypeRole).toString();
        QString itemFormat = index.data(SchemaFormatRole).toString();

        if (itemType == "integer") {
            QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);
            if (spinBox) model->setData(index, spinBox->value(), Qt::EditRole);
        } else if (itemType == "string" && itemFormat == "date") {
            QDateEdit* dateEdit = qobject_cast<QDateEdit*>(editor);
            if (dateEdit) model->setData(index, dateEdit->date().toString("yyyy-MM-dd"), Qt::EditRole);
        } else if (itemType == "boolean") {
            QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
            if (comboBox) model->setData(index, comboBox->currentData().toBool(), Qt::EditRole);
        } else if (itemType == "string" && index.data(SchemaEnumValuesRole).canConvert<QStringList>()) {
             QComboBox* comboBox = qobject_cast<QComboBox*>(editor);
             if (comboBox) model->setData(index, comboBox->currentText(), Qt::EditRole);
        }
        else {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    }

    void paint(QPainter* painter,
                const QStyleOptionViewItem& option,
                const QModelIndex& index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);

        if (infoPanelPtr && index.column() == 1) {
            infoPanelPtr->paintDeleteItemButton(painter, option, index);
        }
    }
    private:
        InfoPanel* infoPanelPtr; // pointer to the InfoPanel instance
};

InfoPanel::InfoPanel(QWidget *parent) : QWidget(parent), treeWidget(new QTreeWidget(this)), inEditMode(false),
    m_baseFontSize(15), m_fontScaleFactor(0.9), m_minFontSize(7), m_maxFontSize(15) // Initialize font params
{
    treeWidget->setHeaderLabels({tr("Attribut"), tr("Wert")});
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Create horizontal layout for buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    // set icon of the save button
    saveButton = new QPushButton(tr("Speichern"), this);
    saveButton->setIcon(QIcon(":/icons/save.png"));
    saveButton->setVisible(true);
    saveButton->setEnabled(false);
    saveButton->setStyleSheet("QPushButton { background-color: lightgray; }");
    buttonLayout->addWidget(saveButton);

    // set the icon of the edit button
    editButton = new QPushButton(tr("Bearbeiten"), this);
    editButton->setIcon(QIcon(":/icons/edit.png"));
    editButton->setVisible(true);
    buttonLayout->addWidget(editButton);

    // Add button layout first, then treeWidget
    layout->addLayout(buttonLayout);
    layout->addWidget(treeWidget);

    treeWidget->setItemDelegate(new ReadOnlyDelegate(this, treeWidget)); 

    // connect signals and slots
    connect(saveButton, &QPushButton::clicked, this, &InfoPanel::saveChanges);
    connect(editButton, &QPushButton::clicked, this, &InfoPanel::enterEditMode);
    connect(treeWidget, &QTreeWidget::itemChanged, this, &InfoPanel::onItemChanged);

    // load the delete icon
    deleteIcon = QPixmap(":/icons/quit.png");
    if (deleteIcon.isNull()) {
        QMessageBox::warning(this, tr("Fehler"), tr("Das Symbol für das Löschen konnte nicht geladen werden."));
    }

    treeWidget->viewport()->installEventFilter(this); // Install event filter to handle mouse events
    treeWidget->viewport()->setMouseTracking(true); // Enable mouse tracking for the viewport
}

InfoPanel::~InfoPanel() {
    delete treeWidget;
}

void InfoPanel::displayInfo(const QJsonObject& jsonObject) {
    originalData = jsonObject; // Store original data for canceling edits
    treeWidget->clear();
    hoveredItemForDelete = QPersistentModelIndex(); // Reset the hovered item for delete button

    // reset buttons
    saveButton->setEnabled(false);
    saveButton->setStyleSheet("QPushButton { background-color: lightgray; }");
    editButton->setText(tr("Bearbeiten"));
    editButton->setIcon(QIcon(":/icons/edit.png"));
    
    // Für jedes Top-Level-Objekt einen Haupteintrag erstellen
    // for (const QString& key : jsonObject.keys()) {
    //     QTreeWidgetItem *topItem = new QTreeWidgetItem(treeWidget, {key, ""});
    //     topItem->setFirstColumnSpanned(true); // Nur der Schlüssel wird angezeigt
    //     addJsonToTreeRecursive(jsonObject[key], topItem);
    //     treeWidget->addTopLevelItem(topItem);
    // }
    addJsonToTreeRecursive(jsonObject, treeWidget->invisibleRootItem());
    
    treeWidget->expandAll();
    treeWidget->resizeColumnToContents(0);
    treeWidget->resizeColumnToContents(1);

    // Initial nicht editierbar
    inEditMode = false;
    setTreeItemsEditable(inEditMode);
    updateAddButtons(inEditMode);
}



void InfoPanel::displayInfo(const QJsonObject& jsonObject, const QJsonObject& schemaObject) {
    originalData = jsonObject;
    currentSchema = schemaObject; // Store the schema
    treeWidget->clear();
    hoveredItemForDelete = QPersistentModelIndex();

    saveButton->setEnabled(false);
    saveButton->setStyleSheet("QPushButton { background-color: lightgray; }");
    editButton->setText(tr("Bearbeiten"));
    editButton->setIcon(QIcon(":/icons/edit.png"));
    
    // for (const QString& key : jsonObject.keys()) {
    //     QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(treeWidget);
    //     topLevelItem->setText(0, key);
    //     // Pass the schema definition for this specific key
    //     addJsonToTreeRecursive(jsonObject[key], topLevelItem, 0, currentSchema.value(key).toObject());
    // }

    // now recurse once on the invisible root
    addJsonToTreeRecursive(jsonObject,
                           treeWidget->invisibleRootItem(),
                           0,
                           currentSchema);

    treeWidget->expandAll();
    treeWidget->resizeColumnToContents(0);
    treeWidget->resizeColumnToContents(1);

    setTreeItemsEditable(inEditMode);
    updateAddButtons(inEditMode);
}


void InfoPanel::addJsonToTreeRecursive(const QJsonValue& valueForThisItem, QTreeWidgetItem* thisItem, int depth, const QJsonObject& currentItemSchema) {
    QFont itemFont = calculateFontForDepth(depth);
    thisItem->setFont(0, itemFont); // Font for the key column of thisItem (e.g., "person", or "fname")

    if (valueForThisItem.isObject()) {
        thisItem->setData(0, Qt::UserRole, "object"); // 'thisItem' (e.g., "person", "subclass_params") represents an object.
        QJsonObject obj = valueForThisItem.toObject();

        QJsonObject propertiesSchema;
        if (currentItemSchema.value("type").toString() == "object" && currentItemSchema.contains("properties")) {
            propertiesSchema = currentItemSchema.value("properties").toObject();
        }
        qDebug() << "Current item schema for object:" << currentItemSchema << "\n";
        qDebug() << "propertiesSchema from" << thisItem->text(0) << "->" << propertiesSchema << "\n";
        for (const QString& key : obj.keys()) {
            QTreeWidgetItem* child = new QTreeWidgetItem(thisItem);
            child->setText(0, key);
            QJsonObject childSchema = propertiesSchema.value(key).toObject();
            qDebug() << key << "->" << childSchema;
            if (childSchema.isEmpty() && currentItemSchema.contains("additionalProperties") && currentItemSchema.value("additionalProperties").isObject()) {
                childSchema = currentItemSchema.value("additionalProperties").toObject();
            }
            addJsonToTreeRecursive(obj[key], child, depth + 1, childSchema);
        }
    } else if (valueForThisItem.isArray()) {
        thisItem->setData(0, Qt::UserRole, "array"); // 'thisItem' (e.g., "transactions", "artist_ids") represents an array.
        QJsonArray array = valueForThisItem.toArray();

        QJsonObject itemSchema; // Schema for individual items in the array
        if (currentItemSchema.value("type").toString() == "array" && currentItemSchema.contains("items")) {
            itemSchema = currentItemSchema.value("items").toObject();
        }

        for (int i = 0; i < array.size(); ++i) {
            QTreeWidgetItem* child = new QTreeWidgetItem(thisItem);
            child->setText(0, QString("[%1]").arg(i));
            addJsonToTreeRecursive(array[i], child, depth + 1, itemSchema);
        }
    } 
    else { // Leaf node
        thisItem->setText(1, valueForThisItem.toVariant().toString());
        thisItem->setData(0, Qt::UserRole, "leaf");
        thisItem->setFont(1, calculateFontForDepth(depth));

        // set the schema type and format for the item
        QString itemType = currentItemSchema.value("type").toString();
        QString itemFormat = currentItemSchema.value("format").toString();
        QStringList enumValues = currentItemSchema.value("enum").toVariant().toStringList();
        thisItem->setData(1, SchemaTypeRole, itemType);
        thisItem->setData(1, SchemaFormatRole, itemFormat);
        thisItem->setData(1, SchemaEnumValuesRole, enumValues);
    }
}

void InfoPanel::addJsonToTreeRecursive(const QJsonValue& valueForThisItem, QTreeWidgetItem* thisItem, int depth) {
    QFont itemFont = calculateFontForDepth(depth);
    thisItem->setFont(0, itemFont);
    thisItem->setFont(1, itemFont);
    if (valueForThisItem.isObject()) {
        thisItem->setData(0, Qt::UserRole, "object");
        QJsonObject obj = valueForThisItem.toObject();
        for (const QString& key : obj.keys()) {
            QTreeWidgetItem* child = new QTreeWidgetItem(thisItem);
            child->setText(0, key);
            addJsonToTreeRecursive(obj[key], child, depth + 1);
        }
    } else if (valueForThisItem.isArray()) {
        thisItem->setData(0, Qt::UserRole, "array");
        QJsonArray array = valueForThisItem.toArray();
        for (int i = 0; i < array.size(); ++i) {
            QTreeWidgetItem* child = new QTreeWidgetItem(thisItem);
            child->setText(0, QString("[%1]").arg(i));
            addJsonToTreeRecursive(array[i], child, depth + 1);
        }
    } else { // Leaf node
        thisItem->setText(1, valueForThisItem.toVariant().toString());
        thisItem->setData(0, Qt::UserRole, "leaf");
    }
}

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

bool InfoPanel::isHighestItem(QTreeWidgetItem* item) const {
    if (!item || !item->parent()) {
        // If item is null or has no parent (e.g., it might be the invisible root itself,
        // or an item not yet added to a tree), it's not considered a user-visible highest item.
        // qDebug() << "Item is null or has no parent.";
        return true;
    }
    // A "highest item" (top-level item) is a direct child of the tree widget's invisible root item.
    return item->parent() == treeWidget->invisibleRootItem();
    
}

void InfoPanel::enterEditMode() {
    inEditMode = true;
    
    // TreeWidget editierbar machen
    setTreeItemsEditable(true);
    
    // Button-Zustände ändern
    saveButton->setEnabled(true);
    saveButton->setStyleSheet("QPushButton { background-color: lightblue; }");
    
    // change the edit button to cancel button
    editButton->setText(tr("Abbrechen"));
    editButton->setIcon(QIcon(":/icons/quit.png"));

    // Signal-Verbindungen umschalten
    disconnect(editButton, &QPushButton::clicked, this, &InfoPanel::enterEditMode);
    connect(editButton, &QPushButton::clicked, this, &InfoPanel::cancelEditMode);

    updateAddButtons(true);
}

void InfoPanel::saveChanges() {
    if (!inEditMode) return;
    
    // Daten aus Tree sammeln
    QJsonObject modifiedData = collectDataFromTree();
    
    // Signal aussenden
    emit saveRequested(modifiedData);
    
    // Edit-Modus verlassen
    inEditMode = false;
    setTreeItemsEditable(false);
    
    // Button-Zustände zurücksetzen
    saveButton->setEnabled(false);
    saveButton->setStyleSheet("QPushButton { background-color: lightgray; }");
    editButton->setText(tr("Bearbeiten"));
    editButton->setIcon(QIcon(":/icons/edit.png"));
    
    // Signal-Verbindungen zurücksetzen
    disconnect(editButton, &QPushButton::clicked, this, &InfoPanel::cancelEditMode);
    connect(editButton, &QPushButton::clicked, this, &InfoPanel::enterEditMode);
    
    // QMessageBox::information(this, tr("Gespeichert"), tr("Änderungen wurden übernommen."));
    updateAddButtons(false); // Hide add buttons after saving
    if (hoveredItemForDelete.isValid()) {
        // If a delete button was hovered, we need to reset it
        treeWidget->update(hoveredItemForDelete);
    }
    hoveredItemForDelete = QPersistentModelIndex(); // Reset the hovered item for delete button
}

void InfoPanel::cancelEditMode() {
    // Originaldaten wiederherstellen
    restoreOriginalData();
    
    // Edit-Modus verlassen
    inEditMode = false;
    setTreeItemsEditable(false);
    updateAddButtons(false); // Hide add buttons
    
    // Button-Zustände zurücksetzen
    saveButton->setEnabled(false);
    saveButton->setStyleSheet("QPushButton { background-color: lightgray; }");
    editButton->setText(tr("Bearbeiten"));
    editButton->setIcon(QIcon(":/icons/edit.png"));
    
    // Signal-Verbindungen zurücksetzen
    disconnect(editButton, &QPushButton::clicked, this, &InfoPanel::cancelEditMode);
    connect(editButton, &QPushButton::clicked, this, &InfoPanel::enterEditMode);

    // updateAddButtons(false); // Hide add buttons after canceling
    if (hoveredItemForDelete.isValid()) { // Force repaint
        treeWidget->update(hoveredItemForDelete);
    }
    hoveredItemForDelete = QPersistentModelIndex();
}

void InfoPanel::setTreeItemsEditable(bool editable) {
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
        setTreeItemEditable(treeWidget->topLevelItem(i), editable);
    }
}

void InfoPanel::setTreeItemEditable(QTreeWidgetItem* item, bool editable) {
    if (!item) return;
    
    // Nur Blatt-Knoten (Werte) editierbar machen, nicht Schlüssel oder Gruppennamen
    bool isLeaf = item->data(0, Qt::UserRole).toString() == "leaf";
    QString itemKey = item->text(0).toLower();
    
    if (isLeaf && editable && !itemKey.contains("id")) {
        // Nur Spalte 1 (Wert) editierbar machen
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    } else {
        // Nicht editierbar
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
    
    // Rekursiv für alle Kinder
    for (int i = 0; i < item->childCount(); ++i) {
        setTreeItemEditable(item->child(i), editable);
    }
}

void InfoPanel::onItemChanged(QTreeWidgetItem* item, int column) {
    Q_UNUSED(item); // Item might be useful for more granular change tracking later
    Q_UNUSED(column); // Column might be useful
    if (!inEditMode) return; // Only react to changes if in edit mode
    
    // Enable save button and change style if any relevant item changed.
    // The itemChanged signal is quite broad; this simple check is a starting point.
    saveButton->setEnabled(true);
    saveButton->setStyleSheet("QPushButton { background-color: orange; }");
}

QJsonObject InfoPanel::collectDataFromTree() {
    QJsonObject result;
    
    for (int i = 0; i < treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem* topItem = treeWidget->topLevelItem(i);
        QString groupName = topItem->text(0);
        QJsonValue groupValue = getValueFromItem(topItem);
        result[groupName] = groupValue;
    }
    
    return result;
}

QJsonValue InfoPanel::getValueFromItem(QTreeWidgetItem* item) {
    QString itemType = item->data(0, Qt::UserRole).toString();

    if (itemType == "leaf" || item->childCount() == 0) { // Leaf or no children implies direct value
        QString text = item->text(1);
        bool ok;
        int intVal = text.toInt(&ok);
        if (ok) return intVal;
        
        double doubleVal = text.toDouble(&ok);
        if (ok) return doubleVal;
        
        if (text.toLower() == "true") return true;
        if (text.toLower() == "false") return false;
        
        return text;
    } else if (itemType == "array") {
        QJsonArray array;
        for (int i = 0; i < item->childCount(); ++i) {
            array.append(getValueFromItem(item->child(i)));
        }
        return array;
    } else { // Default to object if it has children and is not explicitly an array or leaf
        QJsonObject obj;
        for (int i = 0; i < item->childCount(); ++i) {
            QTreeWidgetItem* child = item->child(i);
            QString key = child->text(0);
            obj[key] = getValueFromItem(child);
        }
        return obj;
    }
    return QJsonValue(); // Fallback, should not happen
}

void InfoPanel::restoreOriginalData() {
    // Einfach die ursprünglichen Daten neu anzeigen
    displayInfo(originalData);
}

void InfoPanel::updateAddButtons(bool show) {
    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        QTreeWidgetItem* item = *it;
        
        // First, always try to remove existing widget if we are hiding or if it's not eligible
        QWidget* currentWidget = treeWidget->itemWidget(item, 1);
        bool shouldHaveButton = show && isLowestCollection(item) && !isHighestItem(item);

        if (shouldHaveButton) {
            if (!currentWidget) { // Add button only if one doesn't exist
                QToolButton* addButton = new QToolButton();
                addButton->setIcon(QIcon(":/icons/add.png")); // Ensure this icon exists
                QString itemType = item->data(0, Qt::UserRole).toString();
                addButton->setToolTip(itemType == "array" ? tr("Neues Element zum Array hinzufügen") : tr("Neues Attribut zum Objekt hinzufügen"));
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
            // If currentWidget exists and shouldHaveButton is true, we assume it's the correct button already.
        } else { // Should NOT have a button (either !show or !isLowestCollection)
            if (currentWidget) {
                 treeWidget->removeItemWidget(item, 1);
                 delete currentWidget;
            }
        }
        ++it;
    }
}

void InfoPanel::onAddArrayItem() {
    if (!inEditMode) return; // Only allow adding items in edit mode

    QToolButton* button = qobject_cast<QToolButton*>(sender());
    if (!button) return;

    QTreeWidgetItem* parentArrayItem = static_cast<QTreeWidgetItem*>(button->property("treeItem").value<void*>());
    if (!parentArrayItem) return;

    int newIndex = parentArrayItem->childCount();
    QString newItemKey = QString("[%1]").arg(newIndex);

    QTreeWidgetItem* newItem = new QTreeWidgetItem(parentArrayItem);
    newItem->setText(0, newItemKey);
    newItem->setText(1, ""); // Default empty value
    newItem->setData(0, Qt::UserRole, "leaf");

    int depth = getItemDepth(newItem);
    QFont itemFont = calculateFontForDepth(depth);
    newItem->setFont(0, itemFont);
    newItem->setFont(1, itemFont);

    if (inEditMode) {
        setTreeItemEditable(newItem, true);
        treeWidget->setCurrentItem(newItem, 1);
        treeWidget->editItem(newItem, 1);
    }
    onItemChanged(newItem, 1); // Notify that data changed
}

void InfoPanel::onAddObjectItem() {
    if (!inEditMode) return; // Only allow adding items in edit mode

    QToolButton* button = qobject_cast<QToolButton*>(sender());
    if (!button) return;

    QTreeWidgetItem* parentObjectItem = static_cast<QTreeWidgetItem*>(button->property("treeItem").value<void*>());
    if (!parentObjectItem) return;

    bool ok;
    QString newKey = QInputDialog::getText(this, tr("Neuer Schlüssel für Objekt"),
                                           tr("Schlüsselname:"), QLineEdit::Normal,
                                           "", &ok);
    if (ok && !newKey.isEmpty()) {
        for (int i = 0; i < parentObjectItem->childCount(); ++i) {
            if (parentObjectItem->child(i)->text(0) == newKey) {
                QMessageBox::warning(this, tr("Schlüssel existiert bereits"),
                                     tr("Ein Attribut mit dem Schlüssel '%1' existiert bereits in diesem Objekt.").arg(newKey));
                return;
            }
        }

        QTreeWidgetItem* newItem = new QTreeWidgetItem(parentObjectItem);
        newItem->setText(0, newKey);
        newItem->setText(1, ""); // Default empty value
        newItem->setData(0, Qt::UserRole, "leaf");

        int depth = getItemDepth(newItem);
        QFont itemFont = calculateFontForDepth(depth);
        newItem->setFont(0, itemFont);
        newItem->setFont(1, itemFont);

        if (inEditMode) {
            setTreeItemEditable(newItem, true);
            treeWidget->setCurrentItem(newItem, 1);
            treeWidget->editItem(newItem, 1);
        }
        onItemChanged(newItem, 1); // Notify that data changed
    }
}

QFont InfoPanel::calculateFontForDepth(int depth) const {
    double scaledSize = static_cast<double>(m_baseFontSize);
    if (depth > 0) { // Apply scaling factor for items deeper than top-level
        scaledSize *= std::pow(m_fontScaleFactor, depth);
    }

    // Clamp the font size
    double finalSize = std::max(static_cast<double>(m_minFontSize),
                                std::min(scaledSize, static_cast<double>(m_maxFontSize)));

    QFont font = treeWidget->font(); // Start with the tree widget's default font to inherit family etc.
    font.setPointSizeF(finalSize);   // Use setPointSizeF for potentially fractional sizes
    return font;
}

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

bool InfoPanel::isItemDeletable(QTreeWidgetItem* item) const {
    if (!inEditMode || !item || !item->parent()) {
        return false;
    }
    // Only "leaf" items (individual values) are deletable via this hover mechanism.
    if (item->data(0, Qt::UserRole).toString() != "leaf") {
        return false;
    }
    // The parent of this leaf item must be a "lowest collection" (a simple array or object).
    return isLowestCollection(item->parent()) && !isHighestItem(item->parent());
}

QRect InfoPanel::getDeleteButtonRectForItem(const QTreeWidgetItem* item, const QStyleOptionViewItem& option) const {
    Q_UNUSED(item); // Item might be used for more specific sizing in the future
    // Place button at the far right of the item's visual rect for column 1
    // We need the visual rect for column 1. The 'option.rect' is for the whole row if columns are not individually handled by delegate.
    // For simplicity, let's place it at the end of option.rect for now.
    // A more precise way would be to get column 1's rect: treeWidget->visualRect(treeWidget->indexFromItem(item, 1))
    // However, option.rect is what the delegate's paint method gets.
    
    int buttonSize = 16; // Size of the delete button
    // Position it vertically centered, and to the far right of the item's area
    // A small margin from the right edge
    int margin = 2; 
    // Ensure we are using the rect for the correct column if possible, or the whole item rect
    QRect itemRect = option.rect; // This is the rect for the cell being painted by the delegate

    return QRect(itemRect.right() - buttonSize - margin,
                 itemRect.top() + (itemRect.height() - buttonSize) / 2,
                 buttonSize, buttonSize);
}

void InfoPanel::paintDeleteItemButton(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) {
    if (index.isValid() && index == hoveredItemForDelete) {
        QTreeWidgetItem* item = static_cast<QTreeWidgetItem*>(index.internalPointer());
        if (isItemDeletable(item)) {
            QRect buttonRect = getDeleteButtonRectForItem(item, option);
            painter->drawPixmap(buttonRect, deleteIcon);
        }
    }
}

void InfoPanel::handleDeleteAction(QTreeWidgetItem* itemToDelete) {
    if (!itemToDelete || !itemToDelete->parent()) return;

    QTreeWidgetItem* parentItem = itemToDelete->parent();
    int removedAtIndex = parentItem->indexOfChild(itemToDelete); // Get index before removing

    parentItem->removeChild(itemToDelete);
    delete itemToDelete; // Crucial: actually delete the item

    // If parent was an array, re-index subsequent siblings
    if (parentItem->data(0, Qt::UserRole).toString() == "array") {
        for (int i = 0; i < parentItem->childCount(); ++i) {
            // Only re-index items that were at or after the removed item's original position
            // This loop re-indexes all, which is fine and simpler.
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

bool InfoPanel::eventFilter(QObject* watched, QEvent* event) {
    if (watched == treeWidget->viewport()) {
        if (!inEditMode) { // Only handle hover/delete actions in edit mode
            if (hoveredItemForDelete.isValid()) { // Clear hover if exiting edit mode while hovering
                QModelIndex oldHover = hoveredItemForDelete;
                hoveredItemForDelete = QPersistentModelIndex();
                treeWidget->update(oldHover);
            }
            return QWidget::eventFilter(watched, event);
        }

        if (event->type() == QEvent::MouseMove || event->type() == QEvent::HoverMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QModelIndex index = treeWidget->indexAt(mouseEvent->pos());
            QTreeWidgetItem* item = nullptr;
            if (index.isValid()) {
                item = static_cast<QTreeWidgetItem*>(index.internalPointer());
            }

            QModelIndex oldHoveredIndex = hoveredItemForDelete;
            QPersistentModelIndex newHoverPersistentIndex; // Default to invalid

            if (item && isItemDeletable(item)) {
                newHoverPersistentIndex = index;
            }
            // else: item is not deletable or index is invalid, newHoverPersistentIndex remains invalid

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
        else if (event->type() == QEvent::HoverLeave || event->type() == QEvent::Leave) {
             if (hoveredItemForDelete.isValid()) {
                QModelIndex oldHover = hoveredItemForDelete;
                hoveredItemForDelete = QPersistentModelIndex();
                treeWidget->update(oldHover); // Repaint to remove button
            }
            return true; // Event handled
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            if (hoveredItemForDelete.isValid()) {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                QTreeWidgetItem* item = static_cast<QTreeWidgetItem*>(hoveredItemForDelete.internalPointer());
                // Need QStyleOptionViewItem for the hovered item to get its rect
                QStyleOptionViewItem option;
                option.rect = treeWidget->visualRect(hoveredItemForDelete); // Get rect for the specific cell
                // If the delegate is per-column, this might need to be index for column 1.
                // For simplicity, assuming option.rect from visualRect(hoveredItemForDelete) is sufficient.

                QRect buttonRect = getDeleteButtonRectForItem(item, option);
                if (buttonRect.contains(mouseEvent->pos())) {
                    handleDeleteAction(item);
                    // hoveredItemForDelete will be cleared in handleDeleteAction if it was the one deleted
                    return true; // Event handled
                }
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}