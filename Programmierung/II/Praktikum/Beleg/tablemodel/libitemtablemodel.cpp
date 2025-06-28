/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the LibItemTableModel class, which is a custom table model for the libitem objects.
Purpose: Provide a table model for the libitem objects. Used to display the libitem objects in a table view.
Provides methods to get default schema and json objects for new libitem objects or methods to update and save libitem objects from json objects.
Also provides methods to handle media id changes and provides methods to remove rows or display just a subset of the columns.
*/

#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>
#include <QModelIndex>

// include the media and libitem classes
#include "media.h"
#include "video.h"
#include "text.h"
#include "audio.h"
#include "libitem.h"
#include "libitemman.h"
#include "transactionman.h"
#include "libitemtablemodel.h"

// include utils
#include "jsonschemautils.h"
#include "returns.h"

LibItemTableModel::LibItemTableModel(LibitemMan* libItemMan, MediaMan* mediaMan, TransactionMan* transactionMan, PersonMan* personMan, QObject *parent)
    : QAbstractTableModel(parent), libItemMan(libItemMan), mediaMan(mediaMan), transactionMan(transactionMan), personMan(personMan) {
    // connect the libitemMan onMediaChange signal to the handleMediaIdChange slot
    libItemMan->setOnMediaChangeCallback(
        [this](unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId) {
            return handleMediaIdChange(libitemId, oldMediaId, newMediaId);
        }
    );
    // set the displayed columns to all columns
    for (int i = 0; i < MaxColumnIdentity; ++i) {
        displayedColumns.push_back(static_cast<ColumnIdentity>(i));
    }
}
// Destructor
LibItemTableModel::~LibItemTableModel() {
}

// override the rowCount method with the number of libitems in the libitem manager
int LibItemTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return libItemMan->getLibitems().size();
}

// override the columnCount method with the number of displayed columns
int LibItemTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return displayedColumns.size();
}

// remove a row by index
int LibItemTableModel::removeRow(int row, const QModelIndex &parent) {
    // std::cout << "Trying to remove row: " << row << std::endl;
    if (row < 0 || row >= libItemMan->getLibitems().size()) {
        return false;
    }
    std::shared_ptr<Libitem> libitem = (*libItemMan)[row];
    if (libitem == nullptr) {
        return false;
    }
    std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());
    if (media == nullptr) {
        return false;
    }
    // std::cout << "Media Ref Count: " << media->getRefCount() << std::endl;
    // check if the media item behind the libitem is referenced by any other libitem
    if (media->getRefCount() <= 1) {
        // remove the media item if it is not referenced by any other libitem
        if (mediaMan->removeMedia(media->getId()) != 0) {
            return false; // Failed to remove media item
        }
    }
    beginRemoveRows(parent, row, row);
    // remove the libitem from the libitem manager
    libItemMan->removeLibitem(row);
    endRemoveRows();
    return true;
}

bool LibItemTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row + count > libItemMan->getLibitems().size()) {
        return false;
    }
    for (int i = 0; i < count; ++i) {
        if (!removeRow(row, parent)) {
            return false; // If any row removal fails, return false
        }
    }
    
    return true;
}

// handle media id changes
int LibItemTableModel::handleMediaIdChange(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId) {
    /*
    When the media id of a libitem changes:
        - the reference count of the old media is decremented. When the old media isnt found, the method doesnt return, because the libitem could be created and therefore the uninitialized old media is invalid
        - the reference count of the new media is incremented.
    */
    // std::cout << "Handling media ID change: " << libitemId << ", " << oldMediaId << ", " << newMediaId << std::endl;
    std::shared_ptr<Media> oldMedia = mediaMan->getMedia(oldMediaId);
    if (oldMedia != nullptr) {
        // decrement the reference count of the old media
        oldMedia->setRefCount(oldMedia->getRefCount() - 1);
    }
    else {
        // std::cerr << "Old media not found for ID: " << oldMediaId << std::endl;
    }
    std::shared_ptr<Media> newMedia = mediaMan->getMedia(newMediaId);
    if (newMedia != nullptr) {
        // increment the reference count of the new media
        newMedia->setRefCount(newMedia->getRefCount() + 1);
    }
    else {
        std::cerr << "New media not found for ID: " << newMediaId << std::endl;
    }

    return 0; // Success
}

