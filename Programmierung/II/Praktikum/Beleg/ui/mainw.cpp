#include <QJsonObject>
#include <QDebug>
#include <QMainWindow>
#include <QDockWidget>
#include <QMessageBox>
#include <QCompleter>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QList>
#include <QStringListModel>
#include <QTableView>
#include <QTabWidget>
#include <iostream>

#include "custfiltproxmodel.h"
#include "mainw.h"
#include "infopanel.h"
#include "library.h"
#include "persontablemodel.h"
#include "libitemtablemodel.h"
#include "transactiontablemodel.h"
#include "toolbar.h"
#include "jsonschemautils.h"


void MainWindow::setupUi()
{
    // 1) Eigenes Central Widget erstellen und setzen
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // 2) Layout jetzt auf das Central Widget anwenden
    mainLayout = new QVBoxLayout(central);
    mainLayout->setObjectName("mainLayout");

    // 3)
    setWindowTitle("Library Management System");
    setMinimumSize(1200, 600);
    setWindowIcon(QIcon(":/icons/lib.png"));

    // ---- toolbarUi als echtes QWidget ----
    toolbarUi    = new Ui_toolbar();
    toolbarWidget = new QWidget(central);
    toolbarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    toolbarWidget->setMaximumHeight(150);
    toolbarUi->setupUi(toolbarWidget);
    mainLayout->addWidget(toolbarWidget);

    // ---- TableWidget als echtes QWidget ----
    tableWidgetUi     = new Ui_TableWidget();
    tableWidgetWidget = new QWidget(central);
    tableWidgetUi->setupUi(tableWidgetWidget);
    mainLayout->addWidget(tableWidgetWidget);
}

void MainWindow::updateSubclassType(QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue)
{
    qDebug() << "Field changed:" << fieldName << "from" << oldValue.toString() << "to" << newValue.toString();
    // get the index of the current active tab
    int currentIndex = tableWidgetUi->TabSelector->currentIndex();
    
    if (fieldName != "subclass_type" || 
        oldValue == newValue || 
        currentIndex != 1 || 
        oldValue.toString().isEmpty()) {
        return;
    }

    // get the updated json object from the info panel
    QJsonObject modifiedData = infoPanel->collectDataFromTree();
    QJsonObject mediaBaseData = modifiedData.value("media").toObject().value("media").toObject();
    
    // get the new matching default json object and default schema from the libitemModel
    QJsonObject defaultJson = libitemModel->getDefaultJsonObject(newValue.toString());
    QJsonObject defaultMediaJson = defaultJson.value("media").toObject();
    defaultMediaJson["subclass_type"] = newValue.toString();
    defaultMediaJson["media"] = mediaBaseData; // Preserve the base media data
    defaultJson["media"] = defaultMediaJson; // Update the media object with the new subclass_type
    QJsonObject defaultSchema = libitemModel->getDefaultSchema(newValue.toString());
    
    // disconnect the fieldChanged signal to prevent recursion
    disconnect(infoPanel, &InfoPanel::fieldChanged, this, nullptr);

    // update the info panel with the new default json object and schema
    qDebug() << "=== UPDATING THE INFO PANEL ===";
    infoPanel->displayInfo(defaultJson, defaultSchema, false);
    infoPanel->enterEditMode();
    qDebug() << "=== Updated info panel with new subclass_type:" << newValue.toString() << "===";

    // connect again the fieldChanged signal
    connect(infoPanel, &InfoPanel::fieldChanged, this, 
        [this](QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue) {
            // Check if the changed field is subclass_type
            updateSubclassType(item, column, fieldName, oldValue, newValue);
        });
    // QTimer::singleShot(0, this, [this, newValue]() {
    // });
}

