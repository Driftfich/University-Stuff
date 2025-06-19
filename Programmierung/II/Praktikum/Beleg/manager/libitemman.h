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
    QString filename;
    QVector<std::shared_ptr<Libitem>> libitems;
    QHash<unsigned long, std::shared_ptr<Libitem>> libitem_map; // fast access to libitems by id
    unsigned long next_id;
    std::function<void(unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId)> onMediaChangeCallback;

    void setNextId(unsigned long id) {
        next_id = id;
    }

    public:
        LibitemMan(QString filename, bool load = true);
        ~LibitemMan();

        int setFilename(const QString& filename);
        int addLibitem(std::shared_ptr<Libitem> libitem);
        int removeLibitemId(unsigned long id);
        int removeLibitem(unsigned long index);
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

        std::shared_ptr<Libitem> operator[](unsigned long idx);

        friend std::ostream& operator<<(std::ostream& os, const LibitemMan& lm);
};

#endif