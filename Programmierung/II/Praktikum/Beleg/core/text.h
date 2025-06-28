/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the Text class, which holds information and logic related to text media.
*/

#ifndef _TEXT_H
#define _TEXT_H

#include <QString>

#include "media.h"
#include "returns.h"

class Text : public Media {
    int pages;
    QString isbn;  // International Standard Book Number
    QString bindingType; // e.g. paperback, hardcover
    QString textFormat;  // e.g. Book, Magazine, Newspaper

    public:
        int setPages(int pages) {this->pages = pages; return 0;}
        int setIsbn(const QString& isbn);
        int setBindingType(const QString& bindingType);
        int setTextFormat(const QString& textFormat);

        int getPages() const {return this->pages;}
        QString getIsbn() const {return this->isbn;}
        QString getBindingType() const {return this->bindingType;}
        QString getTextFormat() const {return this->textFormat;}

        // methods for loading & saving text data
        QString getSubclassType() const override {return "Text";} // used to identify it is a text

        QJsonObject getSubclassParams() const override;

        static QJsonObject getSubclassSchema();
        static QJsonObject getSchema();

        Result loadSubclassParams(const QJsonObject& json) override;


        // constructor using setters
        Text(unsigned long id, const QString& title, const QDate publication_date,
             const QVector<unsigned long>& artist_ids, const QString& publisher, const QString& description,
             const QString& genre, const QVector<QString>& languages, QMap<QString, QVariant> metadata,
             int pages, const QString& isbn, const QString& bindingType, const QString& textFormat)
            : Media(id, title, publication_date, artist_ids, publisher, description, genre, languages, metadata) {
            setPages(pages);
            setIsbn(isbn);
            setBindingType(bindingType);
            setTextFormat(textFormat);
        }

        // constructor using json object
        Text(const QJsonObject& json) : Media(json) {
            if (loadSubclassParams(json["subclass_params"].toObject()) != 0) {
                throw std::runtime_error("Issue loading subclass parameters for Text");
            }
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
        }
};

#endif