void MainWindow::setupSideDock()
{
    // Side-Panel instanziieren
    sidePanelWidget = new QWidget(this);
    sidePanelUi     = new Ui::Form();
    sidePanelUi->setupUi(sidePanelWidget);

    // InfoPanel in die infopanel-Seite einfügen
    infoPanel = new InfoPanel(sidePanelUi->infopanel);
    auto *infoLayout = new QVBoxLayout(sidePanelUi->infopanel);
    infoLayout->setContentsMargins(0,0,0,0);
    infoLayout->addWidget(infoPanel);

    // Dock erstellen und einhängen
    sideDock = new QDockWidget(tr("Seitenpanel"), this);
    sideDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    sideDock->setWidget(sidePanelWidget);
    addDockWidget(Qt::RightDockWidgetArea, sideDock);
    sideDock->hide(); // startet ausgeblendet

    // Signal vom InfoPanel verbinden
    connect(infoPanel, &InfoPanel::saveRequested, this, &MainWindow::saveModifiedData);
    
    // Connect to field change signal to detect subclass_type changes
    connect(infoPanel, &InfoPanel::fieldChanged, this, 
        [this](QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue) {
            // Check if the changed field is subclass_type
            updateSubclassType(item, column, fieldName, oldValue, newValue);
        });
    
    // Doppelklick auf Tabelleintrag zeigt Info-Panel
    auto showInfo = [this](const QModelIndex& index) {
        // 1) sender() ist das QTableView, das doubleClicked gefeuert hat
        auto *view = qobject_cast<QTableView*>(sender());
        if (!view) return;

        // 2) Proxy-Model holen
        auto *proxy = qobject_cast<QSortFilterProxyModel*>(view->model());
        if (!proxy) return;

        // 3) Source-Model holen
        auto *srcModel = qobject_cast<QAbstractTableModel*>(proxy->sourceModel());
        if (!srcModel) return;

        // 4) auf Source-Index mappen
        QModelIndex srcIndex = proxy->mapToSource(index);

        // Aktuelle Bearbeitung tracken
        currentEditModel = srcModel;
        currentEditIndex = srcIndex;

        // 5) JSON holen je nach Model-Typ
        QJsonObject info;
        QJsonObject schema = QJsonObject(); // Schema-Objekt, falls verfügbar
        if (auto *tm = qobject_cast<TransactionTableModel*>(srcModel))
            {info = tm->getJsonObject(srcIndex);
            schema = tm->getSchemaObject(srcIndex);}
        else if (auto *pm = qobject_cast<PersonTableModel*>(srcModel))
            {info = pm->getJsonObject(srcIndex, lib->getTransactionManager());
            schema = pm->getSchemaObject(srcIndex);}
        else if (auto *lm = qobject_cast<LibItemTableModel*>(srcModel))
            {info = lm->getJsonObject(srcIndex);
            schema = lm->getSchemaObject(srcIndex);}

        // 6) anzeigen
        // if schema available use it
        if (!schema.isEmpty()) {
            // qDebug() << "Using schema:\n" << schema;
            // qDebug() << "Displaying info:\n" << info;
            infoPanel->displayInfo(info, schema, true);
        } else {
            infoPanel->displayInfo(info, true);
        }
        // infoPanel->displayInfo(info);
        sidePanelUi->stackedWidget->setCurrentWidget(sidePanelUi->infopanel);
        sideDock->show();
        // change the title of the side dock
        sideDock->setWindowTitle(tr("Item Information"));
    };

    for (auto *tv : { tableWidgetUi->persontab,
                tableWidgetUi->itemtab,
                tableWidgetUi->transtab })
    connect(tv, &QTableView::doubleClicked, this, showInfo);
}

