#ifndef _TEXT_H
#define _TEXT_H

#include "media.h"

class Text : public Media {
    int pages;
    QString isbn;  // International Standard Book Number
    QString bindingType; // e.g. paperback, hardcover
    QString textFormat;  // e.g. Book, Magazine, Newspaper

    public:
        void setPages(int pages) {this->pages = pages;}
        int setIsbn(const QString& isbn);
        int setBindingType(const QString& bindingType);
        int setTextFormat(const QString& textFormat);

        int getPages() const {return this->pages;}
        QString getIsbn() const {return this->isbn;}
        QString getBindingType() const {return this->bindingType;}
        QString getTextFormat() const {return this->textFormat;}

        // constructor using setters
        Text(unsigned long id, const QString& title, const std::chrono::year_month_day publication_date,
             const QVector<int>& artist_ids, const QString& publisher, const QString& description,
             const QString& genre, const QVector<QString>& languages, unsigned int available_copies,
             int pages, const QString& isbn, const QString& bindingType, const QString& textFormat) :
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