#ifndef _AUDIO_H
#define _AUDIO_H

#include <vector>
#include <string>
#include "config.h"
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
        int setType(const string& type);
        int setBitrate(unsigned int bitrate) {this->bitrate = bitrate;}
        int setSampleRate(unsigned int sample_rate) {this->sample_rate = sample_rate;}
        int setChannels(const string& channels);
        int setCodec(const string& codec);
        int setAlbum(const string& album);
        int setTracks(const vector<string>& tracks);

        unsigned int getDuration() const {return this->duration;}
        string getType() const {return this->type;}
        unsigned int getBitrate() const {return this->bitrate;}
        unsigned int getSampleRate() const {return this->sample_rate;}
        string getChannels() const {return this->channels;}
        QString getCodec() const {return this->codec;}
        QString getAlbum() const {return this->album;}
        QVector<QString> getTracks() const {return this->tracks;}

        // constructor using setters
        Audio(unsigned long id, const QString& title, const std::chrono::year_month_day publication_date,
              const QVector<int>& artist_ids, const QString& publisher, const QString& description,
              const QString& genre, const QVector<QString>& languages, unsigned int available_copies,
              unsigned int duration, const QString& type, unsigned int bitrate, unsigned int sample_rate,
              const QString& channels, const QString& codec, const QString& album,
              const QVector<QString>& tracks) :
              Media(id, title, publication_date, artist_ids, publisher, description, genre, languages, available_copies) {
            setDuration(duration);
            setType(type);
            setBitrate(bitrate);
            setSampleRate(sample_rate);
            setChannels(channels);
            setCodec(codec);
            setAlbum(album);
            setTracks(tracks);
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
}

#endif