void MainWindow::setupAddPanel()
{
    addPanel = new InfoPanel(sidePanelUi->addpanel);
    auto *addLayout = new QVBoxLayout(sidePanelUi->addpanel);
    addLayout->addWidget(addPanel);
    addPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // enter Edit Mode directly at beginning
    addPanel->enterEditMode();

    // Button aus toolbarUi zeigt Add-Panel
    connect(toolbarUi->add, &QPushButton::clicked, this, [=]() {
        sidePanelUi->stackedWidget->setCurrentWidget(sidePanelUi->addpanel);
        sideDock->show();

        // set the default json object for the add panel based on the current tab
        int currentIndex = tableWidgetUi->TabSelector->currentIndex();
        QJsonObject defaultJson;
        QJsonObject schemaJson = QJsonObject(); // Schema object, if available
        switch(currentIndex) {
            case 0: // Person tab
                defaultJson = personModel->getDefaultJsonObject();
                schemaJson = personModel->getDefaultSchema();
                break;
            case 1: // LibItem tab  
                defaultJson = libitemModel->getDefaultJsonObject();
                schemaJson = libitemModel->getDefaultSchema(); // No schema for LibItem yet
                break;
            case 2: // Transaction tab
                defaultJson = transactionModel->getDefaultJsonObject();
                schemaJson = transactionModel->getDefaultSchema();
                break;
            default:
                return; // No valid tab selected
        }
        // qDebug() << schemaJson;
        // set the default json object for the add panel
        addPanel->displayInfo(defaultJson, schemaJson, true);
        addPanel->enterEditMode();
        // change the title of the side dock
        sideDock->setWindowTitle(tr("Add New Item"));
    });

    // when the save button in the add panel is clicked, save the new item, enter edit mode again
    connect(addPanel, &InfoPanel::saveRequested, this, [=](const QJsonObject& modifiedData) {
        // depending on the current tab, save the data to the correct model
        int currentIndex = tableWidgetUi->TabSelector->currentIndex();
        qDebug() << "Modified Data:" << modifiedData;
        switch(currentIndex) {
            case 0: // Person tab
                personModel->saveFromJsonObject(modifiedData);
                break;
            case 1: // LibItem tab  
                // libitemModel->saveFromJsonObject(modifiedData);
                break;
            case 2: // Transaction tab
                transactionModel->saveFromJsonObject(modifiedData);
                break;
            default:
                return; // No valid tab selected
        }

        // reset the add panel and enter edit mode again
        emit tableWidgetUi->TabSelector->currentChanged(currentIndex);
    });

    // connection: when the cancel button in the add panel is clicked, enter Edit Mode again
    connect(addPanel, &InfoPanel::editModeCancelled, addPanel, &InfoPanel::enterEditMode);

    // when the tab is changed, the default json object for the current tab should be set
    connect(tableWidgetUi->TabSelector, &QTabWidget::currentChanged, this, [=]() {
        // get the current index of the tab
        int currentIndex = tableWidgetUi->TabSelector->currentIndex();
        // depending on the index, set the default json object for the add panel
        QJsonObject defaultJson;
        QJsonObject schemaJson = QJsonObject(); // Schema object, if available
        switch(currentIndex) {
            case 0: // Person tab
                defaultJson = personModel->getDefaultJsonObject();
                schemaJson = personModel->getDefaultSchema();
                break;
            case 1: // LibItem tab  
                defaultJson = libitemModel->getDefaultJsonObject();
                schemaJson = libitemModel->getDefaultSchema(); // No schema for LibItem yet
                break;
            case 2: // Transaction tab
                defaultJson = transactionModel->getDefaultJsonObject();
                schemaJson = transactionModel->getDefaultSchema();
                break;
            default:
                return; // No valid tab selected
        }

        // set the default json object for the add panel
        addPanel->displayInfo(defaultJson, schemaJson, true);
        addPanel->enterEditMode();

        // // change the title of the side dock
        // sideDock->setWindowTitle(tr("Add New Item"));
    });
    
}

void MainWindow::setupDataLayers()
{
    setupLib();
    setupTableModels();
    setupProxyModels();
}

void MainWindow::setupLib()
{
    lib = new Library(QCoreApplication::applicationDirPath());
    // load happens directly in the constructor of Library
}

void MainWindow::setupTableModels()
{
    // create table models
    personModel = new PersonTableModel(lib->getPersonManager(), this);
    libitemModel = new LibItemTableModel(lib->getLibitemManager(), lib->getMediaManager(), this);
    transactionModel = new TransactionTableModel(lib->getTransactionManager(), 
                                                 lib->getPersonManager(), 
                                                 lib->getLibitemManager(), 
                                                 lib->getMediaManager(), this);

}

