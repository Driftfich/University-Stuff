/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation of the PersonMan class, which holds the person objects and provides methods to load and save the data from and to files.
Uses a hash map for fast access to person objects by id.
It also provides methods to add, remove and get person objects by id.
*/

#include <iostream>
#include <QString>
#include <QVector>
#include <QHash>

#include "person.h"
#include "personman.h"

// constructor, loads the data from the file if load is true
PersonMan::PersonMan(const QString& filename, bool load) {
    setFilename(filename);
    setNextId(0);
    if (load) {
        this->load();
    }
}

// destructor, saves the data to the file
PersonMan::~PersonMan() {
    save();
}

// load the data from the file
int PersonMan::load() {
    QFile file(filename);
    // check if the file exists
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error opening file for reading: " << filename.toStdString() << std::endl;
        return -1;
    }
    // load each person from the file
    while (!file.atEnd()) {
        std::shared_ptr<Person> person = Person::fromFile(file);
        if (person) {
            addPerson(person);
        } else {
            std::cerr << "Error loading person from file" << std::endl;
            break;
        }
    }
    return 0;
}

// save the data to the file
int PersonMan::save() {
    QFile file(filename);
    // check if the file exists, if it is writeable and truncate it
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        std::cerr << "Error opening file for writing: " << filename.toStdString() << std::endl;
        return -1;
    }
    // save each person to the file
    for (const auto& person : persons) {
        person->toFile(file);
    }
    return 0;
}

// setter for the filename
int PersonMan::setFilename(const QString& filename) {
    this->filename = filename;
    return 0;
}

// add a person to the vector and the hash map
int PersonMan::addPerson(std::shared_ptr<Person> person) {
    if (person) {
        persons.push_back(person);
        // if the new person object has an id greater than the next id, update the next id
        if (person->getId() >= next_id) {
            setNextId(person->getId() + 1);
        }
        person_map.insert(person->getId(), person);
        return 0;
    }
    return -1;
}

// get all the persons
QVector<std::shared_ptr<Person>> PersonMan::getPersons() const {
    return persons;
}

// get a person by id
std::shared_ptr<Person> PersonMan::getPerson(unsigned long id) const {
    // find the person in the hash map
    QHash<unsigned long, std::shared_ptr<Person>>::const_iterator it = person_map.find(id);
    if (it != person_map.end()) {
        return it.value();
    }
    return nullptr;
}

// get the next id
unsigned long PersonMan::getNextId() const {
    return next_id;
}

// get the filename
QString PersonMan::getFilename() const {
    return filename;
}

// remove a person by id
int PersonMan::removePersonId(unsigned long id) {
    // iterate over the vector and remove the person if the id matches
    for (QVector<std::shared_ptr<Person>>::iterator p = persons.begin(); p != persons.end(); ++p) {
        if ((*p)->getId() == id) {
            persons.erase(p);
            person_map.remove(id);
            return 0;
        }
    }
    return -1;
}

// remove a person by index
int PersonMan::removePerson(unsigned long index) {
    if (index < (unsigned long) persons.size()) {
        unsigned long id = persons[index]->getId();
        persons.erase(persons.begin() + index);
        person_map.remove(id);
        return 0;
    }
    return -1;
}

// get a person by index in the vector
std::shared_ptr<Person> PersonMan::operator[](unsigned long idx) {
    if (idx < (unsigned long) persons.size()) {
        return persons[idx];
    }
    return nullptr;
}

// print the person objects in the vector
std::ostream& operator<<(std::ostream& os, const PersonMan& pm) {
    os << "== PersonMan: " << pm.getFilename().toStdString() << " ==" << std::endl;
    os << "Next ID: " << pm.getNextId() << std::endl;
    os << "Count: " << pm.getPersons().size() << std::endl;
    os << "-------------------------" << std::endl;
    os << "Persons:" << std::endl;
    for (const auto& person : pm.getPersons()) {
        os << *person << std::endl;
    }
    os << "======== End Personman ===========" << std::endl;
    return os;
}

