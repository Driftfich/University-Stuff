/*
Author: Franz Rehschuh
Date: 2025-06-20

Description:
Custom Qt table model to display the personman class in the mainwindow.

Purpose:
    - overwrites the data method to return the data for a specific index and role
    - overwrites the headerData method to show the selected columns at given section and orientation
    - overwrites the flags method to return the flags for a specific index
    - methods to update the displayed columns
    - methods to get the json and schema objects for a specific index
    - methods to get default json and schema objects for new person objects
    - methods to update and save a person from a json object
    - methods to remove rows
*/
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
#include "mediaman.h"
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

        explicit PersonTableModel(PersonMan* personMan, MediaMan* mediaMan, const TransactionMan* transactionMan, QObject *parent = nullptr);
        ~PersonTableModel() override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        int removeRow(int row, const QModelIndex &parent);
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        void setDisplayedColumns(const QVector<QString>& displayedColumns);
        QMap<ColumnIdentity, QString> getAllColumnNames() const;
        QVector<QString> getDisplayedColumns() const;

        QJsonObject getJsonObject(const QModelIndex& index) const;
        QJsonObject getSchemaObject(const QModelIndex& index) const;
        QJsonObject getDefaultJsonObject(bool artistChecked = false, bool borrowerChecked = false) const;
        QJsonObject getDefaultSchema(bool artistChecked = false, bool borrowerChecked = false) const;
        bool updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index);
        Result saveFromJsonObject(const QJsonObject& jsonObject);

    public slots:
        void refreshData();

    private:
        // pointer to the person manager
        PersonMan* personMan;
        // pointer to the media manager
        MediaMan* mediaMan;
        // pointer to the transaction manager
        const TransactionMan* transactionMan;
        // vector with the displayed columns
        QVector<ColumnIdentity> displayedColumns;

};

#endif