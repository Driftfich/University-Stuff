// Uses the toolbar.h and tablewidget.h files to create the main window ui.
#include "ui/toolbar.h"
#include "ui/tablewidget.h"
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

#include <QApplication>
#include <QHeaderView>
#include <QSizePolicy>


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
        {
            setupUi();
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

    // …existing code…

    void setupUi()
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
};