#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "library.h"
#include "mediaman.h"
#include "libitemman.h"
#include "personman.h"
#include "transactionman.h"


Library::Library(const QString& dir, bool load) : dir(dir) {
    mediaManager = std::make_unique<MediaMan>(dir + "/media.ndjson", load);
    libitemManager = std::make_unique<LibitemMan>(dir + "/libitem.ndjson", load);
    personManager = std::make_unique<PersonMan>(dir + "/person.ndjson", load);
    transactionManager = std::make_unique<TransactionMan>(dir + "/transaction.ndjson", load);
    // if (load) {
    //     this->load();
    // }
}
Library::~Library() {
    save();
}

int Library::setDir(const QString& dir) {
    this->dir = dir;
    return 0;
}
QString Library::getDir() const {
    return dir;
}
MediaMan* Library::getMediaManager() const {
    return mediaManager.get();
}
LibitemMan* Library::getLibitemManager() const {
    return libitemManager.get();
}
PersonMan* Library::getPersonManager() const {
    return personManager.get();
}
TransactionMan* Library::getTransactionManager() const {
    return transactionManager.get();
}

int Library::load() {
    if (!QDir(dir).exists()) {
        QDir().mkdir(dir);
    }
    mediaManager->load();
    libitemManager->load();
    personManager->load();
    transactionManager->load();
    return 0;
}
int Library::save() {
    mediaManager->save();
    libitemManager->save();
    personManager->save();
    transactionManager->save();
    return 0;
}

std::ostream& operator<<(std::ostream& os, const Library& lib) {
    os << lib.getDir().toStdString() << std::endl;
    os << *(lib.getMediaManager()) << std::endl;
    os << *(lib.getLibitemManager()) << std::endl;
    os << *(lib.getPersonManager()) << std::endl;
    os << *(lib.getTransactionManager()) << std::endl;
    return os;
}