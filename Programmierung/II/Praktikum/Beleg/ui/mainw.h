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

#include <QApplication>
#include <QHeaderView>
#include <QSizePolicy>


class MainWindow : public QWidget
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr) : QWidget(parent)
        {
            setupUi(this);
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

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        mainLayout = new QVBoxLayout(MainWindow);
        mainLayout->setObjectName("mainLayout");

        // ---- Toolbar als echtes QWidget ----
        toolbarUi     = new Ui_toolbar();
        toolbarWidget = new QWidget(MainWindow);
        toolbarWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        toolbarWidget->setMaximumHeight(150);
        toolbarUi->setupUi(toolbarWidget);
        mainLayout->addWidget(toolbarWidget);

        // ---- TableWidget als echtes QWidget ----
        tableWidgetUi     = new Ui_TableWidget();
        tableWidgetWidget = new QWidget(MainWindow);
        tableWidgetUi->setupUi(tableWidgetWidget);
        mainLayout->addWidget(tableWidgetWidget);
    }
};