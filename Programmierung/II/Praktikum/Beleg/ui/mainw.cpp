#include <QJsonObject>
#include <QDebug>
#include <QMainWindow>
#include <QDockWidget>
#include <QMessageBox>
#include <iostream>

#include "mainw.h"

void MainWindow::setupUi()
{
    // 1) Eigenes Central Widget erstellen und setzen
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // 2) Layout jetzt auf das Central Widget anwenden
    mainLayout = new QVBoxLayout(central);
    mainLayout->setObjectName("mainLayout");

    // ---- Toolbar als echtes QWidget ----
    toolbarUi     = new Ui_toolbar();
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

    // Beispiel: Button aus Toolbar zeigt Add-Panel
    connect(toolbarUi->add, &QPushButton::clicked, this, [=]() {
        sidePanelUi->stackedWidget->setCurrentWidget(sidePanelUi->addpanel);
        sideDock->show();
    });

    // Signal vom InfoPanel verbinden
    connect(infoPanel, &InfoPanel::saveRequested, this, &MainWindow::saveModifiedData);
    
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
        if (auto *tm = qobject_cast<TransactionTableModel*>(srcModel))
            info = tm->getJsonObject(srcIndex);
        else if (auto *pm = qobject_cast<PersonTableModel*>(srcModel))
            info = pm->getJsonObject(srcIndex);
        else if (auto *lm = qobject_cast<LibItemTableModel*>(srcModel))
            info = lm->getJsonObject(srcIndex);

        // 6) anzeigen
        infoPanel->displayInfo(info);
        sidePanelUi->stackedWidget->setCurrentWidget(sidePanelUi->infopanel);
        sideDock->show();
    };

    for (auto *tv : { tableWidgetUi->persontab,
                tableWidgetUi->itemtab,
                tableWidgetUi->transtab })
    connect(tv, &QTableView::doubleClicked, this, showInfo);
}

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

