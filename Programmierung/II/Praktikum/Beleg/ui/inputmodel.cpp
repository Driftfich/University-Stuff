#include "inputmodel.h"

InputModel::InputModel(QObject *parent)
    : QStandardItemModel(parent) {
    setColumnCount(2);
    setHorizontalHeaderLabels({tr("Attribut"), tr("Wert")});
}

InputModel::~InputModel() {
    // Destructor
}

void InputModel::configureFromSource(QAbstractTableModel* sourceModel) {
    if (!sourceModel) {
        qWarning() << "InputModel::configureFromSource - sourceModel is null.";
        return;
    }

    // Clear the model
    clear();

    // Set the column count and header labels (wird bereits im Konstruktor gemacht, aber hier zur Sicherheit)
    setColumnCount(2);
    setHorizontalHeaderLabels({tr("Attribut"), tr("Wert")});

    // Populate the model with the attributes from the source model
    for (int i = 0; i < sourceModel->columnCount(QModelIndex()); ++i) {
        QString attributeName = sourceModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
        QStandardItem* attributeItem = new QStandardItem(attributeName);
        attributeItem->setEditable(false); // Attributnamen sind normalerweise nicht editierbar

        // Holen Sie die Metadaten für die Spalte aus dem Quellmodell
        QVariant typeVariant = sourceModel->headerData(i, Qt::Horizontal, FormTreeModel::ModelRoles::TypeRole); // Verwenden Sie Ihre Enum
        QVariant optionsVariant = sourceModel->headerData(i, Qt::Horizontal, FormTreeModel::ModelRoles::OptionsRole);
        bool isList = sourceModel->headerData(i, Qt::Horizontal, FormModelRoles::ModelRoles::IsListRole).toBool();

        QStandardItem* valueItem = new QStandardItem();

        // Speichern Sie die Metadaten auch im Attribut-Item, falls der Delegate sie benötigt,
        // um den richtigen Editor für das valueItem zu erstellen.
        attributeItem->setData(typeVariant, FormTreeModel::ModelRoles::TypeRole);
        attributeItem->setData(optionsVariant, FormTreeModel::ModelRoles::OptionsRole);
        attributeItem->setData(isList, FormTreeModel::ModelRoles::IsListRole);


        if (isList) {
            valueItem->setText(tr("[Liste]")); // Platzhaltertext für Listen
            valueItem->setEditable(false); // Das Haupt-Listenitem ist nicht direkt editierbar
                                         // Kind-Items werden für die eigentlichen Werte hinzugefügt
            // Hier könnten Sie bereits leere Kind-Items oder einen "Hinzufügen"-Platzhalter einfügen
            // QStandardItem *addPlaceholder = new QStandardItem(tr("Neuen Eintrag hinzufügen..."));
            // addPlaceholder->setEditable(false);
            // attributeItem->appendRow({nullptr, addPlaceholder}); // In Spalte 1 des Kindes
        } else {
            valueItem->setEditable(true); // Normale Felder sind editierbar
            if (typeVariant.isValid()) {
                QVariant::Type type = static_cast<QVariant::Type>(typeVariant.toInt()); // Konvertieren Sie den int aus der Rolle zurück zum Typ

                // Setzen Sie einen initialen Wert basierend auf dem Typ
                switch (type) {
                    case QVariant::Date:
                        valueItem->setData(QDate::currentDate(), Qt::EditRole);
                        break;
                    case QVariant::DateTime:
                        valueItem->setData(QDateTime::currentDateTime(), Qt::EditRole);
                        break;
                    case QVariant::Int:
                        valueItem->setData(0, Qt::EditRole);
                        break;
                    case QVariant::Double:
                        valueItem->setData(0.0, Qt::EditRole);
                        break;
                    case QVariant::Bool:
                        valueItem->setData(false, Qt::EditRole);
                        // Für Booleans möchten Sie vielleicht eine CheckBox (Delegate)
                        break;
                    case QVariant::String:
                    default:
                        valueItem->setData("", Qt::EditRole); // Leerer String als Standard
                        break;
                }
            } else {
                 valueItem->setData("", Qt::EditRole); // Fallback, falls kein Typ definiert ist
            }
        }
        appendRow({attributeItem, valueItem});
    }
}

