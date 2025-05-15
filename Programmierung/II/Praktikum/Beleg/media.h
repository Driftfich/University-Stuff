#ifndef _MEDIA_H
#define _MEDIA_H

#include <iostream>
#include <vector>
#include <string>
#include <QDate>
#include <QString>
#include <QVector>
#include <QMap>
#include <variant>
#include "json.hpp" // for json work with files
#include "config.h"

class Media {
    unsigned long id;
    QString title;
    QDate publication_date;
    QVector<int> artist_ids; // array persons ids => Information about saved in Person class
    QVector<Person*> artists; // array of deserialized artists/persons (person.h) objects
    QString publisher;
    QString description;
    QString genre;
    QVector<QString> languages;
    // unsigned int available_copies;
    // metadata map with string to T map
    QMap<QString, std::variant<int, float, QString>> metadata;

    public:
        // setter methods
        void setId(unsigned long id) {this->id = id;}
        int setTitle(const QString& title);
        int setPublicationDate(const QDate publication_date);
        int setArtistIds(const QVector<int>& artist_ids);
        int setPublisher(const QString& publisher);
        int setDescription(const QString& description);
        int setGenre(const QString& genre);
        int setLanguages(const QVector<QString>& languages);
        // void setAvailableCopies(unsigned int available_copies) {this->available_copies = available_copies;}
        int setMetadata(const QMap<QString, std::variant<int, float, QString>>& metadata);

        // getter methods
        unsigned long getId() const {return this->id;}
        QString getTitle() const {return this->title;}
        QDate getPublicationDate() const {return this->publication_date;}
        QVector<int> getArtistIds() const {return this->artist_ids;}
        QString getPublisher() const {return this->publisher;}
        QString getDescription() const {return this->description;}
        QString getGenre() const {return this->genre;}
        QVector<QString> getLanguages() const {return this->languages;}
        // unsigned int getAvailableCopies() const {return this->available_copies;}
        QMap<QString, std::variant<int, float, QString>> getMetadata() const {return this->metadata;}
        
        // constructor using setters
        Media(unsigned long id, const QString& title, const QDate publication_date,
              const QVector<int>& artist_ids, const QString& publisher, const QString& description,
              const QString& genre, const QVector<QString>& languages,
            //    unsigned int available_copies,
              const QMap<QString, std::variant<int, float, QString>>& metadata) {
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
            this->artists = QVector<Person*>();
        }

        // copy constructor
        Media(const Media& other) {
            this->id = other.id;
            this->title = other.title;
            this->publication_date = other.publication_date;
            this->artists = other.artists;
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
                this->artists = other.artists;
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
        virtual QString getSubclassType() const = "Media"; // used to identify if it is a text, audio, video, etc.
        //
        
        // METHODS FOR LOADING & SAVING MEDIA data
        // get json object from subclasses
        virtual nlohmann::json getSubclassParams() const = nlohmann::json::object();
        // get json object from local media object
        nlohmann::json getLocalParams();
        // get json object including subclass and local params
        nlohmann::json getJson() const;
        // write json object to file
        void writeJsonToFile(FILE *file) const;

        // read json object from file
         virtual void loadFromFile(FILE* file) = 0;
        // factory method to create the correct subclass object
        std::shared_ptr<Media> MediaFactory(FILE *file) const;

}

#endif