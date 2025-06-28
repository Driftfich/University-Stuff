/*
Author: Franz Rehschuh
Date: 2025-06-28

Description:
The mainw.cpp file contains the implementation of the MainWindow class.
It is responsible for the main window ui and the data management.

*/

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
#include <QDir>
#include <iostream>

#include "custfiltproxmodel.h"
#include "entitycompleter.h"
#include "mainw.h"
#include "infopanel.h"
#include "library.h"
#include "persontablemodel.h"
#include "libitemtablemodel.h"
#include "transactiontablemodel.h"
#include "toolbar.h"
#include "jsonschemautils.h"

#include "media.h"
#include "text.h"
#include "audio.h"
#include "video.h"

#include "returns.h"

/**
 * @brief MainWindow::setupUi
 * @details This method is responsible for setting up the main window ui.
 * It creates the central widget and applies the layout to it.
 * It also sets the window title and icon.
 * It creates the toolbar and table widget and adds them to the main layout.
 */
void MainWindow::setupUi()
{
    // 1) create and set the central widget
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // 2) apply the layout to the central widget
    mainLayout = new QVBoxLayout(central);
    mainLayout->setObjectName("mainLayout");

    // 3) set the window title and icon
    setWindowTitle("Library Management System");
    setMinimumSize(1200, 600);
    setWindowIcon(QIcon(":/icons/lib.png"));

    // ---- toolbarUi as a real QWidget ----
    toolbarUi    = new Ui_toolbar();
    toolbarWidget = new QWidget(central);
    toolbarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    toolbarWidget->setMaximumHeight(150);
    toolbarUi->setupUi(toolbarWidget);
    mainLayout->addWidget(toolbarWidget);

    // ---- TableWidget as a real QWidget ----
    tableWidgetUi     = new Ui_TableWidget();
    tableWidgetWidget = new QWidget(central);
    tableWidgetUi->setupUi(tableWidgetWidget);
    mainLayout->addWidget(tableWidgetWidget);
}

/**
 * @brief MainWindow::setupCompleterForAddpanel
 * @details When a LineEdit is focused in the addPanel, a completer is setup via this handler if the field name is one of the following:
 * libitem_id, borrower_id, media_id, artist_ids
 */
void MainWindow::setupCompleterForAddpanel(QLineEdit* editor, const QModelIndex& index)
{
    // qDebug() << "Setting up completer for editor...";
    
    // get the tree widget item from the index
    QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(const_cast<QAbstractItemModel*>(index.model())->parent());
    if (!treeWidget) return;
    
    // get the tree widget item from the index
    QTreeWidgetItem* item = treeWidget->itemFromIndex(index);
    if (!item) return;
    
    // Get the field name from the tree item's data
    QString fieldName = item->data(0, SchemaOriginalKeyRole).toString();
    // qDebug() << "Field name:" << fieldName;

    // create a new entity completer if the field name is supported
    EntityCompleter* completer = nullptr;
    if (fieldName == "libitem_id") {
        qDebug() << "Setting up completer for libitem_id...";
        QStringList filterColumns = QStringList() << "ID" << "Title" << "Publisher";
        completer = new EntityCompleter(libitemModel, "{ID}", filterColumns, "{Title} - {Publisher} ({ID})\nAvailable Copies: {Available Copies}\nBorrowed Copies: {Borrowed Copies}", ".", editor);
    }
    else if (fieldName == "borrower_id") {
        qDebug() << "Setting up completer for borrower_id...";
        QStringList filterColumns = QStringList() << "ID" << "First Name" << "Last Name" << "Email";
        completer = new EntityCompleter(personModel, "{ID}", filterColumns, "{First Name} {Last Name} ({ID})", ".", editor);
    }
    else if (fieldName == "media_id") {
        qDebug() << "Setting up completer for media_id...";
        QStringList filterColumns = QStringList() << "ID" << "Title" << "Publisher";
        completer = new EntityCompleter(mediaModel, "{ID}", filterColumns, "{Title} - {Publisher} ({ID})", ".", editor);
    }
    // if item parent key is "artist_ids" setup a completer for the given array key
    else if (item->parent()->data(0, SchemaOriginalKeyRole).toString() == "artist_ids") {
        qDebug() << "Setting up completer for artist_ids...";
        QStringList filterColumns = QStringList() << "ID" << "First Name" << "Last Name" << "Email";
        completer = new EntityCompleter(personModel, "{ID}", filterColumns, "{First Name} {Last Name} ({ID})", ".", editor);
    }
    
    if (completer) {
        // configure the completer
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setFilterMode(Qt::MatchContains);
        completer->setWrapAround(false);
        
        // set the completer to the editor
        editor->setCompleter(completer);
        
        // initialize completions
        completer->updateCompletions("");
    }
}

/**
 * @brief MainWindow::setupCompleterForInfoPanel
 * @details When a LineEdit is focused in the infoPanel, a completer is setup via this handler if the field name is one of the following:
 * media_id, artist_ids
 */
