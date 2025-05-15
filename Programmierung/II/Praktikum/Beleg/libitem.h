#ifndef _LIBITEM_H
#define _LIBITEM_H

#include <QString>
#include "media.h"
#include "config.h"

class Libitem {
    unsigned long id; // libitem id
    unsigned long media_id; // media id
    Media* media; // pointer to media object
    unsigned long available_copies; // number of available copies in the library
    unsigned long borrowed_copies; // number of borrowed copies
    QString location; // location of the libitem in the library
    QString condition; // condition of the libitem (e.g. new, used, damaged)
    

    public:

        // setters
        void setId(unsigned long id) { this->id = id; }
        int setMediaId(unsigned long media_id) { this->media_id = media_id; }
        void setAvailableCopies(unsigned long available_copies) { this->available_copies = available_copies; }
        void setBorrowedCopies(unsigned long borrowed_copies) { this->borrowed_copies = borrowed_copies; }
        void setLocation(const QString& location) { this->location = location; }
        void setCondition(const QString& condition) { this->condition = condition; }

        // getters
        unsigned long getId() const {return id;}
        unsigned long getMediaId() const {return media_id;}
        unsigned long getAvailableCopies() const {return available_copies;}
        unsigned long getBorrowedCopies() const {return borrowed_copies;}
        Media* getMedia() const {return media;}
        QString getLocation() const {return location;}
        QString getCondition() const {return condition;}

        // constructor
        Libitem(unsigned long id, unsigned long media_id, unsigned long available_copies, unsigned long borrowed_copies, 
                const QString& location = "", const QString& condition = "") {
            this->id = id;
            this->media_id = media_id;
            // nullptr is set to null at init. When the media is get the first time, it is loaded from the media list and set to this->media
            this->media = nullptr;
            this->available_copies = available_copies;
            this->borrowed_copies = borrowed_copies;
            this->location = location;
            this->condition = condition;
        }

        // destructor
        ~Libitem() {
            if (media != nullptr) {
                delete media;
            }
        }

        // copy constructor
        Libitem(const Libitem& other) {
            this->id = other.id;
            this->media_id = other.media_id;
            this->available_copies = other.available_copies;
            this->media = other.media; // media object has assignment and copy constructor
            this->borrowed_copies = other.borrowed_copies;
            this->location = other.location;
            this->condition = other.condition;
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
                if (this->media != nullptr) {
                    delete this->media;
                }
                this->media = other.media; // media object has assignment and copy constructor
            }
            return *this;
        }
}

#endif