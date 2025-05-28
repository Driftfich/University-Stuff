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
#include "infopanel.h"

class ReadOnlyDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override
    {
        if (index.column() == 0)
            return nullptr;
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
};

InfoPanel::InfoPanel(QWidget *parent) : QWidget(parent), treeWidget(new QTreeWidget(this)), inEditMode(false) {
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

    treeWidget->setItemDelegate(new ReadOnlyDelegate(treeWidget)); 

    // connect signals and slots
    connect(saveButton, &QPushButton::clicked, this, &InfoPanel::saveChanges);
    connect(editButton, &QPushButton::clicked, this, &InfoPanel::enterEditMode);
    connect(treeWidget, &QTreeWidget::itemChanged, this, &InfoPanel::onItemChanged);

}

InfoPanel::~InfoPanel() {
    delete treeWidget;
}

void InfoPanel::displayInfo(const QJsonObject& jsonObject) {
    originalData = jsonObject; // Store original data for canceling edits
    treeWidget->clear();

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
    setTreeItemsEditable(false);
}

// void InfoPanel::addJsonToTree(const QJsonValue& value, QTreeWidgetItem* parent) {
//     if (value.isObject()) {
//         // Rekursion für verschachtelte Objekte
//         parent->setData(0, Qt::UserRole, "object"); // Markiere als Gruppe
//         QJsonObject obj = value.toObject();
//         for (const QString& key : obj.keys()) {
//             QTreeWidgetItem* child = new QTreeWidgetItem(parent, {key, ""});
//             addJsonToTree(obj[key], child);
//         }
//     } else if (value.isArray()) {
//         // Arrays als nummerierte Einträge anzeigen
//         QJsonArray array = value.toArray();
//         for (int i = 0; i < array.size(); ++i) {
//             QTreeWidgetItem* child = new QTreeWidgetItem(parent, {QString("[%1]").arg(i), ""});
//             addJsonToTree(array[i], child);
//         }
//     } else {
//         // Einfache Werte direkt in die zweite Spalte
//         parent->setText(1, value.toVariant().toString());
//         // Markiere als Blatt-Knoten (editierbar)
//         parent->setData(0, Qt::UserRole, "leaf");
//     }
// }

void InfoPanel::addJsonToTreeRecursive(const QJsonValue& valueForThisItem, QTreeWidgetItem* thisItem) {
    if (valueForThisItem.isObject()) {
        thisItem->setData(0, Qt::UserRole, "object");
        QJsonObject obj = valueForThisItem.toObject();
        for (const QString& key : obj.keys()) {
            QTreeWidgetItem* child = new QTreeWidgetItem(thisItem);
            child->setText(0, key);
            addJsonToTreeRecursive(obj[key], child);
        }
    } else if (valueForThisItem.isArray()) {
        thisItem->setData(0, Qt::UserRole, "array");
        QJsonArray array = valueForThisItem.toArray();
        for (int i = 0; i < array.size(); ++i) {
            QTreeWidgetItem* child = new QTreeWidgetItem(thisItem);
            child->setText(0, QString("[%1]").arg(i));
            addJsonToTreeRecursive(array[i], child);
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
}

void InfoPanel::cancelEditMode() {
    // Originaldaten wiederherstellen
    restoreOriginalData();
    
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

    updateAddButtons(false); // Hide add buttons after canceling
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

    if (inEditMode) {
        setTreeItemEditable(newItem, true);
        treeWidget->setCurrentItem(newItem, 1);
        treeWidget->editItem(newItem, 1);
    }
    onItemChanged(newItem, 1); // Notify that data changed
}

void InfoPanel::onAddObjectItem() {
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

        if (inEditMode) {
            setTreeItemEditable(newItem, true);
            treeWidget->setCurrentItem(newItem, 1);
            treeWidget->editItem(newItem, 1);
        }
        onItemChanged(newItem, 1); // Notify that data changed
    }
}