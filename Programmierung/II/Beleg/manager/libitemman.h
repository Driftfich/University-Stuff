/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the LibitemMan class, which manages a collection of media library items in a higher abstraction level.
A hash map is used for fast access to libitems by id. The callback function onMediaChangeCallback is used to propagate the 
change of a media id to the next higher level (libitemtablemodel). More information about this can be found in libitem.h.
*/

#ifndef _LIBITEMMAN_H
#define _LIBITEMMAN_H

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>
#include <QString>
#include <QVector>
#include <QHash>
#include <iostream>

#include "libitem.h"

class LibitemMan {
    QString filename; // file to save the libitems to or load from
    QVector<std::shared_ptr<Libitem>> libitems; // vector to hold all libitems
    QHash<unsigned long, std::shared_ptr<Libitem>> libitem_map; // fast access to libitems by id
    unsigned long next_id; // next id for a new libitem
    std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> onMediaChangeCallback; // signal to notify when the media id of a libitem changes

    // set the next id for a new libitem. This is private to ensure the next id is always set correctly.
    void setNextId(unsigned long id) {
        next_id = id;
    }

    public:
        LibitemMan(QString filename, bool load = true);
        ~LibitemMan();

        int setFilename(const QString& filename);
        int addLibitem(std::shared_ptr<Libitem> libitem); // add a libitem to both the vector and the hash map
        int removeLibitemId(unsigned long id); // remove a libitem by id
        int removeLibitem(unsigned long index); // remove a libitem by index in the vector
        int setOnMediaChangeCallback(std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> callback) {
            onMediaChangeCallback = callback;
            return 0;
        }

        unsigned long getNextId() const;
        QVector<std::shared_ptr<Libitem>> getLibitems() const;
        std::shared_ptr<Libitem> getLibitem(unsigned long id) const;
        QString getFilename() const;

        int load();
        int save();

        std::shared_ptr<Libitem> operator[](unsigned long idx); // access libitem by index in the vector

        friend std::ostream& operator<<(std::ostream& os, const LibitemMan& lm);
};

#endif