void MainWindow::setupProxyModels()
{
    // create proxy models
    personProxy = new CustomFilterProxyModel(this);
    personProxy->setSourceModel(personModel);
    personProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    personProxy->setDynamicSortFilter(true);

    libitemProxy = new CustomFilterProxyModel(this);
    libitemProxy->setSourceModel(libitemModel);
    libitemProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    libitemProxy->setDynamicSortFilter(true);

    transactionProxy = new CustomFilterProxyModel(this);
    transactionProxy->setSourceModel(transactionModel);
    transactionProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    transactionProxy->setDynamicSortFilter(true);

    // set the proxy models to the table views
    tableWidgetUi->persontab->setModel(personProxy);
    tableWidgetUi->persontab->setSortingEnabled(true);
    tableWidgetUi->persontab->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetUi->persontab->setSelectionMode(QAbstractItemView::MultiSelection);
    tableWidgetUi->persontab->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidgetUi->persontab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    tableWidgetUi->itemtab->setModel(libitemProxy);
    tableWidgetUi->itemtab->setSortingEnabled(true);
    tableWidgetUi->itemtab->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetUi->itemtab->setSelectionMode(QAbstractItemView::MultiSelection);
    tableWidgetUi->itemtab->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidgetUi->itemtab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    tableWidgetUi->transtab->setModel(transactionProxy);
    tableWidgetUi->transtab->setSortingEnabled(true);
    tableWidgetUi->transtab->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetUi->transtab->setSelectionMode(QAbstractItemView::MultiSelection);
    tableWidgetUi->transtab->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidgetUi->transtab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


}

void MainWindow::setupToolbarConnections()
{
    setupColumnsConnections();
    setupSortConnections();
    setupSearchConnections();
    setupDeleteConnections();
    setupAddConnections();

    emit tableWidgetUi->TabSelector->currentChanged(0);
}

void MainWindow::setupColumnsConnections()
{
    // 1) When the tab is changed, the columns dropdown should be updated to the current model columns
    QObject::connect(tableWidgetUi->TabSelector, &QTabWidget::currentChanged, [&]() {
        // get the current active abstract table model
        std::variant<PersonTableModel*,
               LibItemTableModel*,
               TransactionTableModel*> model;
        switch(tableWidgetUi->TabSelector->currentIndex()) {
            case 0: model = personModel;      break;
            case 1: model = libitemModel;     break;
            case 2: model = transactionModel; break;
            default: return;
        }
        toolbarUi->setColumns(model);

    });

    // use the dropdown selection change signal to uncheck/check this item in the columns dropdown
    QObject::connect(toolbarUi->columns->view(), &QAbstractItemView::pressed,
                     [&](const QModelIndex &index) {
        // get the current index of the selected item
        // int index = toolbarUi->columns->currentIndex();
        // change the check state of the item
        // if (toolbarUi->columns->itemData(index, Qt::CheckStateRole).toBool()) {
        //     toolbarUi->columns->setItemData(index, Qt::Unchecked, Qt::CheckStateRole);
        // } else {
        //     toolbarUi->columns->setItemData(index, Qt::Checked, Qt::CheckStateRole);
        // }
        std::variant<PersonTableModel*,
        LibItemTableModel*,
        TransactionTableModel*> model;
        switch(tableWidgetUi->TabSelector->currentIndex()) {
            case 0: model = personModel;      break;
            case 1: model = libitemModel;     break;
            case 2: model = transactionModel; break;
            default: return;
        }
        
        QComboBox *combo = toolbarUi->columns;
        QAbstractItemModel *m = combo->model();
        Qt::CheckState cs = static_cast<Qt::CheckState>(
            m->data(index, Qt::CheckStateRole).toInt());
            m->setData(index,
                (cs == Qt::Checked ? Qt::Unchecked : Qt::Checked),
                Qt::CheckStateRole);
        combo->showPopup();

        // update the columns in the current model
        std::visit([&](auto* m) {
            // Hier rufen wir die andere setColumns-Methode auf, die mit QStringList arbeitet
            m->setDisplayedColumns(toolbarUi->getCheckedColumns());
        }, model);
    });
}