// override the data method to return the data for a specific index and role
QVariant LibItemTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    // get the row and column from the index
    unsigned long row = (unsigned long) index.row();
    int column = index.column();

    // check if the index is valid
    if (row >= (unsigned long) libItemMan->getLibitems().size() || column < 0 || column >= displayedColumns.size()) {
        return QVariant();
    }

    // get the libitem and media from the row
    std::shared_ptr<Libitem> libitem = (*libItemMan)[row];
    std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());

    // check if the libitem is valid
    if (libitem == nullptr) {
        return QVariant();
    }

    // get the column identity
    ColumnIdentity columnIdentity = displayedColumns[column];
    switch (columnIdentity) {
        case Id:
            return static_cast<quint64>(libitem->getId());
        case MediaId:
            return static_cast<quint64>(libitem->getMediaId());
        case AvailableCopies:
            return static_cast<quint64>(libitem->getAvailableCopies());
        case BorrowedCopies:
            return static_cast<quint64>(transactionMan->getTransactionsByLibitemId(libitem->getId()).size());
        case Location:
            return libitem->getLocation();
        case Condition:
            return libitem->getCondition();
        case Title:
            if (!media) return QVariant();
            return media->getTitle();
        case PublicationDate:
            if (!media) return QVariant();
            return media->getPublicationDate().toString("yyyy-MM-dd");
        case Publisher:
            if (!media) return QVariant();
            return media->getPublisher();
        case MainArtist:
            {if (!media) return QVariant();
            // get the first artist id from the media
            QVector<unsigned long> artistIds = media->getArtistIds();
            if (artistIds.isEmpty()) return QVariant();
            // get the artist from the artist manager
            std::shared_ptr<Person> person = personMan->getPerson(artistIds[0]);
            if (!person) return QVariant();
            return person->getFname() + " " + person->getLname() + " (" + QString::number(person->getId()) + ")";
            };
        case Description:
            if (!media) return QVariant();
            return media->getDescription();
        case Genre:
            if (!media) return QVariant();
            return media->getGenre();
        case Languages:
            if (!media) return QVariant();
            return media->getLanguages().join(", ");
        default:
            return QVariant();
    }
}

// override the headerData method to return the header data for a specific section and orientation
QVariant LibItemTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        ColumnIdentity columnIdentity = displayedColumns[section];
        switch (columnIdentity) {
            case Id:
                return "ID";
            case MediaId:
                return "Media ID";
            case AvailableCopies:
                return "Available Copies";
            case BorrowedCopies:
                return "Borrowed Copies";
            case Location:
                return "Location";
            case Condition:
                return "Condition";
            case Title:
                return "Title";
            case PublicationDate:
                return "Publication Date";
            case MainArtist:
                return "Main Artist";
            case Publisher:
                return "Publisher";
            case Description:
                return "Description";
            case Genre:
                return "Genre";
            case Languages:
                return "Languages";
            default:
                return QVariant();
        }
    }
    return QVariant();
}

// override the flags method to return the flags for a specific index
Qt::ItemFlags LibItemTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    return QAbstractTableModel::flags(index);
}

// update the displayed columns
void LibItemTableModel::setDisplayedColumns(const QVector<QString>& displayedColumns) {
    beginResetModel();
    this->displayedColumns.clear();
    for (const auto& column : displayedColumns) {
        for (int i = 0; i < MaxColumnIdentity; ++i) {
            if (getAllColumnNames()[static_cast<ColumnIdentity>(i)] == column) {
                this->displayedColumns.push_back(static_cast<ColumnIdentity>(i));
                break;
            }
        }
    }
    emit layoutChanged();
    endResetModel();
}

