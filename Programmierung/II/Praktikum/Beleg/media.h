#ifndef _MEDIA_H
#define _MEDIA_H

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "config.h"

class Media {
    unsigned long id;
    std::string title;
    std::chrono::year_month_day publication_date;
    std::vector<Person*> artist_ids; // array of artists/persons (person.h) ids => Information about saved in Person class
    std::string publisher;
    std::string description;
    std::string genre;
    std::vector<std::string> languages;
    int available_copies;

    public:
        // setter methods
        void setId(unsigned long id) {this->id = id;}
        int setTitle(const std::string& title);
        int setPublicationDate(const std::chrono::year_month_day publication_date);
        int setArtistIds(const std::vector<Person*>& artist_ids);
        int setPublisher(const std::string& publisher);
        int setDescription(const std::string& description);
        int setGenre(const std::string& genre);
        int setLanguages(const std::vector<std::string>& languages);
        int setAvailableCopies(int available_copies);

        // getter methods
        unsigned long getId() const {return this->id;}
        std::string getTitle() const {return this->title;}
        std::chrono::year_month_day getPublicationDate() const {return this->publication_date;}
        std::vector<Person*> getArtistIds() const {return this->artist_ids;}
        std::string getPublisher() const {return this->publisher;}
        std::string getDescription() const {return this->description;}
        std::string getGenre() const {return this->genre;}
        std::vector<std::string> getLanguages() const {return this->languages;}
        int getAvailableCopies() const {return this->available_copies;}
        
        // constructor using setters
        Media(unsigned long id, const std::string& title, const std::chrono::year_month_day publication_date,
              const std::vector<Person*>& artist_ids, const std::string& publisher, const std::string& description,
              const std::string& genre, const std::vector<std::string>& languages, int available_copies) {
            setId(id);
            setTitle(title);
            setPublicationDate(publication_date);
            setArtistIds(artist_ids);
            setPublisher(publisher);
            setDescription(description);
            setGenre(genre);
            setLanguages(languages);
            setAvailableCopies(available_copies);
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
            this->available_copies = other.available_copies;
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
                this->available_copies = other.available_copies;
            }
            return *this;
        }

        // destructor
        ~Media() {
            // Destructor logic if needed
        }
}

#endif