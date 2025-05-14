#ifndef _LIBITEM_H
#define _LIBITEM_H

#include "media.h"
#include "config.h"

class Libitem {
    unsigned long id; // libitem id
    unsigned long lib_id; // library id
    unsigned long media_id; // media id
    Media* media; // pointer to media object
    unsigned int available_copies; // number of available copies in the library

    public:

        // setters
        void setId(unsigned long id) { this->id = id; }
        int setMediaId(unsigned long media_id) { this->media_id = media_id; }
        void setAvailableCopies(unsigned int available_copies) { this->available_copies = available_copies; }

        // getters
        unsigned long getId() const {return id;}
        unsigned long getMediaId() const {return media_id;}
        unsigned int getAvailableCopies() const {return available_copies;}
        Media* getMedia() const {return media;}

        // constructor
        Libitem(unsigned long id, unsigned long media_id, unsigned int available_copies) {
            this->id = id;
            this->media_id = media_id;
            // nullptr is set to null at init. When the media is get the first time, it is loaded from the media list and set to this->media
            this->media = nullptr;
            this->available_copies = available_copies;
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
        }

        // assignment operator
        Libitem& operator=(const Libitem& other) {
            if (this != &other) {
                this->id = other.id;
                this->media_id = other.media_id;
                this->available_copies = other.available_copies;
                if (this->media != nullptr) {
                    delete this->media;
                }
                this->media = other.media; // media object has assignment and copy constructor
            }
            return *this;
        }
}

#endif