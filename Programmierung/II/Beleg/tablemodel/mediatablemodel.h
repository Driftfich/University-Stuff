/*
Author: Franz Rehschuh
Date: 2025-06-21

Description: Header file for the MediaTableModel class, which is a simple table model for media items used primarily for EntityCompleter.
*/

#ifndef _MEDIATABLEMODEL_H
#define _MEDIATABLEMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QVector>
#include <memory>

#include "mediaman.h"
#include "media.h"

class MediaTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum ColumnIdentity {
        Id,
        Title,
        Publisher,
        Description,
        Genre,
        PublicationDate,
        SubclassType,
        MaxColumnIdentity
    };

    Q_ENUM(ColumnIdentity)

    explicit MediaTableModel(MediaMan* mediaMan, QObject *parent = nullptr);
    ~MediaTableModel() override;

    // QAbstractTableModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    MediaMan* mediaMan;
};

#endif