void MainWindow::setupSortConnections()
{
    // 2.1) When the tab is changed, the sort dropdown should be updated to the current model columns
    QObject::connect(tableWidgetUi->TabSelector, &QTabWidget::currentChanged, [&]() {
        // get the current active abstract table model
        std::variant<PersonTableModel*,
               LibItemTableModel*,
               TransactionTableModel*> model;
        switch(tableWidgetUi->TabSelector->currentIndex()) {
            case 0: model = personModel;      break;
            case 1: model = libitemModel;     break;
            case 2: model = transactionModel; break;
            default: return;
        }

        // get the previous active sort column from the current selected tab proxy model
        int sortColumn = 0;
        switch(tableWidgetUi->TabSelector->currentIndex()) {
            case 0: sortColumn = personProxy->sortColumn();      break;
            case 1: sortColumn = libitemProxy->sortColumn();     break;
            case 2: sortColumn = transactionProxy->sortColumn(); break;
            default: return;
        }

        // update the sort dropdown with the current model columns
        toolbarUi->setSortColumns(model);
        
        // update the active sort column to the last used sort column
        toolbarUi->sort->setCurrentIndex(sortColumn);
    });

    // 2.2) When the sort dropdown is changed, the current proxy model should be sorted by the selected column
    QObject::connect(toolbarUi->sort, QOverload<int>::of(&QComboBox::activated), [&](int dropdownIndex) {
        // get the current active proxy model e.g. personProxy, libitemProxy, transactionProxy
        CustomFilterProxyModel *currentProxyModel = nullptr;
        QTableView *currentTableView = nullptr;

        switch(tableWidgetUi->TabSelector->currentIndex()) {
            case 0: 
                currentProxyModel = personProxy;
                currentTableView = tableWidgetUi->persontab;
                break;
            case 1: 
                currentProxyModel = libitemProxy; 
                currentTableView = tableWidgetUi->itemtab;
                break;
            case 2: 
                currentProxyModel = transactionProxy; 
                currentTableView = tableWidgetUi->transtab;
                break;
            default:
                qWarning() << "Ungültiger Tab-Index für Sortierung:" << tableWidgetUi->TabSelector->currentIndex();
                return;
        }

        if (!currentProxyModel || !currentTableView) {
            qWarning() << "Konnte ProxyModel oder TableView für Sortierung nicht ermitteln.";
            return;
        }

        // Daten aus der ComboBox abrufen. Wir erwarten, dass die ColumnIdentity (als int)
        // oder der Spaltenindex des Quellmodells als UserData gespeichert ist.
        // In toolbarUi->setSortColumns wurde die ColumnIdentity als UserData gespeichert.
        int columnToSort = toolbarUi->sort->itemData(dropdownIndex).toInt();
        // Aktuelle Sortierspalte und -reihenfolge des Proxy-Modells abrufen
        int currentSortColumn = currentProxyModel->sortColumn();
        Qt::SortOrder currentSortOrder = currentProxyModel->sortOrder();

        Qt::SortOrder newSortOrder = Qt::AscendingOrder;
        // Wenn dieselbe Spalte erneut ausgewählt wird, die Sortierreihenfolge umkehren
        if (columnToSort == currentSortColumn) {
            newSortOrder = (currentSortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
        }
        
        currentProxyModel->sort(columnToSort, newSortOrder);
        // Optional: Sicherstellen, dass der Header der TableView die Sortierindikatoren aktualisiert
        // Dies geschieht normalerweise automatisch, wenn setSortingEnabled(true) gesetzt ist.
        currentTableView->horizontalHeader()->setSortIndicator(dropdownIndex, newSortOrder);
    });
}

void MainWindow::applySearchFilter()
{
    CustomFilterProxyModel *currentProxyModel = nullptr;

    switch(tableWidgetUi->TabSelector->currentIndex()) {
            case 0: 
                currentProxyModel = personProxy;
                break;
            case 1: 
                currentProxyModel = libitemProxy; 
                break;
            case 2: 
                currentProxyModel = transactionProxy; 
                break;
            default:
                qWarning() << "Ungültiger Tab-Index für Filterung:" << tableWidgetUi->TabSelector->currentIndex();
                return;
        }

    if (!currentProxyModel) {
        qWarning() << "Konnte ProxyModel für Filterung nicht ermitteln.";
        return;
    }

    QString filterString = toolbarUi->searchbar->text();

    currentProxyModel->setFilterKeyColumn(-1);

    currentProxyModel->setSearchString(filterString, " ");
}

void MainWindow::setupSearchConnections()
{
    // 3.1) Wenn der Tab gewechselt wird, Filter anwenden
    QObject::connect(tableWidgetUi->TabSelector, &QTabWidget::currentChanged, [this]()
    {applySearchFilter();});

    // 3.2) Wenn sich der Text im Suchfeld ändert, Filter anwenden
    QObject::connect(toolbarUi->searchbar, &QLineEdit::textChanged, [this](const QString&)
    {applySearchFilter();});
}

void MainWindow::setupDeleteConnections()
{
    // 4) When the deletec button is clicked, the selected rows in the current table view should be deleted
    QObject::connect(toolbarUi->deletec, &QPushButton::clicked, [&]() {
        // Bestimme das aktuelle TableView und ProxyModel
        QTableView *currentTableView = nullptr;
        CustomFilterProxyModel *currentProxyModel = nullptr;
        QAbstractTableModel *currentSourceModel = nullptr;

        switch(tableWidgetUi->TabSelector->currentIndex()) {
            case 0: // Person Tab
                currentTableView = tableWidgetUi->persontab;
                currentProxyModel = personProxy;
                currentSourceModel = personModel;
                break;
            case 1: // LibItem Tab
                currentTableView = tableWidgetUi->itemtab;
                currentProxyModel = libitemProxy;
                currentSourceModel = libitemModel;
                break;
            case 2: // Transaction Tab
                currentTableView = tableWidgetUi->transtab;
                currentProxyModel = transactionProxy;
                currentSourceModel = transactionModel;
                break;
            default:
                qWarning() << "Ungültiger Tab-Index für Löschvorgang:" << tableWidgetUi->TabSelector->currentIndex();
                return;
        }

        if (!currentTableView || !currentProxyModel || !currentSourceModel) {
            qWarning() << "Konnte TableView, ProxyModel oder SourceModel für Löschvorgang nicht ermitteln.";
            return;
        }

        // Hole die ausgewählten Zeilen
        QModelIndexList selectedIndexes = currentTableView->selectionModel()->selectedRows();
        
        if (selectedIndexes.isEmpty()) {
            QMessageBox::information(this, "Keine Auswahl", "Bitte wählen Sie mindestens eine Zeile zum Löschen aus.");
            return;
        }

        // Bestätigungsdialog anzeigen
        QString message = QString("Möchten Sie wirklich %1 Eintrag/Einträge löschen?").arg(selectedIndexes.size());
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Löschen bestätigen", message,
                                                                QMessageBox::Yes | QMessageBox::No);
        
        if (reply != QMessageBox::Yes) {
            return;
        }

        // Konvertiere Proxy-Indizes zu Source-Indizes
        QList<int> sourceRows;
        for (const QModelIndex &proxyIndex : selectedIndexes) {
            QModelIndex sourceIndex = currentProxyModel->mapToSource(proxyIndex);
            sourceRows.append(sourceIndex.row());
        }

        // Sortiere die Zeilen in absteigender Reihenfolge, um beim Löschen keine Indexverschiebungen zu haben
        std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());

        // Lösche die Zeilen aus dem Source-Model
        for (int row : sourceRows) {
            // std::cout << "Deleting row: " << row << std::endl;
            currentSourceModel->removeRow(row);
        }

        // std::cout << "Gelöscht: " << sourceRows.size() << " Einträge" << std::endl;
    });
}