// get all the available column names
QMap<LibItemTableModel::ColumnIdentity, QString> LibItemTableModel::getAllColumnNames() const {
    QMap<ColumnIdentity, QString> columnNames;
    columnNames[Id] = "ID";
    columnNames[MediaId] = "Media ID";
    columnNames[AvailableCopies] = "Available Copies";
    columnNames[BorrowedCopies] = "Borrowed Copies";
    columnNames[Location] = "Location";
    columnNames[Condition] = "Condition";
    columnNames[Title] = "Title";
    columnNames[PublicationDate] = "Publication Date";
    columnNames[MainArtist] = "Main Artist";
    columnNames[Publisher] = "Publisher";
    columnNames[Description] = "Description";
    columnNames[Genre] = "Genre";
    columnNames[Languages] = "Languages";
    return columnNames;
}

// get the displayed columns
QVector<QString> LibItemTableModel::getDisplayedColumns() const {
    QVector<QString> columnNames;
    for (const auto& column : displayedColumns) {
        columnNames.push_back(getAllColumnNames()[column]);
    }
    return columnNames;
}

// get the json object for a specific libitem at a specific index to show in infopanel
QJsonObject LibItemTableModel::getJsonObject(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QJsonObject();
    }

    // get the row from the index
    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) libItemMan->getLibitems().size()) {
        return QJsonObject();
    }

    // get the libitem from the row
    std::shared_ptr<Libitem> libitem = (*libItemMan)[row];
    if (!libitem) {
        return QJsonObject();
    }

    // get the media from the libitem
    std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());

    // create the json object with the libitem and media
    QJsonObject jsonObject = QJsonObject();
    QJsonObject libitemJson = (*libitem).getJson();
    // overwrite the borrowed copies in the libitem json object with the amount of transactions containing the libitem id
    libitemJson["borrowed_copies"] = static_cast<qint64>(transactionMan->getTransactionsByLibitemId(libitem->getId()).size());
    jsonObject["libitem"] = libitemJson;
    if (media) {
        QJsonObject mediaJson = (*media).getJson();
        if (mediaJson.contains("subclass_params") && mediaJson["subclass_params"].toObject().isEmpty()) {
            mediaJson.remove("subclass_params");
        }
        jsonObject["media"] = mediaJson;
    }
    else {
        jsonObject["media"] = QJsonObject();
    }
    return jsonObject;
}

// get the schema object for a specific libitem at a specific index to show in infopanel
QJsonObject LibItemTableModel::getSchemaObject(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QJsonObject();
    }

    // get the row from the index
    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) libItemMan->getLibitems().size()) {
        return QJsonObject();
    }

    // get the libitem from the row
    std::shared_ptr<Libitem> libitem = (*libItemMan)[row];
    if (!libitem) {
        return QJsonObject();
    }

    // get the media from the libitem
    std::shared_ptr<Media> media = mediaMan->getMedia(libitem->getMediaId());

    // create the schema object
    QJsonObject rootSchema;
    rootSchema.insert("type", "object");
    QJsonObject properties;
    properties.insert("type", "object");
    properties.insert("libitem", libitem->getSchema());
    if (media) {
        properties.insert("media", media->getSchemaByType());
    } else {
        properties.insert("media", QJsonObject());
    }
    rootSchema.insert("properties", properties);
    return rootSchema;
}

// update the libitem and media from a json object and a specific index
Result LibItemTableModel::updateFromJsonObject(const QJsonObject& jsonObject, const QModelIndex& index) {
    if (!index.isValid()) {
        return Result::Error("Invalid index");
    }

    // get the row from the index
    unsigned long row = (unsigned long) index.row();
    if (row >= (unsigned long) libItemMan->getLibitems().size()) {
        return Result::Error("Invalid row");
    }

    // update the media and libitem
    emit beginResetModel();
    // first update the media as a new media item could be created which needs to be referenced by the libitem that the onMediaChangeCallback works correctly
    Result mediaUpdate = updateMediaFromJsonObject(jsonObject);
    Result libitemUpdate = updateLibitemFromJsonObject(jsonObject);

    // check if the media or libitem update failed
    if (mediaUpdate != 0) {
        return mediaUpdate;
    }
    if (libitemUpdate != 0) {
        return libitemUpdate;
    }

    emit endResetModel();
    return Result::Success();
    
}

