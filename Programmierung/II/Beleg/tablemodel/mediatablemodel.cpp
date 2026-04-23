/*
Author: Franz Rehschuh
Date: 2025-06-21

Description: Implementation file for the MediaTableModel class, which is a simple table model for media items used primarily for EntityCompleter.
*/

#include "mediatablemodel.h"


MediaTableModel::MediaTableModel(MediaMan* mediaMan, QObject *parent)
    : QAbstractTableModel(parent), mediaMan(mediaMan)
{
}

MediaTableModel::~MediaTableModel() = default;

int MediaTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (!mediaMan) return 0;
    return static_cast<int>(mediaMan->getAllMedia().size());
}

int MediaTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return MaxColumnIdentity;
}

QVariant MediaTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole || !mediaMan) {
        return QVariant();
    }

    int row = index.row();
    int column = index.column();

    if (row >= mediaMan->getAllMedia().size() || column >= MaxColumnIdentity) {
        return QVariant();
    }

    std::shared_ptr<Media> media = mediaMan->getAllMedia()[row];
    if (!media) {
        return QVariant();
    }

    ColumnIdentity columnIdentity = static_cast<ColumnIdentity>(column);
    switch (columnIdentity) {
        case Id:
            return QString::number(media->getId());
        case Title:
            return media->getTitle();
        case Publisher:
            return media->getPublisher();
        case Description:
            return media->getDescription();
        case Genre:
            return media->getGenre();
        case PublicationDate:
            return media->getPublicationDate().toString("yyyy-MM-dd");
        case SubclassType:
            return media->getSubclassType();
        default:
            return QVariant();
    }
}

QVariant MediaTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }

    ColumnIdentity columnIdentity = static_cast<ColumnIdentity>(section);
    switch (columnIdentity) {
        case Id:
            return "ID";
        case Title:
            return "Title";
        case Publisher:
            return "Publisher";
        case Description:
            return "Description";
        case Genre:
            return "Genre";
        case PublicationDate:
            return "Publication Date";
        case SubclassType:
            return "Type";
        default:
            return QVariant();
    }
}
