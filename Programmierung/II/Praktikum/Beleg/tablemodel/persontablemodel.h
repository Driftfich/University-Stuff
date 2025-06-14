// Abstract Qt table model for the personman class

#ifndef _PERSONMODEL_H
#define _PERSONMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
#include "personman.h"
#include <QJsonObject>
#include "transactionman.h"
#include "returns.h"

class PersonTableModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        enum ColumnIdentity {
            Id,
            FirstName,
            LastName,
            ExtensionName,
            BirthDate,
            Gender,
            Location,
            Email,
            Phone,
            Note,
            MaxColumnIdentity
        };

        Q_ENUM(ColumnIdentity)

        explicit PersonTableModel(PersonMan* personMan, QObject *parent = nullptr);
        ~PersonTableModel() override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        int removeRow(int row, const QModelIndex &parent);
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        // void setDisplayedColumns(const QVector<ColumnIdentity>& displayedColumns);
        void setDisplayedColumns(const QVector<QString>& displayedColumns);
        QMap<ColumnIdentity, QString> getAllColumnNames() const;
        QVector<QString> getDisplayedColumns() const;

        QJsonObject getJsonObject(const QModelIndex& index) const;
        QJsonObject getJsonObject(const QModelIndex& index, const TransactionMan* transactionMan) const;
        QJsonObject getSchemaObject(const QModelIndex& index) const;
        QJsonObject getDefaultJsonObject(bool artistChecked = false, bool borrowerChecked = false) const;
        QJsonObject getDefaultSchema(bool artistChecked = false, bool borrowerChecked = false) const;
        bool updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index);
        Result saveFromJsonObject(const QJsonObject& jsonObject);

    public slots:
        void refreshData();

    private:
        PersonMan* personMan;
        QVector<ColumnIdentity> displayedColumns;

};

#endif