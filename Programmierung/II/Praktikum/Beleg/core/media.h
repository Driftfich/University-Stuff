#ifndef _MEDIA_H
#define _MEDIA_H

#include <iostream>
#include <vector>
#include <string>
#include <QDate>
#include <QString>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <variant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "person.h" // for person work with files
#include "json.hpp" // for json work with files
#include "config.h"

class Media {
    unsigned long id;
    QString title;
    QDate publication_date;
    QVector<unsigned long> artist_ids; // array persons ids => Information about saved in Person class
    // QVector<Person*> artists; // array of deserialized artists/persons (person.h) objects
    QString publisher;
    QString description;
    QString genre;
    QVector<QString> languages;
    // unsigned int available_copies;
    // metadata map with string to T map
    QMap<QString, QVariant> metadata;

    public:
        // setter methods
        void setId(unsigned long id) {this->id = id;}
        int setTitle(const QString& title);
        int setPublicationDate(const QDate publication_date);
        int setArtistIds(const QVector<unsigned long>& artist_ids);
        int setPublisher(const QString& publisher);
        int setDescription(const QString& description);
        int setGenre(const QString& genre);
        int setLanguages(const QVector<QString>& languages);
        // void setAvailableCopies(unsigned int available_copies) {this->available_copies = available_copies;}
        int setMetadata(const QMap<QString, QVariant>& metadata);

        // getter methods
        unsigned long getId() const {return this->id;}
        QString getTitle() const {return this->title;}
        QDate getPublicationDate() const {return this->publication_date;}
        QVector<unsigned long> getArtistIds() const {return this->artist_ids;}
        QString getPublisher() const {return this->publisher;}
        QString getDescription() const {return this->description;}
        QString getGenre() const {return this->genre;}
        QVector<QString> getLanguages() const {return this->languages;}
        // unsigned int getAvailableCopies() const {return this->available_copies;}
        QMap<QString, QVariant> getMetadata() const {return this->metadata;}

        // constructor using setters
        Media(unsigned long id, const QString& title, const QDate publication_date,
              const QVector<unsigned long>& artist_ids, const QString& publisher, const QString& description,
              const QString& genre, const QVector<QString>& languages,
            //    unsigned int available_copies,
              const QMap<QString, QVariant>& metadata) {
            setId(id);
            setTitle(title);
            setPublicationDate(publication_date);
            setArtistIds(artist_ids);
            setPublisher(publisher);
            setDescription(description);
            setGenre(genre);
            setLanguages(languages);
            // setAvailableCopies(available_copies);
            setMetadata(metadata);
        }

        // constructor for loading from file
        Media(QJsonObject json) {
            if (loadLocalParams(json["media"].toObject()) != 0) {
                throw std::runtime_error("Issues loading media parameters");
            }
        }

        // copy constructor
        Media(const Media& other) {
            this->id = other.id;
            this->title = other.title;
            this->publication_date = other.publication_date;
            this->artist_ids = other.artist_ids;
            this->publisher = other.publisher;
            this->description = other.description;
            this->genre = other.genre;
            this->languages = other.languages;
            // this->available_copies = other.available_copies;
            this->metadata = other.metadata;
        }

        // assignment operator
        Media& operator=(const Media& other) {
            if (this != &other) {
                this->id = other.id;
                this->title = other.title;
                this->publication_date = other.publication_date;
                this->artist_ids = other.artist_ids;
                this->publisher = other.publisher;
                this->description = other.description;
                this->genre = other.genre;
                this->languages = other.languages;
                // this->available_copies = other.available_copies;
                this->metadata = other.metadata;
            }
            return *this;
        }

        // destructor
        virtual ~Media() {
            // Destructor logic if needed
        }

        // method to retrieve the subclass type
        virtual QString getSubclassType() const {return "Media";} // used to identify if it is a text, audio, video, etc.
        // get json object from local media object
        QJsonObject getLocalParams() const;
        virtual QJsonObject getSubclassParams() const {return QJsonObject();}
        // get json object including subclass and local params
        QJsonObject getJson() const;
        // write json object to file
        void toFile(QFile& file) const;

        // load json object from file
        int loadLocalParams(const QJsonObject& json);
        // read json object from file
        virtual int loadSubclassParams(const QJsonObject& json) {Q_UNUSED(json); return 0;}
        // load json object from file
        static std::shared_ptr<Media> fromFile(QFile& file);
        // factory method to create the correct subclass object
        static std::shared_ptr<Media> MediaFactory(const QJsonObject& json);

        // print methods
        void printbase(std::ostream& os) const;
        virtual void printsubclass(std::ostream& os) const;
        friend std::ostream& operator<<(std::ostream& os, const Media& media);

};

#endif