// update or create a media item from a json object
Result LibItemTableModel::updateMediaFromJsonObject(const QJsonObject& jsonObject) {
    // get the media object from the json object
    unsigned long mediaId = jsonObject["media"].toObject()["media"].toObject()["id"].toVariant().toULongLong();
    std::shared_ptr<Media> media = mediaMan->getMedia(mediaId);

    // check if the media is valid
    if (!media) {
        // qDebug() << "Cannot update media from JSON object, media not found with ID:" << mediaId;
        // return -1;
        // create a new media item when the media id is not found
        QJsonObject mediaJson = jsonObject["media"].toObject();
        media = Media::MediaFactory(mediaJson);
        if (!media) {
            qDebug() << "Failed to create new media item from JSON object";
            // return -1; // Failed to create new media item
            return Result::Error("Failed to create new media item from JSON object", -1);
        }
        // add the new media item to the media manager
        mediaMan->addMedia(media);
    }
    // check if the media could be updated
    // else if (media->loadLocalParams(jsonObject["media"]["media"].toObject()) != 0 || media->loadSubclassParams(jsonObject["media"]["subclass_params"].toObject()) != 0) {
    //     qDebug() << "Failed to update media from JSON object, issues with loading parameters";
    //     return Result::Error("Failed to update media from JSON object, issues with loading parameters", -2);
    // }
    else {
        Result loadLocalResult = media->loadLocalParams(jsonObject["media"]["media"].toObject());
        Result loadSubclassResult = media->loadSubclassParams(jsonObject["media"]["subclass_params"].toObject());
        if (loadLocalResult != 0) return loadLocalResult;
        if (loadSubclassResult != 0) return loadSubclassResult;
    }

    // return success
    // std::cout << "Updated media with ID: " << mediaId << std::endl;
    return Result::Success();
}

// update the libitem from a json object. As the libitem id is fixed via schema, the libitem will never be created in this stage
Result LibItemTableModel::updateLibitemFromJsonObject(const QJsonObject& jsonObject) {
    // get the libitem id from the json object
    unsigned long libitemId = jsonObject["libitem"].toObject()["id"].toVariant().toULongLong();
    // get the libitem from the libitem manager
    std::shared_ptr<Libitem> libitem = libItemMan->getLibitem(libitemId);
    // check if the libitem is valid
    if (!libitem) {
        qDebug() << "Cannot update libitem from JSON object, libitem not found with ID:" << libitemId;
        return Result::Error("Libitem not found with ID:" + QString::number(libitemId));
    }
    // update the libitem
    if (libitem->loadLocalParams(jsonObject["libitem"].toObject()) != 0) {
        qDebug() << "Failed to update libitem from JSON object, issues with loading parameters";
        return Result::Error("Failed to update libitem from JSON object, issues with loading parameters");
    }
    // std::cout << "Updated libitem with ID: " << libitemId << std::endl;
    return Result::Success();
}

// save a new libitem from a json object
Result LibItemTableModel::saveFromJsonObject(const QJsonObject& jsonObject) {
    // check if the json object contains the required keys
    if (!jsonObject.contains("libitem") || !jsonObject.contains("media")) {
        qDebug() << "JSON object does not contain required keys 'libitem' or 'media'";
        return Result::Error("Invalid JSON object");
    }

    // try to update the media item. When the return value is -1 a new media item needs to be created
    int mediaUpdateResult = updateMediaFromJsonObject(jsonObject);
    qDebug() << "Media update result:" << mediaUpdateResult;
    if (mediaUpdateResult == -1) {
        // create a new media item
        QJsonObject mediaJson = jsonObject["media"].toObject();
        std::shared_ptr<Media> newMedia = Media::MediaFactory(mediaJson);
        if (!newMedia) {
            qDebug() << "Failed to create new media item from JSON object";
            return Result::Error("Failed to create new media item");
        }
        mediaMan->addMedia(newMedia);
    }

    // libitem is always new
    // create a new libitem from the json object
    QJsonObject libitemJson = jsonObject["libitem"].toObject();
    std::shared_ptr<Libitem> newLibitem = Libitem::LibitemFactory(libitemJson, 
        [this](unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId) {
            return handleMediaIdChange(libitemId, oldMediaId, newMediaId);
        });
    if (!newLibitem) {
        qDebug() << "Failed to create new libitem from JSON object";
        return Result::Error("Failed to create new libitem");
    }

    // add the new libitem to the libitem manager
    libItemMan->addLibitem(newLibitem);

    // emit dataChanged signal to update the view
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);
    emit layoutChanged(); // Notify that the layout has changed

    // custom return value for better notifications in the app
    return Result::Success();
}

