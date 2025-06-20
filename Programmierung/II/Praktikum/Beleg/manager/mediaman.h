/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the MediaMan class, which holds the media objects and provides methods to load and save the data from and to files.
Uses a hash map for fast access to media objects by id.
It also provides methods to add, remove and get media objects by id.
*/

#ifndef _MEDIAMAN_H
#define _MEDIAMAN_H

#include <iostream>
#include <QVector>
#include <QHash>
#include <QString>

#include "media.h"

class MediaMan {
    QString filename;
    QVector<std::shared_ptr<Media>> media;
    QHash<unsigned long, std::shared_ptr<Media>> media_map; // fast access to media by id
    unsigned long next_id;

    // this method is private, because the next id is updated internally
    void setNextId(unsigned long id) {
        next_id = id;
    }

    public:
        MediaMan(QString filename, bool load = true);
        ~MediaMan();

        // setter for the filename
        int setFilename(const QString& filename);

        // add and remove media objects
        int addMedia(std::shared_ptr<Media> media);
        int removeMedia(unsigned long id);

        // getters
        unsigned long getNextId() const;
        QVector<std::shared_ptr<Media>> getAllMedia() const;
        std::shared_ptr<Media> getMedia(unsigned long id) const;
        QString getFilename() const;

        // load and save
        int load();
        int save();

        // get a media object by index in the vector
        std::shared_ptr<Media> operator[](unsigned long idx);

        // print the all the media objects in the vector
        friend std::ostream& operator<<(std::ostream& os, const MediaMan& mm);
};

#endif