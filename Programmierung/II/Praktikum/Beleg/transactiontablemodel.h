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

        Q_ENUM(ColumnIdentity);

        explicit TransactionTableModel(TransactionMan* transactionMan, PersonMan* personMan, LibitemMan* libItemMan, MediaMan* mediaMan, QObject *parent = nullptr);
        ~TransactionTableModel() override;

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

        void setDisplayedColumns(const QVector<ColumnIdentity>& displayedColumns);
        QMap<ColumnIdentity, QString> getAllColumnNames() const;
        QVector<ColumnIdentity> getDisplayedColumns() const;
    
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