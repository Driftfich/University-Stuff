#ifndef _INFOPANEL_H
#define _INFOPANEL_H

#include <QTreeWidget>
#include <QWidget>
#include <QJsonObject>
#include <QPushButton>

class InfoPanel : public QWidget {
    Q_OBJECT

    public:
        explicit InfoPanel(QWidget *parent = nullptr);
        ~InfoPanel() override;

    public slots:
        void displayInfo(const QJsonObject& jsonObject);
        // void addJsonToTree(const QJsonValue& value, QTreeWidgetItem* parent);
        void enterEditMode();
        void saveChanges();
        void cancelEditMode();

        void paintDeleteItemButton(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index);
    signals:
        // Signal wird ausgelöst, wenn der User Änderungen speichern will
        void saveRequested(const QJsonObject& modifiedData);

    private:
        QTreeWidget *treeWidget;
        QPushButton *saveButton;
        QPushButton *editButton;

        QJsonObject originalData; // original data for quit edit mode without saving
        bool inEditMode;

        // for delete button at item level
        QPixmap deleteIcon;
        QPersistentModelIndex hoveredItemForDelete;

        // Font configuration
        int m_baseFontSize;
        double m_fontScaleFactor;
        int m_minFontSize;
        int m_maxFontSize;

        void setTreeItemsEditable(bool editable);
        void setTreeItemEditable(QTreeWidgetItem* item, bool editable);
        QJsonObject collectDataFromTree();
        QJsonValue getValueFromItem(QTreeWidgetItem* item);
        void restoreOriginalData();

        void updateAddButtons(bool enable);
        void onAddArrayItem();
        void onAddObjectItem();

        bool isItemDeletable(QTreeWidgetItem* item) const;
        QRect getDeleteButtonRectForItem(const QTreeWidgetItem* item, const QStyleOptionViewItem& option) const;

        void addJsonToTreeRecursive(const QJsonValue& valueForThisItem, QTreeWidgetItem* thisItem, int depth = 0);
        bool isLowestCollection(QTreeWidgetItem* item) const;
        bool isHighestItem(QTreeWidgetItem* item) const;

        QFont calculateFontForDepth(int depth) const;
        int getItemDepth(QTreeWidgetItem* item) const;

    private slots:
        void onItemChanged(QTreeWidgetItem* item, int column);
        void handleDeleteAction(QTreeWidgetItem* item);

    protected:
        bool eventFilter(QObject* watched, QEvent *event) override;
};

#endif