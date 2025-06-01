// Franz Rehschuh (s88216)
// Class definition for the Person (abstract) class and child classes
// TODO: Implement class for Tel number and Location

// Return values
// return -1 <=> error
// return 0 <=> fine

#ifndef _PERSON_H
#define _PERSON_H

#include <iostream>
#include <string>
#include <QDate>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <QVariant>
#include "config.h"



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
    // Gender gender;
    QString gender;
    QString note;
    QString location;
    QString email;
    QString tel;

    public:
        // setter methods
        void setId(unsigned long id) {this->id = id;}
        int setFname(const QString& fname);
        int setLname(const QString& lname);
        int setEname(const QString& ename);
        int setBirthday(const QDate birthday);
        // int setGender(const Gender gender);
        int setGender(const QString& gender);
        int setNote(const QString& note);
        int setLocation(const QString& location);
        int setEmail(const QString& email);
        int setTel(const QString& tel);

        // getter methods
        unsigned long getId() const {return this->id;}
        QString getFname() const {return this->fname;}
        QString getLname() const {return this->lname;}
        QString getEname() const {return this->ename;}
        QDate getBirthday() const {return this->birthday;}
        // Gender getGender() const {return this->gender;}
        QString getGender() const {return this->gender;}
        QString getGenderString() const;
        QString getNote() const {return this->note;}
        QString getLocation() const {return this->location;}
        QString getEmail() const {return this->email;}
        QString getTel() const {return this->tel;}

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

        // serialization methods
        QJsonObject getLocalParams() const;
        QJsonObject getLocalDefaultParams() const;
        virtual QJsonObject getSubclassParams() const { return QJsonObject(); }
        virtual QJsonObject getSubclassDefaultParams() const { return QJsonObject(); }
        QJsonObject getJson() const;
        QJsonObject getDefaultJson() const;
        void toFile(QFile& file) const;
        int loadLocalParams(const QJsonObject& json);
        virtual int loadSubclassParams(const QJsonObject& json) { Q_UNUSED(json); return 0; }
        static std::shared_ptr<Person> fromFile(QFile& file);
        static std::shared_ptr<Person> PersonFactory(const QJsonObject& json);

        // schema methods
        QJsonObject getLocalSchema() const;
        virtual QJsonObject getSubclassSchema() const { return QJsonObject(); }
        QJsonObject getSchema() const;

        // print methods
        friend std::ostream& operator<<(std::ostream& os, const Person& person);
        void printbase(std::ostream& os) const;
        void print(std::ostream& os) const;
        void printSubclass(std::ostream& os) const;
};



#endif