void MainWindow::setupAddConnections()
{}

void MainWindow::saveModifiedData(const QJsonObject& data) {
    if (!currentEditModel || !currentEditIndex.isValid()) {
        QMessageBox::warning(this, tr("Fehler"), tr("Keine gültigen Daten zum Speichern."));
        return;
    }

    // print out the modified data to console
    qDebug() << "Modified Data:" << data << "\n\n";
    
    try {
        // Je nach Model-Typ die entsprechende Speicher-Methode aufrufen
        bool success = false;
        
        if (auto *tm = qobject_cast<TransactionTableModel*>(currentEditModel)) {
            // Sie müssten eine setJsonObject-Methode implementieren oder direkt den Manager aufrufen
            success = tm->updateFromJsonObject(data, currentEditIndex);
        } 
        else if (auto *pm = qobject_cast<PersonTableModel*>(currentEditModel)) {
            success = pm->updateFromJsonObject(data, currentEditIndex);
        } 
        else if (auto *lm = qobject_cast<LibItemTableModel*>(currentEditModel)) {
            success = lm->updateFromJsonObject(data, currentEditIndex);
        }
        
        if (success) {
            // Model über Änderungen benachrichtigen
            currentEditModel->dataChanged(currentEditIndex, currentEditIndex);
            QMessageBox::information(this, tr("Gespeichert"), 
                                   tr("Änderungen wurden erfolgreich gespeichert."));
        } else {
            QMessageBox::warning(this, tr("Fehler"), 
                               tr("Fehler beim Speichern der Änderungen."));
        }
    } 
    catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Fehler"), 
                            tr("Unerwarteter Fehler: %1").arg(e.what()));
    }
}

