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
#include <iostream>

#include "custfiltproxmodel.h"
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

    setupSearchCompleter();
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
    qDebug() << "Completer activated with suggestion:" << suggestion;
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

        qDebug() << startsWithLastToken;
        
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
