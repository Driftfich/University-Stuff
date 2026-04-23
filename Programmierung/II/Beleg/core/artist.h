/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the Artist class, which holds information and logic related to artists.
*/

#ifndef _ARTIST_H
#define _ARTIST_H

#include <QString>
#include <QVector>
#include <QDate>
#include <QVector>
#include "returns.h"

class Artist {
    QString artist_type; // e.g. actor, director, musician, etc.
    // QVector<unsigned long> media_ids; // array of media ids => works created by the artist

    public:
        // setter methods
        int setArtistType(const QString& artist_type);
        // int setMediaIds(const QVector<unsigned long>& media_ids);

        // getter methods
        QString getArtistType() const {return this->artist_type;}
        // QVector<unsigned long> getMediaIds() const {return this->media_ids;}

        // constructor using setters
        Artist(const QString& artist_type) { // , const QVector<unsigned long>& media_ids
            setArtistType(artist_type);
            // setMediaIds(media_ids);
        }

        // copy constructor
        Artist(const Artist& other) {
            this->artist_type = other.artist_type;
            // this->media_ids = other.media_ids;
        }

        // copy assignment operator
        Artist& operator=(const Artist& other) {
            if (this != &other) {
                this->artist_type = other.artist_type;
                // this->media_ids = other.media_ids;
            }
            return *this;
        }

        // destructor
        ~Artist() {
        }

        // serialization methods
        QString getSubclassType() const { return "Artist"; }
        QJsonObject getSubclassParams() const;
        Result loadSubclassParams(const QJsonObject& json);

        // get the schema for this class. Used to build comprehensive form in infopanel
        static QJsonObject getSubclassSchema(bool checked = false);

        // constructor for loading from JSON
        Artist(const QJsonObject& json) {
            if (loadSubclassParams(json) != 0) {
                throw std::runtime_error("Issue loading subclass parameters for Artist");
            }
        }

};

#endif