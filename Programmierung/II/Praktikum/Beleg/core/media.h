/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the Media class, which holds information and logic related to media items.

Note:
Because a virtual method cannot be static at the same time, virtual static getSubclassSchema() wasnt possible. As I strictly needed a static version
to get the schema for new, uninitialized media items, I had to use a non-virtual setup. This means, to get the correct schema for a media item,
the corresponding getSchema() method has to be called on the corresponding class or subclass.
If static isnt needed, the helper method getSchemaByType() can be used, which automatically retrieves the schema for the correct subclass type.
*/

#ifndef _MEDIA_H
#define _MEDIA_H

#include <iostream>
#include <QDate>
#include <QString>
#include <QVector>
#include <QMap>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

class Media {
    unsigned long id;
    QString title;
    QDate publication_date;
    QVector<unsigned long> artist_ids; // array persons ids => information about artists is stored in the person class
    QString publisher;
    QString description;
    QString genre;
    QVector<QString> languages;
    QMap<QString, QVariant> metadata;
    int refCount = 0; // how many libitems reference this media item, used to determine if the media item can be deleted

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
        int setMetadata(const QMap<QString, QVariant>& metadata);
        void setRefCount(int refCount) {this->refCount = refCount;}

        // getter methods
        unsigned long getId() const {return this->id;}
        QString getTitle() const {return this->title;}
        QDate getPublicationDate() const {return this->publication_date;}
        QVector<unsigned long> getArtistIds() const {return this->artist_ids;}
        QString getPublisher() const {return this->publisher;}
        QString getDescription() const {return this->description;}
        QString getGenre() const {return this->genre;}
        QVector<QString> getLanguages() const {return this->languages;}
        QMap<QString, QVariant> getMetadata() const {return this->metadata;}
        int getRefCount() const {return this->refCount;}

        // constructor using setters
        Media(unsigned long id, const QString& title, const QDate publication_date,
              const QVector<unsigned long>& artist_ids, const QString& publisher, const QString& description,
              const QString& genre, const QVector<QString>& languages,
              const QMap<QString, QVariant>& metadata) {
            setId(id);
            setTitle(title);
            setPublicationDate(publication_date);
            setArtistIds(artist_ids);
            setPublisher(publisher);
            setDescription(description);
            setGenre(genre);
            setLanguages(languages);
            setMetadata(metadata);
        }

        // constructor for loading from JSON object
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
                this->metadata = other.metadata;
            }
            return *this;
        }

        // destructor
        virtual ~Media() {
        }

        // method to retrieve the subclass type
        virtual QString getSubclassType() const {return "Media";} // used to identify if it is a text, audio, video, etc.
        // get json object from local media object
        QJsonObject getLocalParams() const;
        // get json object from subclass e.g. Text, Audio, Video
        virtual QJsonObject getSubclassParams() const {return QJsonObject();}
        // get json object including subclass and local params
        QJsonObject getJson() const;
        // write json object to file
        void toFile(QFile& file) const;

        // load local parameters from a JSON object
        int loadLocalParams(const QJsonObject& json);
        // read subclass parameters from a JSON object
        virtual int loadSubclassParams(const QJsonObject& json) {Q_UNUSED(json); return 0;}
        // load complete media object from file
        static std::shared_ptr<Media> fromFile(QFile& file);
        // factory method to create the correct subclass object based on the media type in the JSON object
        static std::shared_ptr<Media> MediaFactory(const QJsonObject& json);

        // schema methods
        // get local schema for the media class
        static QJsonObject getLocalSchema();
        // get schema for the media class. To get the complete schema, use the corresponding subclass method
        static QJsonObject getSchema();
        // automatically gets the schema for the correct subclass type, but isnt static any more
        QJsonObject getSchemaByType() const;

        // print methods
        void printbase(std::ostream& os) const;
        virtual void printsubclass(std::ostream& os) const;
        friend std::ostream& operator<<(std::ostream& os, const Media& media);

};

#endif