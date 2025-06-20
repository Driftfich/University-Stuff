/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the library class, which holds the manager classes for media, libitems, persons, and transactions
at one place. Used to load and save the data from and to files at once.
*/

#include <iostream>
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

// create the manager classes with the given directory and load them if specified
Library::Library(const QString& dir, bool load) : dir(dir) {
    mediaManager = std::make_unique<MediaMan>(dir + "/media.ndjson", load);
    libitemManager = std::make_unique<LibitemMan>(dir + "/libitem.ndjson", load);
    personManager = std::make_unique<PersonMan>(dir + "/person.ndjson", load);
    transactionManager = std::make_unique<TransactionMan>(dir + "/transaction.ndjson", load);
}

// destructor saves the data to the files
Library::~Library() {
    save();
}

// setter method for the directory
int Library::setDir(const QString& dir) {
    this->dir = dir;
    return 0;
}

// getter methods
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

// load and save methods for the library
int Library::load() {
    // ensure the directory exists, if not create it
    if (!QDir(dir).exists()) {
        QDir().mkdir(dir);
    }
    // load the managers
    mediaManager->load();
    libitemManager->load();
    personManager->load();
    transactionManager->load();
    return 0;
}
int Library::save() {
    // ensure the directory exists, if not create it
    if (!QDir(dir).exists()) {
        QDir().mkdir(dir);
    }
    // save the managers
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