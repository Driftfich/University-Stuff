/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the Libitem class, which holds information and logic related to library items.

Note:
The class uses a "special" callback function to notify when the media id changes. This is needed to update the reference count of a media item.
Because the libitem doesnt hold a serialized pointer to the media item, but instead just a deserialized unsigned long media id, 
the reference count of the shared pointer would be useless. And because the media id of the libitem can be changed in arbitary locations,
the indification of this change has to be observed at the lowest level possible. To handle this change, this signal has to be propagated to a level (libitemtablemodel)
where simultan access to all libitems and media items is possible.
*/

#ifndef _LIBITEM_H
#define _LIBITEM_H

#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include "media.h"

class Libitem {
    unsigned long id; // libitem id
    unsigned long media_id; // media id
    unsigned long available_copies; // number of available copies in the library
    unsigned long borrowed_copies; // number of borrowed copies
    QString location; // location of the libitem in the library
    QString condition; // condition of the libitem (e.g. new, used, damaged)
    std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> onMediaChangeCallback; // callback function to notify when the media id changed

    public:

        // setters
        void setId(unsigned long id) { this->id = id; }
        void setMediaId(unsigned long media_id) { 
            unsigned long oldMediaId = this->media_id;
            this->media_id = media_id;
            // std::cout << ("Changed media id from " + QString::number(oldMediaId) + " to " + QString::number(media_id)).toStdString() << std::endl;
            if (onMediaChangeCallback) {
                // std::cout << "Calling onMediaChangeCallback" << std::endl;
                onMediaChangeCallback(this->id, oldMediaId, media_id);
            }
        }
        void setAvailableCopies(unsigned long available_copies) { this->available_copies = available_copies; }
        void setBorrowedCopies(unsigned long borrowed_copies) { this->borrowed_copies = borrowed_copies; }
        void setLocation(const QString& location) { this->location = location; }
        void setCondition(const QString& condition) { this->condition = condition; }
        void setOnMediaChangeCallback(std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> callback) {
            this->onMediaChangeCallback = callback;
        }

        // getters
        unsigned long getId() const {return id;}
        unsigned long getMediaId() const {return media_id;}
        unsigned long getAvailableCopies() const {return available_copies;}
        unsigned long getBorrowedCopies() const {return borrowed_copies;}
        QString getLocation() const {return location;}
        QString getCondition() const {return condition;}

        // constructor
        Libitem(unsigned long id, unsigned long media_id, unsigned long available_copies, unsigned long borrowed_copies, 
                const QString& location, const QString& condition, std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> onMediaChangeCallback) {
            setOnMediaChangeCallback(onMediaChangeCallback);
            setId(id);
            setMediaId(media_id);
            setAvailableCopies(available_copies);
            setBorrowedCopies(borrowed_copies);
            setLocation(location);
            setCondition(condition);
        }

        // constructor from JSON object
        Libitem(QJsonObject json, std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> onMediaChangeCallback){
            // std::cout << "Reached Libitem constructor" << std::endl;
            setOnMediaChangeCallback(onMediaChangeCallback);
            if (loadLocalParams(json) != 0) {
                throw std::runtime_error("Issues loading libitem parameters");
            }
        }

        // destructor
        ~Libitem() {
        }

        // copy constructor
        Libitem(const Libitem& other) {
            this->id = other.id;
            this->media_id = other.media_id;
            this->available_copies = other.available_copies;
            this->borrowed_copies = other.borrowed_copies;
            this->location = other.location;
            this->condition = other.condition;
            this->onMediaChangeCallback = other.onMediaChangeCallback;
        }

        // assignment operator
        Libitem& operator=(const Libitem& other) {
            if (this != &other) {
                this->id = other.id;
                this->media_id = other.media_id;
                this->available_copies = other.available_copies;
                this->borrowed_copies = other.borrowed_copies;
                this->location = other.location;
                this->condition = other.condition;
                this->onMediaChangeCallback = other.onMediaChangeCallback;
            }
            return *this;
        }

        // collect parameters into a JSON object
        QJsonObject getJson() const;
        void toFile(QFile& file) const;

        // load local parameters from a JSON object
        int loadLocalParams(const QJsonObject& json);
        // read json object from file
        static std::shared_ptr<Libitem> fromFile(QFile& file, std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> onMediaChangeCallback);
        // factory method
        static std::shared_ptr<Libitem> LibitemFactory(const QJsonObject& json, std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> onMediaChangeCallback);

        // schema methods
        static QJsonObject getLocalSchema();
        static QJsonObject getSchema();

        // print method
        friend std::ostream& operator<<(std::ostream& os, const Libitem& libitem);
};

#endif