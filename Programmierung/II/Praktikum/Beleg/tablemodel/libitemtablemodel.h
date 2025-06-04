#ifndef _LIBITEMTABLEMODEL_H
#define _LIBITEMTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>
#include "libitemman.h"
#include "libitem.h"
#include "media.h"
#include "mediaman.h"

class LibItemTableModel : public QAbstractTableModel {
    Q_OBJECT

    public:
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
            Publisher,
            Description,
            Genre,
            Languages,
            // Metadata,
            MaxColumnIdentity
        };

        Q_ENUM(ColumnIdentity)

        explicit LibItemTableModel(LibitemMan* libItemMan, MediaMan* mediaMan, QObject *parent = nullptr);
        ~LibItemTableModel() override;

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

        QJsonObject getJsonObject(const QModelIndex& index) const;
        QJsonObject getDefaultJsonObject() const;
        QJsonObject getSchemaObject(const QModelIndex& index) const;
        bool updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index);

    public slots:
        void refreshData();

    private:
        LibitemMan* libItemMan;
        MediaMan* mediaMan;
        QVector<ColumnIdentity> displayedColumns;
};

#endif