void MainWindow::setupCompleterForInfoPanel(QLineEdit* editor, const QModelIndex& index)
{
    // qDebug() << "Setting up completer for editor...";
    
    // get the tree widget item from the index
    QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(const_cast<QAbstractItemModel*>(index.model())->parent());
    if (!treeWidget) return;

    // get the tree widget item from the index
    QTreeWidgetItem* item = treeWidget->itemFromIndex(index);
    if (!item) return;

    // get the field name from the tree item's data
    QString fieldName = item->data(0, SchemaOriginalKeyRole).toString();

    // create a new entity completer if the field name is supported
    EntityCompleter* completer = nullptr;
    if (fieldName == "media_id") {
        qDebug() << "Setting up completer for media_id...";
        QStringList filterColumns = QStringList() << "ID" << "Title" << "Publisher";
        completer = new EntityCompleter(mediaModel, "{ID}", filterColumns, "{Title} - {Publisher} ({ID})", ".", editor);
    }
    // if item parent key is "artist_ids" setup a completer for the given array key
    else if (item->parent()->data(0, SchemaOriginalKeyRole).toString() == "artist_ids") {
        qDebug() << "Setting up completer for artist_ids...";
        QStringList filterColumns = QStringList() << "ID" << "First Name" << "Last Name" << "Email";
        completer = new EntityCompleter(personModel, "{ID}", filterColumns, "{First Name} {Last Name} ({ID})", ".", editor);
    }

    if (completer) {
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setFilterMode(Qt::MatchContains);
        completer->setWrapAround(false);
        editor->setCompleter(completer);
        completer->updateCompletions("");
    }
}

/**
 * @details When the media id is changed in the given panel, the media object will be updated in the form by rerendering it.
 */
void MainWindow::changedMediaId(InfoPanel* panel, QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue)
{
    Q_UNUSED(column);
    Q_UNUSED(item);

    // get the current index of the tab selector
    int currentIndex = tableWidgetUi->TabSelector->currentIndex();
    // check that the field name is "media_id" and the current tab is the libitem tab. Also check that the old and new value are not the same.
    if (fieldName != "media_id" || currentIndex != 1 || oldValue.toString() == newValue.toString())
    {
        return;
    }

    // get the original data and schema from the info panel. This will be later set again to allow for correct cancel behavior.
    QJsonObject originalData = panel->getOriginalData();
    QJsonObject originalSchema = panel->getOriginalSchema();
    
    // get the current json and schema from the info panel
    QJsonObject newJson = panel->collectDataFromTree();
    QJsonObject newSchema = originalSchema;

    // try to get the media item from the media manager
    std::shared_ptr<Media> mediaItem = nullptr;
    unsigned long currentIdValue = 0;
    bool ok;
    newValue.toString().toULongLong(&ok);
    if (ok) {
        // get the current id from the item as unsigned long long
        currentIdValue = newValue.toString().toULongLong();
        // try to find the media item with the new id in the media manager
        mediaItem = lib->getMediaManager()->getMedia(currentIdValue);
    }
    else {
        // get the next available id from the media manager to create a new default media object
        currentIdValue = lib->getMediaManager()->getNextId();
    }
    
    // if the media item is not found, a new default media json object will be shown, based on the current media type
    if (!mediaItem) {
        // get the current actice media type from the info panel
        QString mediaType = newJson.value("media").toObject().value("subclass_type").toString();
        // qDebug() << "Media item with id" << currentIdValue << "not found in MediaManager.";

        // get a new default media json object based on the current media type
        QJsonObject mediaJson = libitemModel->getDefaultJsonObject(mediaType).value("media").toObject();
        // update the media id in the default media json object to the choosen one from the libitem
        QJsonObject baseinfo = mediaJson.value("media").toObject();
        baseinfo["id"] = QString::number(currentIdValue);
        mediaJson["media"] = baseinfo;
        newJson["media"] = mediaJson;

        // get the default schema for the media item based current media type
        QJsonObject mediaSchema = libitemModel->getDefaultSchema(mediaType).value("properties").toObject().value("media").toObject();
        QJsonObject properties = newSchema.value("properties").toObject();
        properties["media"] = mediaSchema;
        newSchema["properties"] = properties;
    }
    // if the media item is found, update the original media with the new media json object
    else {
        newJson["media"] = mediaItem->getJson();
        // update the schema for the correct media type
        QJsonObject properties = newSchema.value("properties").toObject();
        QJsonObject mediaSchema = properties.value("media").toObject();
        mediaSchema = mediaItem->getSchemaByType();
        properties["media"] = mediaSchema;
        newSchema["properties"] = properties;
    }

    // update the info panel with the new media json object and schema.
    // use a timer to avoid race conditions when the info panel is updated
    // afterwards the original data and schema are set again to allow for correct cancel behavior
    QTimer::singleShot(0, [this, panel, newJson, newSchema, originalData, originalSchema]() {
        panel->displayInfo(newJson, newSchema, false);
        panel->setOriginalData(originalData);
        panel->setOriginalSchema(originalSchema);
    });
}

