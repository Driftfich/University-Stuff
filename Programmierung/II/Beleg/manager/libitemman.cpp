/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the LibitemMan class, which manages a collection of media library items in a higher abstraction level.
*/

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>
#include <QString>
#include <QVector>
#include <QHash>
#include <iostream>

#include "libitem.h"
#include "libitemman.h"

LibitemMan::LibitemMan(QString filename, bool load) {
    setNextId(0);
    setFilename(filename);
    if (load) {
        this->load();
    }
}

LibitemMan::~LibitemMan() {
    save();
}

int LibitemMan::setFilename(const QString& filename) {
    this->filename = filename;
    return 0;
}

int LibitemMan::addLibitem(std::shared_ptr<Libitem> libitem) {
    if (libitem) {
        this->libitems.push_back(libitem);
        // if the libitem has a higher id than the current next_id, update next_id
        // when the libitem id is lower, it is assumed that the id is unused
        if (libitem->getId() >= next_id) {
            setNextId(libitem->getId() + 1);
        }
        libitem_map.insert(libitem->getId(), libitem);
        return 0;
    }
    return -1;
}

int LibitemMan::removeLibitemId(unsigned long id) {
    // find the libitem with the given id in the vector and remove it
    for (QVector<std::shared_ptr<Libitem>>::iterator it = libitems.begin(); it != libitems.end(); ++it) {
        if ((*it)->getId() == id) {
            libitems.erase(it);
            libitem_map.remove(id);
            return 0;
        }
    }
    return -1;
}

int LibitemMan::removeLibitem(unsigned long index) {
    // erase the libitem at the given index from the vector and remove it from the hash map
    if (index < (unsigned long) libitems.size()) {
        unsigned long id = libitems[index]->getId();
        libitems.erase(libitems.begin() + index);
        libitem_map.remove(id);
        return 0;
    }
    return -1;
}

unsigned long LibitemMan::getNextId() const {
    return next_id;
}
QVector<std::shared_ptr<Libitem>> LibitemMan::getLibitems() const {
    return libitems;
}

std::shared_ptr<Libitem> LibitemMan::getLibitem(unsigned long id) const {
    // fast access to libitems by id using the hash map
    QHash<unsigned long, std::shared_ptr<Libitem>>::const_iterator it = libitem_map.find(id);
    if (it != libitem_map.end()) {
        return it.value();
    }
    return nullptr;
}

QString LibitemMan::getFilename() const {
    return filename;
}

int LibitemMan::load() {
    QFile file(filename);
    // check if the file exists
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error opening file for reading: " << filename.toStdString() << std::endl;
        return -1;
    }
    // read the file line by line and create libitems from it
    while (!file.atEnd()) {
        std::shared_ptr<Libitem> libitem = Libitem::fromFile(file, [this](unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId) {
            // connect the onMediaChangeCallback to the libitem's media change signal
            if (onMediaChangeCallback) {
                onMediaChangeCallback(libitemId, oldMediaId, newMediaId);
            }
        });
        if (libitem) {
            addLibitem(libitem);
        } else {
            std::cerr << "Error loading libitem from file" << std::endl;
            break;
        }
    }
    return 0;
}

int LibitemMan::save() {
    QFile file(filename);
    // open the file for writing, truncating it if it already exists
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        std::cerr << "Error opening file for writing: " << filename.toStdString() << std::endl;
        return -1;
    }
    // write each libitem to the file
    for (const auto& libitem : libitems) {
        libitem->toFile(file);
    }
    return 0;
}

std::shared_ptr<Libitem> LibitemMan::operator[](unsigned long idx) {
    if (idx < (unsigned long) libitems.size()) {
        return libitems[idx];
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const LibitemMan& lm) {
    os << "====== LibitemMan: " << lm.getFilename().toStdString() << " ======" << std::endl;
    os << "Next ID: " << lm.getNextId() << std::endl;
    os << "Libitems: " << std::endl;
    os << "-------------------------" << std::endl;
    for (const auto& libitem : lm.getLibitems()) {
        os << *libitem << std::endl;
    }
    os << "======= End of LibitemMan =======" << std::endl;
    return os;
}