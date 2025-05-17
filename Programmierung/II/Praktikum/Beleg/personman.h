#ifndef _PERSONMAN_H
#define _PERSONMAN_H

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>
#include <QString>
#include <QVector>
#include "person.h"


class PersonMan {
    QString filename;
    QVector<std::shared_ptr<Person>> persons;
    QHash<unsigned long, std::shared_ptr<Person>> person_map; // fast access to persons by id
    unsigned long next_id;

    void setNextId(unsigned long id) {
        next_id = id;
    }

    public:
        // constructor and destructor
        PersonMan(const QString& filename);
        ~PersonMan();

        // load and save functions
        int load();
        int save();

        // setters and getters
        int setFilename(const QString& filename);
        int addPerson(std::shared_ptr<Person> person);

        QVector<std::shared_ptr<Person>> getPersons() const;
        unsigned long getNextId() const;
        QString getFilename() const;

        // person management functions
        int removePerson(unsigned long id);

        std::shared_ptr<Person> operator[](unsigned long idx);

        // print functions
        friend std::ostream& operator<<(std::ostream& os, const PersonMan& pm);

};

#endif