/**
 * @brief MainWindow::updateSubclassType
 * @details When the subclass type is changed in the addPanel, the json and schema are updated/rerendered to show the correct fields
 */
void MainWindow::updateSubclassType(QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue)
{
    Q_UNUSED(column);
    Q_UNUSED(item);

    // get the index of the current active tab
    int currentIndex = tableWidgetUi->TabSelector->currentIndex();
    
    // check that the field name is indeed the subclass type and the current tab is the libitem tab.
    // also check that the old and new value are not the same and the old value is not empty
    if (fieldName != "subclass_type" || 
        oldValue.toString() == newValue.toString() || 
        currentIndex != 1 || 
        oldValue.toString().isEmpty()) {
        return;
    }

    // get the current json object from the add panel
    QJsonObject modifiedData = addPanel->collectDataFromTree();
    QJsonObject mediaBaseData = modifiedData.value("media").toObject().value("media").toObject();
    QJsonObject libitemBaseData = modifiedData.value("libitem").toObject();

    // get the original data and schema from the add panel. This will be later set again to allow for correct cancel behavior.
    QJsonObject originalData = addPanel->getOriginalData();
    QJsonObject originalSchema = addPanel->getOriginalSchema();
    
    // get the new default json object based on the new subclass type, here newValue
    QJsonObject defaultJson = libitemModel->getDefaultJsonObject(newValue.toString());
    // extract the media object from the default json object
    QJsonObject defaultMediaJson = defaultJson.value("media").toObject();
    defaultMediaJson["subclass_type"] = newValue.toString(); // overwrite the subclass type
    defaultMediaJson["media"] = mediaBaseData; // Preserve the base media data
    defaultJson["media"] = defaultMediaJson; // Update the media object with the new subclass_type
    defaultJson["libitem"] = libitemBaseData; // Preserve the base libitem data
    QJsonObject defaultSchema = libitemModel->getDefaultSchema(newValue.toString());

    // update the info panel with the new default json object and schema. The edit mode will be not left, as the user is still in the addPanel
    addPanel->displayInfo(defaultJson, defaultSchema, false);
    // set the original data and schema again to allow for correct cancel behavior
    addPanel->setOriginalData(originalData);
    addPanel->setOriginalSchema(originalSchema);
}

/**
 * @brief MainWindow::enabledArtist
 * @details When the artist is enabled in the infoPanel, the artist object will be updated in the form by rerendering it.
 */
void MainWindow::enabledArtist(QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue) {
    Q_UNUSED(column);
    Q_UNUSED(oldValue);
    Q_UNUSED(newValue);
    Q_UNUSED(item);
    
    // get the current index of the tab selector
    int currentIndex = tableWidgetUi->TabSelector->currentIndex();

    // check that the current tab is the person tab and the field name is either artist or borrower
    if ((currentIndex != 0 && currentIndex != 2) ||
        (fieldName != "artist" && fieldName != "borrower")) {
        return;
    }

    // get the original data and schema from the info panel. This will be later set again to allow for correct cancel behavior.
    QJsonObject originalPerson = infoPanel->getOriginalData();
    QJsonObject originalSchema = infoPanel->getOriginalSchema();

    // get the current json object from the info panel
    QJsonObject currentData = infoPanel->collectDataFromTree();
    QJsonObject currentPerson = currentData.value("person").toObject();

    // check if the person is currently an artist or borrower
    bool isArtist = currentPerson.contains("artist") && !currentPerson["artist"].toObject().isEmpty();
    bool isBorrower = currentPerson.contains("borrower") && !currentPerson["borrower"].toObject().isEmpty();

    // get the new default schema for the person object based on the current artist and borrower status
    QJsonObject newSchema = personModel->getDefaultSchema((isArtist || fieldName == "artist"), (isBorrower || fieldName == "borrower"));

    // if the field name is artist and the person is not currently an artist, create a new default artist object
    if (fieldName == "artist" && currentPerson["artist"].toObject().isEmpty()) {
        currentPerson["artist"] = createDefaultJsonFromSchema(Artist::getSubclassSchema(true));
    } 
    else if (!isArtist) currentPerson["artist"] = QJsonObject(); // collectDataFromTree filters unchecked elements -> artist has to be set again to be visible
    if (fieldName == "borrower" && currentPerson["borrower"].toObject().isEmpty()) {
        currentPerson["borrower"] = createDefaultJsonFromSchema(Borrower::getSubclassSchema(true));
    } 
    else if (!isBorrower) currentPerson["borrower"] = QJsonObject(); // collectDataFromTree filters unchecked elements -> borrower has to be set again to be visible

    currentData["person"] = currentPerson; // update the person object in the current data

    // update the info panel with the new default json object and schema. The edit mode will be not left, as the user is still in Edit Mode in the infoPanel
    QTimer::singleShot(0, [this, currentData, newSchema, originalPerson, originalSchema]() {
        infoPanel->displayInfo(currentData, newSchema, false);
        infoPanel->setOriginalData(originalPerson);
        infoPanel->setOriginalSchema(originalSchema);
    });

}