// get the default schema for a specific media type
QJsonObject LibItemTableModel::getDefaultSchema(QString mediaType) const {
    QJsonObject defaultSchema;
    defaultSchema.insert("type", "object");
    QJsonObject properties;
    properties.insert("libitem", Libitem::getSchema());
    mediaType = mediaType.toLower();
    if (mediaType == "audio") {
        properties.insert("media", Audio::getSchema());
    } else if (mediaType == "video") {
        properties.insert("media", Video::getSchema());
    } else if (mediaType == "text") {
        properties.insert("media", Text::getSchema());
    } else {
        properties.insert("media", Media::getSchema());
    }

    // make the subclass_type readonly false as this method is used to create new media items in the addpanel.
    QJsonObject mediaSchema = properties["media"].toObject();
    QJsonObject mediaProperties = mediaSchema["properties"].toObject();
    QJsonObject subclassType = mediaProperties["subclass_type"].toObject();
    subclassType["readonly"] = false; // make it editable
    mediaProperties["subclass_type"] = subclassType;
    mediaSchema["properties"] = mediaProperties;
    properties["media"] = mediaSchema;
    defaultSchema.insert("properties", properties);

    return defaultSchema;
}

// get the default schema for default media type if no media type is specified
QJsonObject LibItemTableModel::getDefaultSchema() const {
    return getDefaultSchema("media");
}

// get the default json object for a specific media type
QJsonObject LibItemTableModel::getDefaultJsonObject(QString mediaType) const {
    // get the default schema for the media type
    QJsonObject defaultSchema = getDefaultSchema(mediaType);
    // create the default json object from the schema
    QJsonObject defaultJsonObject = createDefaultJsonFromSchema(defaultSchema);

    // check if the media object contains the subclass_params object and remove it if it is empty as not needed for the default media object
    if (defaultJsonObject.contains("media")) {
        QJsonObject mediaObject = defaultJsonObject["media"].toObject();
        if (mediaObject.contains("subclass_params") && mediaObject["subclass_params"].toObject().isEmpty()) {
            mediaObject.remove("subclass_params");
        }

        // set the id and media_id to the next id
        QJsonObject baseObject = mediaObject["media"].toObject();
        baseObject["id"] = QJsonValue::fromVariant(static_cast<quint64>(mediaMan->getNextId()));
        mediaObject["media"] = baseObject;
        defaultJsonObject["media"] = mediaObject;
    }

    // set the id and media_id to the next id for the libitem
    if (defaultJsonObject.contains("libitem")) {
        QJsonObject libitemObject = defaultJsonObject["libitem"].toObject();
        libitemObject["id"] = QJsonValue::fromVariant(static_cast<quint64>(libItemMan->getNextId()));
        libitemObject["media_id"] = QJsonValue::fromVariant(static_cast<quint64>(mediaMan->getNextId()));
        defaultJsonObject["libitem"] = libitemObject;
    }

    return defaultJsonObject;
}

// get the default json object for default media type if no media type is specified
QJsonObject LibItemTableModel::getDefaultJsonObject() const {
    return getDefaultJsonObject("media");
}

// refresh the data in the table model
void LibItemTableModel::refreshData() {
    beginResetModel();
    endResetModel();
}