#ifndef _ARTIST_H
#define _ARTIST_H

#include <iostream>
#include <string>
#include <vector>
// #include <QApplication>
#include <QString>
#include <QVector>

#include "config.h"
#include "person.h"

class Artist : public Person {
    QString artist_type; // e.g. actor, director, musician, etc.
    QVector<int> media_ids; // array of media ids => works created by the artist
    QVector<Media*> media; // array of deserialized media objects

    public:
        // setter methods
        int setArtistType(const QString& artist_type);
        int setMediaIds(const QVector<int>& media_ids);

        // getter methods
        QString getArtistType() const {return this->artist_type;}
        QVector<int> getMediaIds() const {return this->media_ids;}
        QVector<Media*> getMedia() const {return this->media;}

        // constructor using setters
        Artist(unsigned long id, const QString& fname, const QString& lname, const QString& ename,
               const std::chrono::year_month_day birthday,
               const QString& artist_type, const QVector<int>& media_ids,) :
               Person(id, fname, lname, ename, birthday) {
            setArtistType(artist_type);
            setMediaIds(media_ids);
            this->media = QVector<Media*>();
        }

        // copy constructor
        Artist(const Artist& other) : Person(other) {
            this->artist_type = other.artist_type;
            this->media_ids = other.media_ids;
            this->media = other.media;
        }

        // copy assignment operator
        Artist& operator=(const Artist& other) {
            if (this != &other) {
                Person::operator=(other); // Call base class assignment operator
                this->artist_type = other.artist_type;
                this->media_ids = other.media_ids;
                this->media = other.media;
            }
            return *this;
        }

        // destructor
        ~Artist() {
            // Destructor logic if needed
            // Note: Media objects are not deleted here, as they are managed elsewhere
        }



}


#endif