/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the PersonMan class, which holds the person objects and provides methods to load and save the data from and to files.
Uses a hash map for fast access to person objects by id.
It also provides methods to add, remove and get person objects by id.
*/

#ifndef _PERSONMAN_H
#define _PERSONMAN_H

#include <iostream>
#include <QString>
#include <QVector>
#include <QHash>

#include "person.h"


class PersonMan {
    QString filename;
    QVector<std::shared_ptr<Person>> persons; // vector of persons
    QHash<unsigned long, std::shared_ptr<Person>> person_map; // hash map for fast access to persons by id
    unsigned long next_id;

    // this method is private, because the next id is updated internally
    void setNextId(unsigned long id) {
        next_id = id;
    }

    public:
        // constructor and destructor
        PersonMan(const QString& filename, bool load = true);
        ~PersonMan();

        // load and save functions
        int load();
        int save();

        // setter for the filename
        int setFilename(const QString& filename);

        // add and remove persons
        int addPerson(std::shared_ptr<Person> person);
        int removePersonId(unsigned long id);
        int removePerson(unsigned long index);

        // getters
        QVector<std::shared_ptr<Person>> getPersons() const;
        std::shared_ptr<Person> getPerson(unsigned long id) const;
        unsigned long getNextId() const;
        QString getFilename() const;

        // get a person by index in the vector
        std::shared_ptr<Person> operator[](unsigned long idx);

        // print the person objects in the vector
        friend std::ostream& operator<<(std::ostream& os, const PersonMan& pm);

};

#endif