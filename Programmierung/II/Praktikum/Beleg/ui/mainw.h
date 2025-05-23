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

    void setupSideDock()
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

};