/**
 * @details This method is responsible for setting up the side dock.
 * It creates the side panel widget and adds the info panel to it.
 * It connects the saveRequested signal from the infoPanel to the saveModifiedData method.
 */
void MainWindow::setupSideDock()
{
    // create the side panel widget
    sidePanelWidget = new QWidget(this);
    sidePanelUi     = new Ui::Form();
    sidePanelUi->setupUi(sidePanelWidget);

    // create the info panel and add it to the side panel widget
    infoPanel = new InfoPanel(sidePanelUi->infopanel);
    QVBoxLayout *infoLayout = new QVBoxLayout(sidePanelUi->infopanel);
    infoLayout->setContentsMargins(0,0,0,0);
    infoLayout->addWidget(infoPanel);

    // create the side dock and add it to the main window
    sideDock = new QDockWidget(tr("Seitenpanel"), this);
    sideDock->setAllowedAreas(
        Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    sideDock->setWidget(sidePanelWidget);
    addDockWidget(Qt::RightDockWidgetArea, sideDock);
    sideDock->hide(); // start hidden

    // connect the saveRequested signal from the infoPanel to the saveModifiedData method
    connect(infoPanel, &InfoPanel::saveRequested, this, &MainWindow::saveModifiedData);

    // connect the completer setup signal for infoPanel too
    connect(infoPanel, &InfoPanel::completerSetupRequested, this, &MainWindow::setupCompleterForInfoPanel);

    // try the changeMediaId and enabledArtist on the info panel
    connect(infoPanel, &InfoPanel::fieldChanged, this, [=](QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue) {
        changedMediaId(infoPanel, item, column, fieldName, oldValue, newValue);
        enabledArtist(item, column, fieldName, oldValue, newValue);
    });
    
    // when a table view item is double clicked, the info panel will be shown
    auto showInfo = [this](const QModelIndex& index) {
        // 1) get the sender, which is the QTableView that was double clicked
        QTableView* view = qobject_cast<QTableView*>(sender());
        if (!view) return;

        // 2) get the proxy model
        QSortFilterProxyModel* proxy = qobject_cast<QSortFilterProxyModel*>(view->model());
        if (!proxy) return;

        // 3) get the source model
        QAbstractTableModel* srcModel = qobject_cast<QAbstractTableModel*>(proxy->sourceModel());
        if (!srcModel) return;

        // 4) map the source index
        QModelIndex srcIndex = proxy->mapToSource(index);

        // track the current edit model and index
        currentEditModel = srcModel;
        currentEditIndex = srcIndex;

        // 5) get the json object and schema based on the model type
        QJsonObject info;
        QJsonObject schema = QJsonObject(); // Schema object, if available
        if (TransactionTableModel* tm = qobject_cast<TransactionTableModel*>(srcModel))
            {info = tm->getJsonObject(srcIndex);
            schema = tm->getSchemaObject(srcIndex);}
        else if (PersonTableModel* pm = qobject_cast<PersonTableModel*>(srcModel))
            {info = pm->getJsonObject(srcIndex);
            schema = pm->getSchemaObject(srcIndex);}
        else if (LibItemTableModel* lm = qobject_cast<LibItemTableModel*>(srcModel))
            {info = lm->getJsonObject(srcIndex);
            schema = lm->getSchemaObject(srcIndex);
        }

        // 6) display the info panel
        infoPanel->displayInfo(info, schema, true);
        sidePanelUi->stackedWidget->setCurrentWidget(sidePanelUi->infopanel);
        sideDock->show();
        // change the title of the side dock
        sideDock->setWindowTitle(tr("Item Information"));
    };

    // connect the doubleClicked signal from the table views to the showInfo method
    for (QTableView *tv : { tableWidgetUi->persontab,
                tableWidgetUi->itemtab,
                tableWidgetUi->transtab })
    connect(tv, &QTableView::doubleClicked, this, showInfo);
}

/**
 * @brief MainWindow::setupAddPanel
 * @details This method is responsible for setting up the add panel.
 * It creates the add panel widget and adds it to the side panel widget.
 * It also enters edit mode directly at beginning.
 */
void MainWindow::setupAddPanel()
{
    // create the add panel widget and add it to the side panel widget
    addPanel = new InfoPanel(sidePanelUi->addpanel);
    QVBoxLayout *addLayout = new QVBoxLayout(sidePanelUi->addpanel);
    addLayout->addWidget(addPanel);
    addPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // enter edit mode directly at beginning
    addPanel->enterEditMode();

    // connect the add button in the toolbar to the addPanel
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
                schemaJson = libitemModel->getDefaultSchema();
                break;
            case 2: // Transaction tab
                defaultJson = transactionModel->getDefaultJsonObject();
                schemaJson = transactionModel->getDefaultSchema();
                break;
            default:
                return; // No valid tab selected
        }
        // set the default json object and schema for the add panel
        addPanel->displayInfo(defaultJson, schemaJson, true);
        // enter edit mode at beginning
        addPanel->enterEditMode();
        // change the title of the side dock to "Add New Item"
        sideDock->setWindowTitle(tr("Add New Item"));
    });

    // connect the saveRequested signal from the addPanel to the saveNewData method
    connect(addPanel, &InfoPanel::saveRequested, this, &MainWindow::saveNewData);
    
    // connect the completer setup signal for addPanel
    connect(addPanel, &InfoPanel::completerSetupRequested, this, &MainWindow::setupCompleterForAddpanel);

    // When the cancel button in the add panel is clicked, enter edit mode again. 
    // This keeps the addPanel in edit mode all the time, while the cancel button still resets the form.
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
                schemaJson = libitemModel->getDefaultSchema(); 
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
    });
    
    // connect the fieldChanged signal from the addPanel to the updateSubclassType and changedMediaId methods
    connect(addPanel, &InfoPanel::fieldChanged, this, 
            [this](QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue) {
                updateSubclassType(item, column, fieldName, oldValue, newValue);
                changedMediaId(addPanel, item, column, fieldName, oldValue, newValue);
            });
}

