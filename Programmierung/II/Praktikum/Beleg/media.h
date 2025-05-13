#ifndef _MEDIA_H
#define _MEDIA_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <map>
#include "json.hpp" // for json work with files
#include "config.h"

class Media {
    unsigned long id;
    std::string title;
    std::chrono::year_month_day publication_date;
    std::vector<int> artist_ids; // array persons ids => Information about saved in Person class
    std::vector<Person*> artists; // array of deserialized artists/persons (person.h) objects
    std::string publisher;
    std::string description;
    std::string genre;
    std::vector<std::string> languages;
    unsigned int available_copies;
    // metadata map with string to T map
    std::map<std::string, std::variant<int, float, std::string>> metadata;

    public:
        // setter methods
        void setId(unsigned long id) {this->id = id;}
        int setTitle(const std::string& title);
        int setPublicationDate(const std::chrono::year_month_day publication_date);
        int setArtistIds(const std::vector<int>& artist_ids);
        int setPublisher(const std::string& publisher);
        int setDescription(const std::string& description);
        int setGenre(const std::string& genre);
        int setLanguages(const std::vector<std::string>& languages);
        void setAvailableCopies(unsigned int available_copies) {this->available_copies = available_copies;}
        int setMetadata(const std::map<std::string, std::variant<int, float, std::string>>& metadata);

        // getter methods
        unsigned long getId() const {return this->id;}
        std::string getTitle() const {return this->title;}
        std::chrono::year_month_day getPublicationDate() const {return this->publication_date;}
        std::vector<int> getArtistIds() const {return this->artist_ids;}
        std::string getPublisher() const {return this->publisher;}
        std::string getDescription() const {return this->description;}
        std::string getGenre() const {return this->genre;}
        std::vector<std::string> getLanguages() const {return this->languages;}
        unsigned int getAvailableCopies() const {return this->available_copies;}
        std::map<std::string, std::variant<int, float, std::string>> getMetadata() const {return this->metadata;}
        
        // constructor using setters
        Media(unsigned long id, const std::string& title, const std::chrono::year_month_day publication_date,
              const std::vector<int>& artist_ids, const std::string& publisher, const std::string& description,
              const std::string& genre, const std::vector<std::string>& languages, unsigned int available_copies,
              const std::map<std::string, std::variant<int, float, std::string>>& metadata) {
            setId(id);
            setTitle(title);
            setPublicationDate(publication_date);
            setArtistIds(artist_ids);
            setPublisher(publisher);
            setDescription(description);
            setGenre(genre);
            setLanguages(languages);
            setAvailableCopies(available_copies);
            setMetadata(metadata);
            this->artists = std::vector<Person*>();
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
            this->available_copies = other.available_copies;
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
                this->available_copies = other.available_copies;
                this->metadata = other.metadata;
            }
            return *this;
        }

        // destructor
        virtual ~Media() {
            // Destructor logic if needed
        }

        // method to retrieve the subclass type
        virtual std::string getSubclassType() const = "Media"; // used to identify if it is a text, audio, video, etc.
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