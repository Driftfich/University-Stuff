/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the Video class, which holds information and logic related to video media.
*/

#ifndef _VIDEO_H
#define _VIDEO_H

#include <QString>
#include <QVector>
#include <QDate>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>

#include "media.h"
#include "returns.h"

class Video : public Media {
    unsigned int duration; // in seconds
    QString type; // e.g. movie, series, documentary
    unsigned int resolution; // e.g. 1080, 2160 (4K), 4320 (8K)
    QString codec; // e.g. H.264, H.265, VP9
    QString aspect_ratio; // e.g. 16:9, 4:3
    QString audio_channels; // e.g. stereo, 5.1, 7.1

    public:
        void setDuration(unsigned int duration) {this->duration = duration;}
        void setResolution(unsigned int resolution) {this->resolution = resolution;}
        int setType(const QString& type);
        int setCodec(const QString& codec);
        int setAspectRatio(const QString& aspect_ratio);
        int setAudioChannels(const QString& audio_channels);

        unsigned int getDuration() const {return this->duration;}
        QString getType() const {return this->type;}
        unsigned int getResolution() const {return this->resolution;}
        QString getCodec() const {return this->codec;}
        QString getAspectRatio() const {return this->aspect_ratio;}
        QString getAudioChannels() const {return this->audio_channels;}

        // constructor using setters
        Video(unsigned long id, const QString& title, const QDate publication_date,
              const QVector<unsigned long>& artist_ids, const QString& publisher, const QString& description,
              const QString& genre, const QVector<QString>& languages, QMap<QString, QVariant> metadata,
              unsigned int duration, const QString& type, unsigned int resolution,
              const QString& codec, const QString& aspect_ratio, const QString& audio_channels)
            : Media(id, title, publication_date, artist_ids, publisher, description, genre, languages, metadata) {
            setDuration(duration);
            setType(type);
            setResolution(resolution);
            setCodec(codec);
            setAspectRatio(aspect_ratio);
            setAudioChannels(audio_channels);
        }

        Video(const QJsonObject& json) : Media(json) {
            if (loadSubclassParams(json["subclass_params"].toObject()) != 0) {
                throw std::runtime_error("Issue loading subclass parameters for Video");
            }
        }

        // copy constructor
        Video(const Video& other) : Media(other) {
            this->duration = other.duration;
            this->type = other.type;
            this->resolution = other.resolution;
            this->codec = other.codec;
            this->aspect_ratio = other.aspect_ratio;
            this->audio_channels = other.audio_channels;
        }

        // copy assignment operator
        Video& operator=(const Video& other) {
            if (this != &other) {
                Media::operator=(other); // Call base class assignment operator
                this->duration = other.duration;
                this->type = other.type;
                this->resolution = other.resolution;
                this->codec = other.codec;
                this->aspect_ratio = other.aspect_ratio;
                this->audio_channels = other.audio_channels;
            }
            return *this;
        }

        // destructor
        ~Video() {
        }

        // methods for loading & saving text data
        QString getSubclassType() const override {return "Video";} // used to identify it is a video

        QJsonObject getSubclassParams() const override;

        static QJsonObject getSubclassSchema();
        static QJsonObject getSchema();

        Result loadSubclassParams(const QJsonObject& json) override;
};

#endif