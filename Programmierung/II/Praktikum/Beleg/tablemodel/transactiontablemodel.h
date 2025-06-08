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


class TransactionTableModel : public QAbstractTableModel {
    Q_OBJECT

    public:
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

        // enum ColumnIdentity {
        //     // column names from transaction.h
        //     Id,
        //     LibitemId,
        //     PersonId,
        //     Timestamp,
        //     // column names from libitem
        //     MediaId,
        //     AvailableCopies,
        //     BorrowedCopies,
        //     Location,
        //     Condition,
        //     // most important column names from libitem->media
        //     Title,
        //     PublicationDate,
        //     Publisher,
        //     // most important column names from person
        //     FirstName,
        //     LastName,
        //     ExtensionName,
        //     BirthDate,
        //     MaxColumnIdentity
        // };

        Q_ENUM(ColumnIdentity);

        explicit TransactionTableModel(TransactionMan* transactionMan, PersonMan* personMan, LibitemMan* libItemMan, MediaMan* mediaMan, QObject *parent = nullptr);
        ~TransactionTableModel() override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        int removeRow(int row, const QModelIndex &parent = QModelIndex());
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        // void setDisplayedColumns(const QVector<ColumnIdentity>& displayedColumns);
        void setDisplayedColumns(const QVector<QString>& displayedColumns);
        QMap<ColumnIdentity, QString> getAllColumnNames() const;
        QVector<QString> getDisplayedColumns() const;

        // Method to get a json with all informations about the transaction and the objects it refers to
        QJsonObject getJsonObject(const QModelIndex& index) const;
        QJsonObject getDefaultJsonObject() const;
        QJsonObject getSchemaObject(const QModelIndex& index) const;
        QJsonObject getDefaultSchema() const;
        bool updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index);
    
    public slots:
        void refreshData();

    private:
        TransactionMan* transactionMan;
        PersonMan* personMan;
        LibitemMan* libItemMan;
        MediaMan* mediaMan;
        QVector<ColumnIdentity> displayedColumns;

};

#endif