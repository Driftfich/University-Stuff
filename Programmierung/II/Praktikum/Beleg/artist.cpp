#include <iostream>
#include <string>
#include <vector>
#include <QString>
#include <QVector>
#include "media.h"
#include "artist.h"

int Artist::setArtistType(const QString& artist_type) {
    this->artist_type = artist_type;
    return 0;
}

int Artist::setMediaIds(const QVector<unsigned long>& media_ids) {
    this->media_ids = media_ids;
    return 0;
}