/**
 * @details This method is responsible for setting up the data layers.
 * It creates the library, table models and proxy models.
 */
void MainWindow::setupDataLayers()
{
    setupLib();
    setupTableModels();
    setupProxyModels();
}

/**
 * @details This method is responsible for setting up the library.
 * It creates the library object, which itself will create the manager classes under the hood.
 * Preloading is disabled here due to following reason:
 * - The LibitemTableModel handles when a libitem is changing/initializing its media reference.
 *   It will then search for the media item in the media manager and increases the reference count in this media item.
 * - When preloading would be enabled, the increase of the reference count could not be handled correctly, as the LibitemTableModel is not yet initialized.
 */
void MainWindow::setupLib()
{
    lib = new Library((QCoreApplication::applicationDirPath() + QDir::separator() + "data"), false);
}

/**
 * @details This method is responsible for setting up the table models.
 * It creates the person, libitem and transaction table models.
 * Then the data is loaded from the files.
 */
void MainWindow::setupTableModels()
{
    // create table models
    personModel = new PersonTableModel(lib->getPersonManager(), lib->getMediaManager(), lib->getTransactionManager(), this);
    libitemModel = new LibItemTableModel(lib->getLibitemManager(), lib->getMediaManager(), lib->getTransactionManager(), lib->getPersonManager(), this);
    transactionModel = new TransactionTableModel(lib->getTransactionManager(), 
                                                 lib->getPersonManager(), 
                                                 lib->getLibitemManager(), 
                                                 lib->getMediaManager(), this);
    mediaModel = new MediaTableModel(lib->getMediaManager(),this);

    lib->load(); // load the data from the files
}

/**
 * @details This method is responsible for setting up the proxy models.
 * It creates the person, libitem and transaction proxy models.
 * Furthermore the Sort and Filter functionality is enabled.
 * Additionally the selection, header and size policy are set.
 */
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

    // set the proxy models to the table views, configure the selection, header and size policy
    tableWidgetUi->persontab->setModel(personProxy);
    tableWidgetUi->persontab->setSortingEnabled(true); // enable sorting
    tableWidgetUi->persontab->setSelectionBehavior(QAbstractItemView::SelectRows); // select rows
    tableWidgetUi->persontab->setSelectionMode(QAbstractItemView::MultiSelection); // allow selection of multiple rows
    tableWidgetUi->persontab->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // allow resizing of the columns
    tableWidgetUi->persontab->horizontalHeader()->setStretchLastSection(true); // stretch the last section to the remaining space
    tableWidgetUi->persontab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // allow the table view to expand
    
    tableWidgetUi->itemtab->setModel(libitemProxy);
    tableWidgetUi->itemtab->setSortingEnabled(true);
    tableWidgetUi->itemtab->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetUi->itemtab->setSelectionMode(QAbstractItemView::MultiSelection);
    tableWidgetUi->itemtab->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableWidgetUi->itemtab->horizontalHeader()->setStretchLastSection(true);
    tableWidgetUi->itemtab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    tableWidgetUi->transtab->setModel(transactionProxy);
    tableWidgetUi->transtab->setSortingEnabled(true);
    tableWidgetUi->transtab->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetUi->transtab->setSelectionMode(QAbstractItemView::MultiSelection);
    tableWidgetUi->transtab->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableWidgetUi->itemtab->horizontalHeader()->setStretchLastSection(true);
    tableWidgetUi->transtab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


}

/**
 * @details This method is responsible for setting up the toolbar connections.
 * It initializes the columns, sort, search and delete connections.
 */
