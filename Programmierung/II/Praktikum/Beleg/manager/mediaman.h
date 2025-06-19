#ifndef _MEDIAMAN_H
#define _MEDIAMAN_H

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>
#include <QString>
#include <QVector>
#include <QHash>
#include <iostream>
#include "media.h"

class MediaMan {
    QString filename;
    QVector<std::shared_ptr<Media>> media;
    QHash<unsigned long, std::shared_ptr<Media>> media_map; // fast access to media by id
    unsigned long next_id;
    void setNextId(unsigned long id) {
        next_id = id;
    }

    public:
        MediaMan(QString filename, bool load = true);
        ~MediaMan();

        int setFilename(const QString& filename);
        int addMedia(std::shared_ptr<Media> media);
        int removeMedia(unsigned long id);

        unsigned long getNextId() const;
        QVector<std::shared_ptr<Media>> getAllMedia() const;
        std::shared_ptr<Media> getMedia(unsigned long id) const;
        QString getFilename() const;

        int load();
        int save();

        std::shared_ptr<Media> operator[](unsigned long idx);

        friend std::ostream& operator<<(std::ostream& os, const MediaMan& mm);
};

#endif