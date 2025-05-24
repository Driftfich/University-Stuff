// Uses the toolbar.h and tablewidget.h files to create the main window ui.
#include "ui/toolbar.h"
#include "ui/tablewidget.h"
#include "ui/sidepanel.h"
#include "ui/infopanel.h"
#include "persontablemodel.h"
#include "libitemtablemodel.h"
#include "transactiontablemodel.h"
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
            setupSideDock();
        }
        ~MainWindow() {}

        // getter for the toolbar and tableWidget
        Ui_toolbar* getToolbarUi() const { return toolbarUi; }
        Ui_TableWidget* getTableWidgetUi() const { return tableWidgetUi; }

    private:
        Ui_toolbar*      toolbarUi;
        QWidget*         toolbarWidget;
        Ui_TableWidget*  tableWidgetUi;
        QWidget*         tableWidgetWidget;
        QVBoxLayout*     mainLayout;

        QDockWidget*   sideDock;
        Ui::Form* sidePanelUi;
        QWidget* sidePanelWidget;

        InfoPanel* infoPanel;

        QAbstractTableModel* currentEditModel = nullptr;
        QModelIndex currentEditIndex;

    void saveModifiedData(const QJsonObject& modifiedData);

    void setupUi();

    void setupSideDock();

};