void MainWindow::setupToolbarConnections()
{
    setupColumnsConnections();
    setupSortConnections();
    setupSearchConnections();
    setupDeleteConnections();
    
    // emit that the tab changed so the sorting and filtering is updated at the beginning
    emit tableWidgetUi->TabSelector->currentChanged(0);
}

/**
 * @details This method is responsible for setting up the columns connections.
 * It initializes the columns dropdown to the current model columns.
 */
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
        // set the columns dropdown to the current model columns
        toolbarUi->setColumns(model);

    });

    // use the dropdown selection change signal to uncheck/check this item in the columns dropdown
    QObject::connect(toolbarUi->columns->view(), &QAbstractItemView::pressed,
                     [&](const QModelIndex &index) {
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

        // get the columns combo box and the model
        QComboBox *combo = toolbarUi->columns;
        QAbstractItemModel *m = combo->model();
        // get the check state of the item
        Qt::CheckState cs = static_cast<Qt::CheckState>(
            m->data(index, Qt::CheckStateRole).toInt());
        // invert the check state of the item
        m->setData(index,
                (cs == Qt::Checked ? Qt::Unchecked : Qt::Checked),
                Qt::CheckStateRole);
        // show the popup of the combo box
        combo->showPopup();

        // update the columns in the current model
        std::visit([&](auto* m) {
            m->setDisplayedColumns(toolbarUi->getCheckedColumns());
        }, model);
    });
}

/**
 * @details This method is responsible for setting up the sort connections.
 * It initializes the sort dropdown to the current model columns.
 */
void MainWindow::setupSortConnections()
{
    // 1) When the tab is changed, the sort dropdown should be updated to the current model columns
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

    // 2) When the sort dropdown is changed, the current proxy model should be sorted by the selected column
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
                qWarning() << "Invalid tab index for sorting:" << tableWidgetUi->TabSelector->currentIndex();
                return;
        }

        if (!currentProxyModel || !currentTableView) {
            qWarning() << "Could not determine ProxyModel or TableView for sorting.";
            return;
        }

        // get the column to sort from the dropdown
        int columnToSort = toolbarUi->sort->itemData(dropdownIndex).toInt();
        // get the current sort column and order of the proxy model
        int currentSortColumn = currentProxyModel->sortColumn();
        Qt::SortOrder currentSortOrder = currentProxyModel->sortOrder();

        // set the new sort order to ascending
        Qt::SortOrder newSortOrder = Qt::AscendingOrder;
        // if the same column is selected again, reverse the sort order
        if (columnToSort == currentSortColumn) {
            newSortOrder = (currentSortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
        }
        
        currentProxyModel->sort(columnToSort, newSortOrder);
        // update the header of the table view to show the sort indicator
        currentTableView->horizontalHeader()->setSortIndicator(dropdownIndex, newSortOrder);
    });
}

/**
 * @details Updates the search filter of the current proxy model.
 */
void MainWindow::applySearchFilter()
{
    // get the current active proxy model
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
                qWarning() << "Invalid tab index for filtering:" << tableWidgetUi->TabSelector->currentIndex();
                return;
        }

    if (!currentProxyModel) {
        qWarning() << "Could not determine ProxyModel for filtering.";
        return;
    }
    // extract the search string from the search bar
    QString filterString = toolbarUi->searchbar->text();

    // set the filter key column to -1, so the search is applied to all columns
    currentProxyModel->setFilterKeyColumn(-1);

    // set the search string to the text in the search bar
    currentProxyModel->setSearchString(filterString, " ");
}

/**
 * @details This method is responsible for setting up the search connections.
 * The table gets updated when the tab is changed or the search bar text is changed.
 */
void MainWindow::setupSearchConnections()
{
    // 1) When the tab is changed, the filter is applied
    QObject::connect(tableWidgetUi->TabSelector, &QTabWidget::currentChanged, [this]()
    {applySearchFilter();});

    // 2) When the text in the search bar is changed, the filter is applied
    QObject::connect(toolbarUi->searchbar, &QLineEdit::textChanged, [this](const QString&)
    {applySearchFilter();});
}

/**
 * @details This method is responsible for setting up the delete connections.
 */
