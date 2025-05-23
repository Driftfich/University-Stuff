#ifndef _INFOPANEL_H
#define _INFOPANEL_H

#include <QTreeWidget>
#include <QWidget>
#include <QJsonObject>

class InfoPanel : public QWidget {
    Q_OBJECT

    public:
        explicit InfoPanel(QWidget *parent = nullptr);
        ~InfoPanel() override;

    public slots:
        void displayInfo(const QJsonObject& jsonObject);
        void addJsonToTree(const QJsonValue& value, QTreeWidgetItem* parent);

    private:
        QTreeWidget *treeWidget;
};

#endif