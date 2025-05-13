#ifndef _VIDEO_H
#define _VIDEO_H

#include <vector>
#include <string>
#include "config.h"
#include "media.h"

class Video : public Media {
    unsigned int duration; // in seconds
    std::string type; // e.g. movie, series, documentary
    unsigned int resolution; // e.g. 1080p, 4K
    std::string codec; // e.g. H.264, H.265, VP9
    std::string aspect_ratio; // e.g. 16:9, 4:3
    std::string audio_channels; // e.g. stereo, 5.1, 7.1

    public:
        void setDuration(unsigned int duration) {this->duration = duration;}
        int setType(const std::string& type);
        int setResolution(unsigned int resolution) {this->resolution = resolution;}
        int setCodec(const std::string& codec) {this->codec = codec;}
        int setAspectRatio(const std::string& aspect_ratio) {this->aspect_ratio = aspect_ratio;}
        int setAudioChannels(const std::string& audio_channels) {this->audio_channels = audio_channels;}

        unsigned int getDuration() const {return this->duration;}
        std::string getType() const {return this->type;}
        unsigned int getResolution() const {return this->resolution;}
        std::string getCodec() const {return this->codec;}
        std::string getAspectRatio() const {return this->aspect_ratio;}
        std::string getAudioChannels() const {return this->audio_channels;}

        // constructor using setters
        Video(unsigned long id, const std::string& title, const std::chrono::year_month_day publication_date,
              const std::vector<int>& artist_ids, const std::string& publisher, const std::string& description,
              const std::string& genre, const std::vector<std::string>& languages, unsigned int available_copies,
              unsigned int duration, const std::string& type, unsigned int resolution,
              const std::string& codec, const std::string& aspect_ratio, const std::string& audio_channels) :
              Media(id, title, publication_date, artist_ids, publisher, description, genre, languages, available_copies) {
            setDuration(duration);
            setType(type);
            setResolution(resolution);
            setCodec(codec);
            setAspectRatio(aspect_ratio);
            setAudioChannels(audio_channels);
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
            // Destructor logic if needed
            // Note: Media objects are not deleted here, as they are managed elsewhere
        }

}

#endif