void MainWindow::setupDeleteConnections()
{
    // 1) When the deletec button is clicked, the selected rows in the current table view should be deleted
    QObject::connect(toolbarUi->deletec, &QPushButton::clicked, [&]() {
        // get the current active table view, proxy model and source model
        QTableView *currentTableView = nullptr;
        CustomFilterProxyModel *currentProxyModel = nullptr;
        QAbstractTableModel *currentSourceModel = nullptr;

        // get the current active table view, proxy model and source model based on the current tab
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
                qWarning() << "Invalid tab index for delete operation:" << tableWidgetUi->TabSelector->currentIndex();
                return;
        }

        // check if the table view, proxy model and source model are valid
        if (!currentTableView || !currentProxyModel || !currentSourceModel) {
            qWarning() << "Could not determine TableView, ProxyModel or SourceModel for delete operation.";
            return;
        }

        // get the selected rows
        QModelIndexList selectedIndexes = currentTableView->selectionModel()->selectedRows();
        
        // check if there are any selected rows
        if (selectedIndexes.isEmpty()) {
            QMessageBox::information(this, "No selection", "Please select at least one row to delete.");
            return;
        }

        // show confirmation dialog
        QString message = QString("Do you really want to delete %1 entry/entries?").arg(selectedIndexes.size());
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm deletion", message,
                                                                QMessageBox::Yes | QMessageBox::No);
        
        if (reply != QMessageBox::Yes) {
            return;
        }

        // convert the proxy indexes to source indexes
        QList<int> sourceRows;
        for (const QModelIndex &proxyIndex : selectedIndexes) {
            QModelIndex sourceIndex = currentProxyModel->mapToSource(proxyIndex);
            sourceRows.append(sourceIndex.row());
        }

        // sort the rows in descending order, so the index shifting is avoided
        std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());

        // delete the rows from the source model
        for (int row : sourceRows) {
            currentSourceModel->removeRow(row);
        }
    });
}

/**
 * @details This method is responsible for saving the modified data from the info panel.
 * It updates the data in the current edit model and index.
 * When the data is saved, a new double click event is emitted on the same index to update the info panel.
 */
void MainWindow::saveModifiedData(const QJsonObject& data) {
    // check if the current edit model and index are valid
    if (!currentEditModel || !currentEditIndex.isValid()) {
        QMessageBox::warning(this, tr("Error"), tr("No valid data to save."));
        return;
    }

    try {
        Result success = Result::Error("No valid data to save");
        
        // save the data to the current table model
        if (TransactionTableModel* tm = qobject_cast<TransactionTableModel*>(currentEditModel)) {
            success = tm->updateFromJsonObject(data, currentEditIndex);
        } 
        else if (PersonTableModel* pm = qobject_cast<PersonTableModel*>(currentEditModel)) {
            success = pm->updateFromJsonObject(data, currentEditIndex);
        } 
        else if (LibItemTableModel* lm = qobject_cast<LibItemTableModel*>(currentEditModel)) {
            success = lm->updateFromJsonObject(data, currentEditIndex);
        }

        // check if the save operation was successful
        if (success) {
            // get the current active table view and proxy model
            QTableView* currentTableView = nullptr;
            CustomFilterProxyModel* currentProxyModel = nullptr;
            
            // Determine which table view corresponds to the current edit model
            if (qobject_cast<TransactionTableModel*>(currentEditModel)) {
                currentTableView = tableWidgetUi->transtab;
                currentProxyModel = transactionProxy;
            } 
            else if (qobject_cast<PersonTableModel*>(currentEditModel)) {
                currentTableView = tableWidgetUi->persontab;
                currentProxyModel = personProxy;
            } 
            else if (qobject_cast<LibItemTableModel*>(currentEditModel)) {
                currentTableView = tableWidgetUi->itemtab;
                currentProxyModel = libitemProxy;
            }
            
            if (currentTableView && currentProxyModel) {
                // Map source index to proxy index
                QModelIndex proxyIndex = currentProxyModel->mapFromSource(currentEditIndex);
                if (proxyIndex.isValid()) {
                    // Emit the double click signal to refresh the info panel
                    emit currentTableView->doubleClicked(proxyIndex);
                }
            }

            // Notify model about changes
            currentEditModel->dataChanged(currentEditIndex, currentEditIndex);
            QMessageBox::information(this, tr("Saved"), 
                                   tr("Changes were successfully saved."));
        } else {
            // show the error message
            QString errorMessage = ((QString) success);
            qDebug() << "Error message:" << errorMessage;
            QMessageBox::warning(this, tr("Error"), 
                               errorMessage);
        }
    } 
    catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Error"), 
                            tr("Unexpected error: %1").arg(e.what()));
    }
}

/**
 * @details This method is responsible for saving new data from the add panel to the current table model.
 */
void MainWindow::saveNewData(const QJsonObject& data) {
    try {
        Result success;

        // save the data to the current table model based on the current tab
        int currentIndex = tableWidgetUi->TabSelector->currentIndex();
        switch(currentIndex) {
            case 0: // Person tab
                success = personModel->saveFromJsonObject(data);
                break;
            case 1: // LibItem tab
                success = libitemModel->saveFromJsonObject(data);
                break;
            case 2: // Transaction tab
                success = transactionModel->saveFromJsonObject(data);
                break;
            default:
                return; // No valid tab selected
        }

        // reset the add panel and enter edit mode again
        emit tableWidgetUi->TabSelector->currentChanged(currentIndex);

        // check if the save operation was successful
        if ((bool) success) {
            QMessageBox::information(this, tr("Saved"),
                                   tr("New entry was successfully saved."));
        } else {
            QMessageBox::warning(this, tr("Error"),
                               QString(success));
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Error"),
                            tr("Unexpected error: %1").arg(e.what()));
    }
}

