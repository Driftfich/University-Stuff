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

LibitemMan::LibitemMan(QString filename) {
    setNextId(0);
    setFilename(filename);
    load(); // internally the load function sets the next id with the last id + 1
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
int LibitemMan::removeLibitem(unsigned long id) {
    for (auto it = libitems.begin(); it != libitems.end(); ++it) {
        if ((*it)->getId() == id) {
            libitems.erase(it);
            libitem_map.remove(id);
            return 0;
        }
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
    auto it = libitem_map.find(id);
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
        std::shared_ptr<Libitem> libitem = Libitem::fromFile(file);
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
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
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
    os << "LibitemMan: " << lm.getFilename().toStdString() << std::endl;
    os << "Next ID: " << lm.getNextId() << std::endl;
    os << "Libitems: " << std::endl;
    for (const auto& libitem : lm.getLibitems()) {
        os << *libitem << std::endl;
    }
    return os;
}