/*
Author: Franz Rehschuh
Date: 2025-06-20

Description:
Custom Qt table model to display the transactionman class in the mainwindow.

Purpose:
    - overwrites the data method to return the data for a specific index and role
    - overwrites the headerData method to show the selected columns at given section and orientation
    - overwrites the flags method to return the flags for a specific index
    - methods to update the displayed columns
    - methods to get the json and schema objects for a specific index
    - methods to get default json and schema objects for new transaction objects
    - methods to update and save a transaction from a json object
    - methods to remove rows
    - method to refresh the data
*/

#ifndef _TRANSACTIONTABLEMODEL_H
#define _TRANSACTIONTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
#include <transactionman.h>
#include <mediaman.h>
#include <libitemman.h>
#include <personman.h>
#include "returns.h"


class TransactionTableModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        // enum with the most important attributes of the transaction, libitem, media and person classes
        // more detailed information can be viewed in the infopanel
        enum ColumnIdentity {
            // column names from transaction.h
            Id,
            LibitemId,
            PersonId,
            Timestamp,
            // column names from libitem
            MediaId,
            // most important column names from libitem->media
            Title,
            PublicationDate,
            Publisher,
            // most important column names from person
            FirstName,
            LastName,
            ExtensionName,
            BirthDate,
            MaxColumnIdentity
        };
        
        // enum to be used in the Q_ENUM macro
        Q_ENUM(ColumnIdentity);

        // constructor & destructor
        explicit TransactionTableModel(TransactionMan* transactionMan, PersonMan* personMan, LibitemMan* libItemMan, MediaMan* mediaMan, QObject *parent = nullptr);
        ~TransactionTableModel() override;
        
        // row and column count
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        
        // remove rows
        int removeRow(int row, const QModelIndex &parent = QModelIndex());
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        
        // override the data, headerData and flags methods
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        // methods to update the displayed columns
        void setDisplayedColumns(const QVector<QString>& displayedColumns);
        QMap<ColumnIdentity, QString> getAllColumnNames() const;
        QVector<QString> getDisplayedColumns() const;

        // json and schema methods
        QJsonObject getJsonObject(const QModelIndex& index) const;
        QJsonObject getSchemaObject(const QModelIndex& index) const;
        QJsonObject getDefaultJsonObject() const;
        QJsonObject getDefaultSchema() const;

        // save and update methods
        Result saveFromJsonObject(const QJsonObject& jsonObject);
        Result updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index);
    
    public slots:
        void refreshData();

    private:
        // pointer to the transaction manager
        TransactionMan* transactionMan;
        // pointer to the person manager
        PersonMan* personMan;
        // pointer to the libitem manager
        LibitemMan* libItemMan;
        // pointer to the media manager
        MediaMan* mediaMan;
        // vector with the displayed columns
        QVector<ColumnIdentity> displayedColumns;

};

#endif