/**
 * @details This method is responsible for setting up the search completer.
 */
void MainWindow::setupSearchCompleter()
{
    // create the completer
    searchCompleter = new QCompleter(this);
    // set the case sensitivity to case insensitive
    searchCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    searchCompleter->setFilterMode(Qt::MatchContains);
    searchCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    searchCompleter->setMaxVisibleItems(10);

    // set the origSearchText to an empty string initially for safety
    origSearchText = QString();
    
    // when a completion is highlighted, activate the onCompleterActivated method to show the text in the search bar
    connect(searchCompleter, QOverload<const QString&>::of(&QCompleter::highlighted),
            this, [this](const QString& text) {
                MainWindow::onCompleterActivated(text);
            });

    // when a completion is activated, activate the onCompleterActivated method to show the text in the search bar
    connect(searchCompleter, QOverload<const QString&>::of(&QCompleter::activated),
            this, [this](const QString& text) {
                // overwrite qt internal logic to complete correctly
                MainWindow::onCompleterActivated(text);
                // hide the completer popup after activation
                searchCompleter->popup()->hide();
            });


    // when the tab is changed, update the completer
    connect(tableWidgetUi->TabSelector, &QTabWidget::currentChanged,
            this, [this]() {
                updateSearchCompleter();
                searchCompleter->popup()->hide(); // hide the popup on tab change
            });

    // when the text in the search bar is changed, update the completer
    connect(toolbarUi->searchbar, &QLineEdit::textChanged,
            this, &MainWindow::updateSearchCompleter);

    // initial fill for the current tab
    updateSearchCompleter();
    
    // set the completer to the search bar
    toolbarUi->searchbar->setCompleter(searchCompleter);
}

/**
 * @details This method is responsible for updating the search completer.
 */
void MainWindow::updateSearchCompleter()
{
    // create a list of suggestions
    QStringList suggestions;
    // get the current tab index
    int currentTabIdx = tableWidgetUi->TabSelector->currentIndex();

    // get the current active table view and proxy model
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

    // get the last token from the search bar
    QString lastToken = toolbarUi->searchbar->text().split(' ').last();
              
    QStringListModel *stringModel = new QStringListModel(suggestions, searchCompleter);
    searchCompleter->setModel(stringModel);
    searchCompleter->setCompletionPrefix(lastToken);
    searchCompleter->complete(); // Update the popup with new suggestions
    searchCompleter->popup()->show();
}

/**
 * @details This method is responsible for handling the completion of the search completer.
 * It updates the search bar with the selected suggestion.
 * When the suggestion begins with the last token, the last token is replaced with the suggestion.
 * Otherwise the whole current text is replaced with the suggestion.
 */
void MainWindow::onCompleterActivated(const QString& suggestion)
{
    // check if the search bar is available
    if (!toolbarUi || !toolbarUi->searchbar) {
        qWarning() << "SearchBar UI component not available";
        return;
    }
    
    // check if the suggestion is empty
    if (suggestion.isEmpty()) {
        qDebug() << "Empty suggestion received, ignoring";
        return;
    }
    
    // get the current search text and split it into tokens
    QString currentText = origSearchText;
    QStringList tokens = currentText.split(' ', Qt::SkipEmptyParts);
    
    // if the current text ends with a space, add a new empty token
    if (currentText.endsWith(' ')) {
        tokens.append(QString());
    }
    

    QString newSearchText;

    // if the current text is empty or only contains spaces, set the new search text to the suggestion
    if (tokens.isEmpty()) {
        newSearchText = suggestion;
    }
    else {
        QString lastToken = tokens.last();
        
        // check if the suggestion starts with the last token (case-insensitive)
        bool startsWithLastToken = !lastToken.isEmpty() && 
                                  suggestion.startsWith(lastToken, Qt::CaseInsensitive);
        
        if (startsWithLastToken) {
            // replace the last token with the suggestion
            tokens[tokens.size() - 1] = suggestion;
            newSearchText = tokens.join(' ');
        }
        else {
            // replace the complete current text with the suggestion
            newSearchText = suggestion;
        }
    }
    
    // Update the search bar with the new search text
    // block the signals of the search bar to avoid recursive updateSearchCompleter calls
    toolbarUi->searchbar->blockSignals(true);
    
    // remove the completer temporarily to avoid that the lineedit gets overwritten by internal qt logic
    toolbarUi->searchbar->setCompleter(nullptr);
    
    toolbarUi->searchbar->setText(newSearchText);
    
    // Set cursor to the end for further input
    toolbarUi->searchbar->setCursorPosition(newSearchText.length());
    
    // reattach the completer
    toolbarUi->searchbar->setCompleter(searchCompleter);
    // make the popup visible again
    searchCompleter->popup()->show();
    toolbarUi->searchbar->blockSignals(false);
}

/**
 * @details This method is responsible for handling the close event of the main window.
 * It saves the library data on close.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    // save the library data on close
    if (lib) lib->save();
    QMainWindow::closeEvent(event);
}
