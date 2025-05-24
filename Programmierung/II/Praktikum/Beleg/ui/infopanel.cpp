#include <QWidget>
#include <QTreeWidget>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QIcon>
#include <QPushButton>
#include <QMessageBox>
#include "infopanel.h"

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
    for (const QString& key : jsonObject.keys()) {
        QTreeWidgetItem *topItem = new QTreeWidgetItem(treeWidget, {key, ""});
        topItem->setFirstColumnSpanned(true); // Nur der Schlüssel wird angezeigt
        addJsonToTree(jsonObject[key], topItem);
        treeWidget->addTopLevelItem(topItem);
    }
    
    treeWidget->expandAll();
    treeWidget->resizeColumnToContents(0);
    treeWidget->resizeColumnToContents(1);

    // Initial nicht editierbar
    setTreeItemsEditable(false);
}

void InfoPanel::addJsonToTree(const QJsonValue& value, QTreeWidgetItem* parent) {
    if (value.isObject()) {
        // Rekursion für verschachtelte Objekte
        QJsonObject obj = value.toObject();
        for (const QString& key : obj.keys()) {
            QTreeWidgetItem* child = new QTreeWidgetItem(parent, {key, ""});
            addJsonToTree(obj[key], child);
        }
    } else if (value.isArray()) {
        // Arrays als nummerierte Einträge anzeigen
        QJsonArray array = value.toArray();
        for (int i = 0; i < array.size(); ++i) {
            QTreeWidgetItem* child = new QTreeWidgetItem(parent, {QString("[%1]").arg(i), ""});
            addJsonToTree(array[i], child);
        }
    } else {
        // Einfache Werte direkt in die zweite Spalte
        parent->setText(1, value.toVariant().toString());
        // Markiere als Blatt-Knoten (editierbar)
        parent->setData(0, Qt::UserRole, "leaf");
    }
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
    
    if (isLeaf && editable) {
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
    if (!inEditMode || column != 1) return;
    
    // Save-Button aktivieren bei Änderungen
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
    if (item->childCount() == 0) {
        // Blatt-Knoten: direkter Wert
        QString text = item->text(1);
        
        // Versuche verschiedene Datentypen zu erkennen
        bool ok;
        int intVal = text.toInt(&ok);
        if (ok) return intVal;
        
        double doubleVal = text.toDouble(&ok);
        if (ok) return doubleVal;
        
        if (text.toLower() == "true") return true;
        if (text.toLower() == "false") return false;
        
        return text; // String als Fallback
    } else {
        // Prüfe ob es ein Array oder Objekt ist
        bool isArray = true;
        for (int i = 0; i < item->childCount(); ++i) {
            QString childKey = item->child(i)->text(0);
            if (!childKey.startsWith("[") || !childKey.endsWith("]")) {
                isArray = false;
                break;
            }
        }
        
        if (isArray) {
            QJsonArray array;
            for (int i = 0; i < item->childCount(); ++i) {
                array.append(getValueFromItem(item->child(i)));
            }
            return array;
        } else {
            QJsonObject obj;
            for (int i = 0; i < item->childCount(); ++i) {
                QTreeWidgetItem* child = item->child(i);
                QString key = child->text(0);
                obj[key] = getValueFromItem(child);
            }
            return obj;
        }
    }
}

void InfoPanel::restoreOriginalData() {
    // Einfach die ursprünglichen Daten neu anzeigen
    displayInfo(originalData);
}