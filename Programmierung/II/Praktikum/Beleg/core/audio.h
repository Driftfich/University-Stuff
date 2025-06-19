#ifndef _AUDIO_H
#define _AUDIO_H

#include <QString>
#include <QVector>
#include <QDate>
#include <vector>
#include <string>
#include "media.h"

class Audio : public Media {
    unsigned int duration; // in seconds
    QString type; // e.g. podcast, audiobook, music
    unsigned int bitrate; // in kbps
    unsigned int sample_rate; // in kHz
    QString channels; // mono, stereo, etc.
    QString codec; // e.g. MP3, AAC, FLAC, etc.
    QString album; // album name
    QVector<QString> tracks;

    public:
        void setDuration(unsigned int duration) {this->duration = duration;}
        int setBitrate(unsigned int bitrate) {this->bitrate = bitrate; return 0;}
        int setSampleRate(unsigned int sample_rate) {this->sample_rate = sample_rate; return 0;}
        int setChannels(const QString& channels);
        int setCodec(const QString& codec);
        int setAlbum(const QString& album);
        int setTracks(const QVector<QString>& tracks);
        int setType(const QString& type);

        unsigned int getDuration() const {return this->duration;}
        QString getType() const {return this->type;}
        unsigned int getBitrate() const {return this->bitrate;}
        unsigned int getSampleRate() const {return this->sample_rate;}
        QString getChannels() const {return this->channels;}
        QString getCodec() const {return this->codec;}
        QString getAlbum() const {return this->album;}
        QVector<QString> getTracks() const {return this->tracks;}

        // constructor using setters
        Audio(unsigned long id, const QString& title, const QDate publication_date,
              const QVector<unsigned long>& artist_ids, const QString& publisher, const QString& description,
              const QString& genre, const QVector<QString>& languages, const QMap<QString, QVariant>& metadata,
              unsigned int duration, const QString& type, unsigned int bitrate, unsigned int sample_rate,
              const QString& channels, const QString& codec, const QString& album,
              const QVector<QString>& tracks) :
              Media(id, title, publication_date, artist_ids, publisher, description, genre, languages, metadata) {
            setDuration(duration);
            setType(type);
            setBitrate(bitrate);
            setSampleRate(sample_rate);
            setChannels(channels);
            setCodec(codec);
            setAlbum(album);
            setTracks(tracks);
        }

        Audio(const QJsonObject& json) : Media(json) {
            if (loadSubclassParams(json["subclass_params"].toObject()) != 0) {
                throw std::runtime_error("Issue loading subclass parameters for Audio");
            }
        }

        // copy constructor
        Audio(const Audio& other) : Media(other) {
            this->duration = other.duration;
            this->type = other.type;
            this->bitrate = other.bitrate;
            this->sample_rate = other.sample_rate;
            this->channels = other.channels;
            this->codec = other.codec;
            this->album = other.album;
            this->tracks = other.tracks;
        }

        // copy assignment operator
        Audio& operator=(const Audio& other) {
            if (this != &other) {
                Media::operator=(other); // Call base class assignment operator
                this->duration = other.duration;
                this->type = other.type;
                this->bitrate = other.bitrate;
                this->sample_rate = other.sample_rate;
                this->channels = other.channels;
                this->codec = other.codec;
                this->album = other.album;
                this->tracks = other.tracks;
            }
            return *this;
        }

        // destructor
        ~Audio() {
            // Destructor logic if needed
            // Note: Media objects are not deleted here, as they are managed elsewhere
        }

        // methods for loading & saving audio data
        QString getSubclassType() const override {return "Audio";} // used to identify it is a audio

        QJsonObject getSubclassParams() const override;

        static QJsonObject getSchema();
        static QJsonObject getSubclassSchema();

        int loadSubclassParams(const QJsonObject& json) override;
};

#endif