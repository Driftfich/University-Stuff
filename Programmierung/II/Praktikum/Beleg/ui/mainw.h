// Uses the toolbar.h and tablewidget.h files to create the main window ui.
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

        // getter for the toolbar and tableWidget
        Ui_toolbar* getToolbarUi() const { return toolbarUi; }
        Ui_TableWidget* getTableWidgetUi() const { return tableWidgetUi; }

    protected:
        void closeEvent(QCloseEvent *event) override;

    private:
        // UI components
        Ui_toolbar*      toolbarUi;
        QWidget*         toolbarWidget;
        Ui_TableWidget*  tableWidgetUi;
        QWidget*         tableWidgetWidget;
        QVBoxLayout*     mainLayout;

        // side panel ui components
        QDockWidget*   sideDock;
        Ui::Form* sidePanelUi;
        QWidget* sidePanelWidget;
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

        // Proxy Models
        CustomFilterProxyModel* personProxy;
        CustomFilterProxyModel* libitemProxy;
        CustomFilterProxyModel* transactionProxy;

        
        // UI setup methods
        void setupUi();
        void setupSideDock();
        void setupAddPanel();

        // 
        void setupDataLayers();

        // setup method for the lib
        void setupLib();

        // setup method for the table models
        void setupTableModels();
        void setupProxyModels();
        
        void setupToolbarConnections();
        void setupColumnsConnections();
        void setupSortConnections();
        void setupSearchConnections();
        void setupDeleteConnections();
        // void setupAddConnections();

        void applySearchFilter();
        
        // search completer methods
        void setupSearchCompleter();
        void updateSearchCompleter();
        void onCompleterActivated(const QString& suggestion);
        
        // Method to save the modified data from the InfoPanel &  addPanel
        void saveModifiedData(const QJsonObject& modifiedData);
        void saveNewData(const QJsonObject& newData);


        void updateSubclassType(QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue);
        void changedMediaId(InfoPanel* panel, QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue);
        void setupCompleterForAddpanel(QLineEdit* editor, const QModelIndex& index);
        void setupCompleterForInfoPanel(QLineEdit* editor, const QModelIndex& index);
        void setupUnifiedFieldChangeHandler();
        void enabledArtist(QTreeWidgetItem* item, int column, const QString& fieldName, const QVariant& oldValue, const QVariant& newValue);
        void addPanelSave();


};