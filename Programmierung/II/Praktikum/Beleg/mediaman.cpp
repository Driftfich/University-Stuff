#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>
#include <QString>
#include <QVector>
#include <iostream>
#include "mediaman.h"
#include "media.h"

MediaMan::MediaMan(QString filename) {
    setNextId(0);
    setFilename(filename);
    load(); // internally the load function sets the next id with the last id + 1
}

MediaMan::~MediaMan() {
    save();
}

int MediaMan::setFilename(const QString& filename) {
    this->filename = filename;
    return 0;
}
int MediaMan::addMedia(std::shared_ptr<Media> media) {
    if (media) {
        this->media.push_back(media);
        return 0;
    }
    return -1;
}

int MediaMan::removeMedia(unsigned long id) {
    for (auto it = media.begin(); it != media.end(); ++it) {
        if ((*it)->getId() == id) {
            media.erase(it);
            return 0;
        }
    }
    return -1;
}

unsigned long MediaMan::getNextId() const {
    return next_id;
}
QVector<std::shared_ptr<Media>> MediaMan::getMedia() const {
    return media;
}
QString MediaMan::getFilename() const {
    return filename;
}

int MediaMan::load() {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error opening file for reading: " << filename.toStdString() << std::endl;
        return -1;
    }
    unsigned long max_id = 0;
    while (!file.atEnd()) {
        std::shared_ptr<Media> media = Media::fromFile(file);
        if (media) {
            this->media.push_back(media);
            if (media->getId() > max_id) {
                max_id = media->getId();
            }
        } else {
            std::cerr << "Error loading media from file" << std::endl;
            break;
        }
    }
    setNextId(max_id + 1);
    return 0;
}

int MediaMan::save() {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Error opening file for writing: " << filename.toStdString() << std::endl;
        return -1;
    }
    for (const auto& media : this->media) {
        media->toFile(file);
    }
    return 0;
}

std::shared_ptr<Media> MediaMan::operator[](unsigned long idx) {
    if (idx < (unsigned long) media.size()) {
        return media[idx];
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const MediaMan& mm) {
    os << "=== MediaMan: " << mm.getFilename().toStdString() << " ===" << std::endl;
    os << "Next ID: " << mm.getNextId() << std::endl;
    os << "Total media items: " << mm.getMedia().size() << std::endl;
    os << "Media items:" << std::endl;
    for (const auto& media : mm.getMedia()) {
        os << *media << std::endl;
    }
    os << "=========================" << std::endl;
    return os;
}