void MainWindow::setupSearchCompleter()
{
    searchCompleter = new QCompleter(this);
    searchCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    searchCompleter->setFilterMode(Qt::MatchContains);
    searchCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    searchCompleter->setMaxVisibleItems(10);

    // set the origSearchText to an empty string initially for safety
    origSearchText = QString();
    
    connect(searchCompleter, QOverload<const QString&>::of(&QCompleter::highlighted),
            this, [this](const QString& text) {
                MainWindow::onCompleterActivated(text);
            });

    connect(searchCompleter, QOverload<const QString&>::of(&QCompleter::activated),
            this, [this](const QString& text) {
                // overwrite qt internal logic to complete correctly
                MainWindow::onCompleterActivated(text);
                // hide the completer popup after activation
                searchCompleter->popup()->hide();
            });


    // Bei Tab-Wechsel und Textänderung Completer aktualisieren
    connect(tableWidgetUi->TabSelector, &QTabWidget::currentChanged,
            this, [this]() {
                updateSearchCompleter();
                searchCompleter->popup()->hide(); // hide the popup on tab change
            });
    connect(toolbarUi->searchbar, &QLineEdit::textChanged,
            this, &MainWindow::updateSearchCompleter);

    // Initiale Befüllung für das aktuelle Tab
    updateSearchCompleter();
    
    // Completer am Suchfeld anbringen
    toolbarUi->searchbar->setCompleter(searchCompleter);
}

