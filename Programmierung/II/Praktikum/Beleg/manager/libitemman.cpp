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
        this->load(); // internally the load function sets the next id with the last id + 1
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
        if (libitem->getId() >= next_id) {
            setNextId(libitem->getId() + 1);
        }
        libitem_map.insert(libitem->getId(), libitem);
        return 0;
    }
    return -1;
}
int LibitemMan::removeLibitemId(unsigned long id) {
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
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error opening file for reading: " << filename.toStdString() << std::endl;
        return -1;
    }
    while (!file.atEnd()) {
        std::shared_ptr<Libitem> libitem = Libitem::fromFile(file, [this](unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId) {
            // call the own callback function
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
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        std::cerr << "Error opening file for writing: " << filename.toStdString() << std::endl;
        return -1;
    }
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