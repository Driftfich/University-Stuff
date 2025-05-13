#ifndef _TEXT_H
#define _TEXT_H

#include "media.h"

class Text : public Media {
    int pages;
    std::string isbn;  // International Standard Book Number
    std::string bindingType; // e.g. paperback, hardcover
    std::string textFormat;  // e.g. Book, Magazine, Newspaper

    public:
        void setPages(int pages) {this->pages = pages;}
        int setIsbn(const std::string& isbn);
        int setBindingType(const std::string& bindingType);
        int setTextFormat(const std::string& textFormat);

        int getPages() const {return this->pages;}
        std::string getIsbn() const {return this->isbn;}
        std::string getBindingType() const {return this->bindingType;}
        std::string getTextFormat() const {return this->textFormat;}

        // constructor using setters
        Text(unsigned long id, const std::string& title, const std::chrono::year_month_day publication_date,
             const std::vector<int>& artist_ids, const std::string& publisher, const std::string& description,
             const std::string& genre, const std::vector<std::string>& languages, unsigned int available_copies,
             int pages, const std::string& isbn, const std::string& bindingType, const std::string& textFormat) :
             Media(id, title, publication_date, artist_ids, publisher, description, genre, languages, available_copies) {
            setPages(pages);
            setIsbn(isbn);
            setBindingType(bindingType);
            setTextFormat(textFormat);
        }

        // copy constructor
        Text(const Text& other) : Media(other) {
            this->pages = other.pages;
            this->isbn = other.isbn;
            this->bindingType = other.bindingType;
            this->textFormat = other.textFormat;
        }

        // copy assignment operator
        Text& operator=(const Text& other) {
            if (this != &other) {
                Media::operator=(other); // Call base class assignment operator
                this->pages = other.pages;
                this->isbn = other.isbn;
                this->bindingType = other.bindingType;
                this->textFormat = other.textFormat;
            }
            return *this;
        }

        // destructor
        ~Text() {
            // Destructor logic if needed
            // Note: Media objects are not deleted here, as they are managed elsewhere
        }
}

#endif