void MainWindow::updateSearchCompleter()
{
    QStringList suggestions;
    int currentTabIdx = tableWidgetUi->TabSelector->currentIndex();

    QTableView* view = nullptr;
    CustomFilterProxyModel* model = nullptr;
    switch (currentTabIdx) {
        case 0:
            view = tableWidgetUi->persontab;
            model = qobject_cast<CustomFilterProxyModel*>(view->model());
            break;
        case 1:
            view = tableWidgetUi->itemtab;
            model = qobject_cast<CustomFilterProxyModel*>(view->model());
            break;
        case 2:
            view = tableWidgetUi->transtab;
            model = qobject_cast<CustomFilterProxyModel*>(view->model());
            break;
        default:
            break;
    }

    if (!model || !view) {
        qDebug() << "No valid model or view found for current tab index:" << currentTabIdx;
        return;
    }

    // update the proxy model with the current search string
    QString filterText = toolbarUi->searchbar->text();
    this->origSearchText = filterText; // store original search text
    QString delimiter = " "; // use space as default delimiter
    model->setSearchString(filterText, delimiter);
    
    // get the current searchtokens from the proxy model
    QList<QRegularExpression> searchTokens = model->getSearchTokens();
    
    // extract all the cells from the current model where these tokens occur
    for (const QRegularExpression& token : searchTokens) {
        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QString text = model->data(model->index(row, col)).toString();
                if (token.match(text).hasMatch() && !suggestions.contains(text) && !filterText.contains(text)) {
                    suggestions << text;
                }
            }
        }
    }

    // sort suggestions by length (shortest first)
    std::sort(suggestions.begin(), suggestions.end(),
              [](const QString& a, const QString& b) {
                  return a.length() < b.length();
              });


    QString lastToken = toolbarUi->searchbar->text().split(' ').last();
    // qDebug() << "Raw suggestions:" << suggestions;
    // qDebug() << "Last token for completer:" << lastToken;
              
    QStringListModel *stringModel = new QStringListModel(suggestions, searchCompleter);
    searchCompleter->setModel(stringModel);
    searchCompleter->setCompletionPrefix(lastToken);
    searchCompleter->complete(); // Update the popup with new suggestions
    searchCompleter->popup()->show();
}

void MainWindow::onCompleterActivated(const QString& suggestion)
{
    // qDebug() << "Completer activated with suggestion:" << suggestion;
    if (!toolbarUi || !toolbarUi->searchbar) {
        qWarning() << "SearchBar UI component not available";
        return;
    }

    if (suggestion.isEmpty()) {
        qDebug() << "Empty suggestion received, ignoring";
        return;
    }

    QString currentText = origSearchText;
    QStringList tokens = currentText.split(' ', Qt::SkipEmptyParts);
    
    // Spezialfall: Text endet mit Leerzeichen -> neuer leerer Token
    if (currentText.endsWith(' ')) {
        tokens.append(QString());
    }
    
    QString newSearchText;
    // qDebug() << "Current search text:" << currentText;
    // qDebug() << "Tokens:" << tokens;
    if (tokens.isEmpty()) {
        // Suchfeld war leer oder enthielt nur Leerzeichen
        newSearchText = suggestion;
        // qDebug() << "Empty search field, setting to:" << suggestion;
    }
    else {
        QString lastToken = tokens.last();
        
        // Prüfe, ob suggestion mit letztem Token beginnt (case-insensitive)
        bool startsWithLastToken = !lastToken.isEmpty() && 
                                  suggestion.startsWith(lastToken, Qt::CaseInsensitive);

        // qDebug() << startsWithLastToken;
        
        if (startsWithLastToken) {
            // Token-Ersetzung: Ersetze nur den letzten Token
            tokens[tokens.size() - 1] = suggestion;
            newSearchText = tokens.join(' ');
            // qDebug() << "Token replacement:" << lastToken << "->" << suggestion;
        }
        else {
            // // Vollständige Ersetzung des Suchtexts
            newSearchText = suggestion;
            // qDebug() << "Complete replacement:" << currentText << "->" << suggestion;

            // Anfügen des neuen Tokens
            // tokens.append(suggestion);
            // newSearchText = tokens.join(' ');
        }
    }
    
    // Text im Suchfeld aktualisieren
    // blockSignals verhindert rekursive updateSearchCompleter-Aufrufe
    toolbarUi->searchbar->blockSignals(true);
    
    // remove the completer temporarily to avoid that the lineedit gets overwritten by internal qt logic
    toolbarUi->searchbar->setCompleter(nullptr);
    
    toolbarUi->searchbar->setText(newSearchText);
    
    // Cursor ans Ende setzen für weitere Eingaben
    toolbarUi->searchbar->setCursorPosition(newSearchText.length());
    
    // Completer wieder anhängen
    toolbarUi->searchbar->setCompleter(searchCompleter);
    // make the popup visible again
    searchCompleter->popup()->show();
    toolbarUi->searchbar->blockSignals(false);
    
    // qDebug() << "Search text updated to:" << newSearchText;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (lib) lib->save(); // Save the library data on close
    QMainWindow::closeEvent(event);
}
