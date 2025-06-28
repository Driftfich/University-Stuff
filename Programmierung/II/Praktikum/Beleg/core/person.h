/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the Person class, which holds information and logic related to persons.

Note:
Originally, the Person class was designed to be a base class for Artist and Borrower. However the problem which originated from this approach was
that a Person could only have one role at the same time, as long as multiple inheritance is not used. To not overcomplicate the design via
multiple inheritance, the Person class was changed to hold Artist and Borrower as unique_ptr members.
This allows a Person to have both roles at the same time, or none, and makes it easier to manage the roles without the need for complex inheritance hierarchies.
*/

#ifndef _PERSON_H
#define _PERSON_H

#include <QDate>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include "config.h"
#include "artist.h"
#include "borrower.h"
#include "returns.h"

enum class Gender {
    female,
    male,
    diverse,
    unknown
};

class Person {
    unsigned long id;
    QString fname;
    QString lname;
    QString ename;
    QDate birthday;
    QString gender;
    QString note;
    QString location;
    QString email;
    QString tel;

    std::unique_ptr<Artist> artist;
    std::unique_ptr<Borrower> borrower;

    public:
        // setter methods
        void setId(unsigned long id) {this->id = id;}
        int setFname(const QString& fname);
        int setLname(const QString& lname);
        int setEname(const QString& ename);
        int setBirthday(const QDate birthday);
        int setGender(const QString& gender);
        int setNote(const QString& note);
        int setLocation(const QString& location);
        int setEmail(const QString& email);
        int setTel(const QString& tel);

        int setArtist(std::unique_ptr<Artist> artist) {
            // move the artist pointer to the Person object
            this->artist = std::move(artist);
            return 0;
        }

        int setBorrower(std::unique_ptr<Borrower> borrower) {
            this->borrower = std::move(borrower);
            return 0;
        }

        // getter methods
        unsigned long getId() const {return this->id;}
        QString getFname() const {return this->fname;}
        QString getLname() const {return this->lname;}
        QString getEname() const {return this->ename;}
        QDate getBirthday() const {return this->birthday;}
        QString getGender() const {return this->gender;}
        QString getGenderString() const;
        QString getNote() const {return this->note;}
        QString getLocation() const {return this->location;}
        QString getEmail() const {return this->email;}
        QString getTel() const {return this->tel;}

        Artist* getArtist() const { return artist.get(); }
        Borrower* getBorrower() const { return borrower.get(); }

        // Role management convenience methods
        bool isArtist() const { return artist != nullptr; }
        bool isBorrower() const { return borrower != nullptr; }
        
        // Create role with default parameters
        int createArtistRole(const QString& artist_type = ""); // , const QVector<unsigned long>& media_ids = {}
        int createBorrowerRole(unsigned int limit = MAX_ITEMS_PER_BORROWER_DEFAULT);
        
        // Remove roles
        int removeArtistRole() { artist.reset(); return 0; }
        int removeBorrowerRole() { borrower.reset(); return 0; }

        // constructor using setters
        Person(unsigned long id, const QString& fname, const QString& lname, const QString& ename,
                const QDate birthday, const QString& gender, const QString& note,
                const QString& location, const QString& email, const QString& tel) {
            setId(id);
            setFname(fname);
            setLname(lname);
            setEname(ename);
            setBirthday(birthday);
            setGender(gender);
            setNote(note);
            setLocation(location);
            setEmail(email);
            setTel(tel);

            this->artist = nullptr;
            this->borrower = nullptr;
        }

        // constructor for loading from JSON
        Person(const QJsonObject& json);

        // copy constructor
        Person(const Person& other);

        // copy assignment operator
        Person& operator=(const Person& other);

        // destructor
        virtual ~Person();

        // subclass type
        virtual QString getSubclassType() const { return "Person"; } // used to identify it is a person

        // collects all parameters into a QJsonObject
        QJsonObject getJson() const;
        // writes the JSON object to a file
        void toFile(QFile& file) const;
        // loads local parameters from a JSON object
        Result loadLocalParams(const QJsonObject& json);
        // loads a Person object from a file
        static std::shared_ptr<Person> fromFile(QFile& file);
        static std::shared_ptr<Person> PersonFactory(const QJsonObject& json);

        // schema methods. Default schema changes based on if the Artist or Borrower should be shown in the info panel
        static QJsonObject getLocalSchema(bool ArtistChecked = false, bool BorrowerChecked = false);
        static QJsonObject getSchema(bool ArtistChecked = false, bool BorrowerChecked = false);
        // non static method to get the schema for this class based on the current state
        QJsonObject getSchemaAuto();

        // print methods
        friend std::ostream& operator<<(std::ostream& os, const Person& person);
        void printbase(std::ostream& os) const;
        void print(std::ostream& os) const;
};



#endif