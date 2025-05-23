#include <QWidget>
#include <QTreeWidget>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QJsonArray>
#include "infopanel.h"

InfoPanel::InfoPanel(QWidget *parent) : QWidget(parent), treeWidget(new QTreeWidget(this)) {
    treeWidget->setHeaderLabels({tr("Attribut"), tr("Wert")});
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(treeWidget);
}

InfoPanel::~InfoPanel() {
    delete treeWidget;
}

void InfoPanel::displayInfo(const QJsonObject& jsonObject) {
    treeWidget->clear();
    
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
    }
}