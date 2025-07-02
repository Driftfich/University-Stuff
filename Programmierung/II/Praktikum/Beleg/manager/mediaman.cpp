/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation of the MediaMan class, which holds the media objects and provides methods to load and save the data from and to files.
Uses a hash map for fast access to media objects by id.
It also provides methods to add, remove and get media objects by id.
*/

#include <iostream>
#include <QString>
#include <QVector>
#include <QHash>

#include "media.h"
#include "mediaman.h"
#include "config.h"

// constructor
MediaMan::MediaMan(QString filename, bool load) {
    setNextId(0);
    setFilename(filename);
    if (load) {
        this->load();
    }
}

// destructor, saves the data to the file
MediaMan::~MediaMan() {
    save();
}

// setter for the filename
int MediaMan::setFilename(const QString& filename) {
    this->filename = filename;
    return 0;
}

// add a media object to the vector and the hash map
int MediaMan::addMedia(std::shared_ptr<Media> media) {
    if (media) {
        this->media.push_back(media);
        // if the new media object has an id greater than the next id, update the next id
        if (media->getId() >= next_id) {
            setNextId(media->getId() + 1);
        }
        media_map.insert(media->getId(), media);
        // add the media to the hash map by person id
        for (const auto& artistId : media->getArtistIds()) {
            media_by_person[artistId].push_back(media);
        }
        return 0;
    }
    return -1;
}

// remove a media object from the vector and the hash map
int MediaMan::removeMedia(unsigned long id) {
    for (QVector<std::shared_ptr<Media>>::iterator it = media.begin(); it != media.end(); ++it) {
        if ((*it)->getId() == id) {
            media.erase(it);
            media_map.remove(id);
            // remove the media from the hash map by person id
            for (const auto& artistId : (*it)->getArtistIds()) {
                media_by_person[artistId].removeOne(*it);
            }
            return 0;
        }
    }
    return -1;
}

// get the media objects by person id
QVector<std::shared_ptr<Media>> MediaMan::getMediaByPersonId(unsigned long personId) const {
    return media_by_person.value(personId, QVector<std::shared_ptr<Media>>());
}

// get the next id
unsigned long MediaMan::getNextId() const {
    // if the next id is invalid/to big, return 0
    if (next_id == INVALID_MEDIA_ID) {
        return 0;
    }
    return next_id;
}
QVector<std::shared_ptr<Media>> MediaMan::getAllMedia() const {
    return media;
}

// get a media object by id
std::shared_ptr<Media> MediaMan::getMedia(unsigned long id) const {
    return media_map.value(id, nullptr);
}

// get the filename
QString MediaMan::getFilename() const {
    return filename;
}

// load the data from the file
int MediaMan::load() {
    QFile file(filename);
    // check if the file exists
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error opening file for reading: " << filename.toStdString() << std::endl;
        return -1;
    }
    // load each media object from the file
    while (!file.atEnd()) {
        std::shared_ptr<Media> media = Media::fromFile(file);
        if (media) {
            addMedia(media);
        } else {
            std::cerr << "Error loading media from file" << std::endl;
            break;
        }
    }
    return 0;
}

// save the data to the file
int MediaMan::save() {
    QFile file(filename);
    // check if the file exists, if it is writeable and truncate it
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        std::cerr << "Error opening file for writing: " << filename.toStdString() << std::endl;
        return -1;
    }
    // save each media object to the file
    for (const auto& media : this->media) {
        media->toFile(file);
    }
    return 0;
}

// get a media object by index in the vector
std::shared_ptr<Media> MediaMan::operator[](unsigned long idx) {
    if (idx < (unsigned long) media.size()) {
        return media[idx];
    }
    return nullptr;
}

// print the media objects in the vector
std::ostream& operator<<(std::ostream& os, const MediaMan& mm) {
    os << "=== MediaMan: " << mm.getFilename().toStdString() << " ===" << std::endl;
    os << "Next ID: " << mm.getNextId() << std::endl;
    os << "Total media items: " << mm.getAllMedia().size() << std::endl;
    os << "Media items:" << std::endl;
    for (const auto& media : mm.getAllMedia()) {
        os << *media << std::endl;
    }
    os << "=========== End MediaMan ===========" << std::endl;
    return os;
}