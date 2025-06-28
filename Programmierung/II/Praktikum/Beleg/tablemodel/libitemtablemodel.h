/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the LibItemTableModel class, which is a custom table model for the libitem objects.
Purpose: Provide a table model for the libitem objects. Used to display the libitem objects in a table view.
Provides methods to get default schema and json objects for new libitem objects or methods to update and save libitem objects from json objects.
Also provides methods to handle media id changes and provides methods to remove rows or display just a subset of the columns.
*/

#ifndef _LIBITEMTABLEMODEL_H
#define _LIBITEMTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
#include <QJsonObject>

#include "libitem.h"
#include "media.h"
#include "libitemman.h"
#include "transactionman.h"
#include "mediaman.h"
#include "personman.h"
#include "returns.h" // for the Result struct

class LibItemTableModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        // enum with the most important columns of the libitem and media object were choosen for direct view in the table view
        enum ColumnIdentity {
            Id,
            MediaId,
            AvailableCopies,
            BorrowedCopies,
            Location,
            Condition,
            Title,
            PublicationDate,
            // ArtistIds,
            MainArtist,
            Publisher,
            Description,
            Genre,
            Languages,
            // Metadata,
            MaxColumnIdentity
        };

        // make the enum available to QML
        Q_ENUM(ColumnIdentity)

        // constructor and destructor
        explicit LibItemTableModel(LibitemMan* libItemMan, MediaMan* mediaMan, TransactionMan* transactionMan, PersonMan* personMan, QObject *parent = nullptr);
        ~LibItemTableModel() override;

        // override the rowCount and columnCount methods
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        // remove a row by index
        int removeRow(int row, const QModelIndex &parent = QModelIndex());
        // remove multiple rows by index
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

        // handle media id changes
        int handleMediaIdChange(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId);

        // override the data method
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        // override the headerData method
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        // override the flags method
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        // set the displayed columns
        void setDisplayedColumns(const QVector<QString>& displayedColumns);
        // get all the column names
        QMap<ColumnIdentity, QString> getAllColumnNames() const;
        // get the displayed columns
        QVector<QString> getDisplayedColumns() const;

        // get the json object for a specific libitem
        QJsonObject getJsonObject(const QModelIndex& index) const;
        // get the schema object for a specific libitem
        QJsonObject getSchemaObject(const QModelIndex& index) const;
        // update the media from a json object
        Result updateMediaFromJsonObject(const QJsonObject& jsonObject);
        // update the libitem from a json object
        Result updateLibitemFromJsonObject(const QJsonObject& jsonObject);
        // update the libitem from a json object and a specific index
        Result updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index);

        // get the default schema for a specific media type
        QJsonObject getDefaultSchema(QString mediaType) const;
        // get the default schema
        QJsonObject getDefaultSchema() const;
        // get the default json object for a specific media type
        QJsonObject getDefaultJsonObject(QString mediaType) const;
        // get the default json object
        QJsonObject getDefaultJsonObject() const;
        // save the libitem from a json object
        Result saveFromJsonObject(const QJsonObject& jsonObject);

    public slots:
        // refresh the data in the table model
        void refreshData();

    private:
        // pointer to the libitem manager
        LibitemMan* libItemMan;
        // pointer to the media manager
        MediaMan* mediaMan;
        // pointer to the transaction manager
        TransactionMan* transactionMan;
        // pointer to the person manager
        PersonMan* personMan;
        // vector with the displayed columns
        QVector<ColumnIdentity> displayedColumns;
};

#endif