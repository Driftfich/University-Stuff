/*
Author: Franz Rehschuh
Date: 2025-06-28

Description: 
The MainWindow class combines all high level ui and data management components to show the data in a user friendly way.

*/


#include "ui/toolbar.h"
#include "ui/tablewidget.h"
#include "ui/sidepanel.h"
#include "ui/infopanel.h"
#include "persontablemodel.h"
#include "libitemtablemodel.h"
#include "transactiontablemodel.h"
#include "custfiltproxmodel.h"
#include "mediatablemodel.h"
#include "library.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableView>
#include <QTabWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QDockWidget>
#include <QVariant>
#include <QJsonObject>
#include <QTabWidget>
#include <QApplication>
#include <QHeaderView>
#include <QSizePolicy>
#include <QSortFilterProxyModel>


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        // constructor and destructor
        MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
        {
            setupUi();
            setupDataLayers();
            setupSideDock();
            setupAddPanel();
            setupToolbarConnections();
            setupSearchCompleter();
            setWindowTitle(tr("Bibliotheksverwaltung"));
        }
        ~MainWindow() {}

    protected:
        // custom close event handler to save the data
        void closeEvent(QCloseEvent *event) override;

    private:
        // UI components
        Ui_toolbar*      toolbarUi; // ui for the toolbar
        QWidget*         toolbarWidget; // widget holding the toolbar
        Ui_TableWidget*  tableWidgetUi; // ui for the table widget
        QWidget*         tableWidgetWidget; // widget holding the table widget
        QVBoxLayout*     mainLayout; // main layout of the main window

        // side panel ui components
        QDockWidget*   sideDock; // dock widget holding the side panel
        Ui::Form* sidePanelUi; // ui for the side panel
        QWidget* sidePanelWidget; // widget holding the side panel
        InfoPanel* infoPanel;
        InfoPanel* addPanel;

        // Models and Proxies for editing in the Infopanel
        QAbstractTableModel* currentEditModel = nullptr;
        QModelIndex currentEditIndex;

        // Search completer objects
        QCompleter *searchCompleter;
        QString origSearchText;

        // Data Management
        Library* lib;

        // Table Models
        PersonTableModel* personModel;
        LibItemTableModel* libitemModel;
        TransactionTableModel* transactionModel;
        MediaTableModel* mediaModel;

        // Proxy Models for searching and filtering
        CustomFilterProxyModel* personProxy;
        CustomFilterProxyModel* libitemProxy;
        CustomFilterProxyModel* transactionProxy;

        
        // UI setup methods
        void setupUi();
        void setupSideDock();
        void setupAddPanel();

        // setup methods for the data layers
        void setupDataLayers();

        // setup method for the lib
        void setupLib();

        // setup method for the table models
        void setupTableModels();
        void setupProxyModels();
        
        // connection setup methods
        void setupToolbarConnections();
        void setupColumnsConnections();
        void setupSortConnections();
        void setupSearchConnections();
        void setupDeleteConnections();
        
        // update the proxy model with new search string from the searchbar
        void applySearchFilter();
        
        // search completer methods
        void setupSearchCompleter();
        void updateSearchCompleter();
        void onCompleterActivated(const QString& suggestion);
        
        // Method to save the modified data from the InfoPanel &  addPanel
        void saveModifiedData(const QJsonObject& modifiedData);
        void saveNewData(const QJsonObject& newData);

        // Special handler methods which handle signals from the InfoPanel/AddPanel
        // When the subclass type is changed in the addPanel, the json and schema are updated/rerendered to show the correct fields
        void updateSubclassType(QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue);
        // When the media id is changed in the panel (addPanel/infoPanel) the form is rerendered to show the correct media object
        void changedMediaId(InfoPanel* panel, QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue);
        // setup completers for the addPanel to show suggestions for specific fields (libitem_id, borrower_id, media_id, artist_ids)
        void setupCompleterForAddpanel(QLineEdit* editor, const QModelIndex& index);
        // setup completers for the infoPanel to show suggestions for specific fields (media_id, artist_ids)
        void setupCompleterForInfoPanel(QLineEdit* editor, const QModelIndex& index);
        // when the artist is enabled in the addPanel, the artist object is updated in the form by rerendering the form
        void enabledArtist(QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue);

};