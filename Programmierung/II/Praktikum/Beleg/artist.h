#ifndef _ARTIST_H
#define _ARTIST_H

#include <iostream>
#include <string>
#include <vector>

#include "config.h"
#include "person.h"

class Artist : public Person {
    std::string artist_type; // e.g. actor, director, musician, etc.
    std::vector<int> media_ids; // array of media ids => works created by the artist
    std::vector<Media*> media; // array of deserialized media objects

    public:
        // setter methods
        int setArtistType(const std::string& artist_type);
        int setMediaIds(const std::vector<int>& media_ids);

        // getter methods
        std::string getArtistType() const {return this->artist_type;}
        std::vector<int> getMediaIds() const {return this->media_ids;}
        std::vector<Media*> getMedia() const {return this->media;}

        // constructor using setters
        Artist(unsigned long id, const std::string& fname, const std::string& lname, const std::string& ename,
               const std::chrono::year_month_day birthday,
               const std::string& artist_type, const std::vector<int>& media_ids,) :
               Person(id, fname, lname, ename, birthday) {
            setArtistType(artist_type);
            setMediaIds(media_ids);
            this->media = std::vector<Media*>();
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