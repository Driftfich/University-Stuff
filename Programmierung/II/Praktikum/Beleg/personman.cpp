#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>
#include <QString>
#include <QVector>
#include <iostream>
#include "personman.h"
#include "person.h"


PersonMan::PersonMan(const QString& filename) {
    setFilename(filename);
    setNextId(0);
    load(); // internally the load function sets the next id with the last id + 1
}

PersonMan::~PersonMan() {
    save();
}

int PersonMan::load() {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error opening file for reading: " << filename.toStdString() << std::endl;
        return -1;
    }
    unsigned long max_id = 0;
    while (!file.atEnd()) {
        std::shared_ptr<Person> person = Person::fromFile(file);
        if (person) {
            persons.push_back(person);
            if (person->getId() > max_id) {
                max_id = person->getId();
            }
        } else {
            std::cerr << "Error loading person from file" << std::endl;
            break;
        }
    }
    setNextId(max_id + 1);
    return 0;
}

int PersonMan::save() {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Error opening file for writing: " << filename.toStdString() << std::endl;
        return -1;
    }
    for (const auto& person : persons) {
        person->toFile(file);
    }
    return 0;
}

int PersonMan::setFilename(const QString& filename) {
    this->filename = filename;
    return 0;
}

int PersonMan::addPerson(std::shared_ptr<Person> person) {
    if (person) {
        persons.push_back(person);
        return 0;
    }
    return -1;
}

QVector<std::shared_ptr<Person>> PersonMan::getPersons() const {
    return persons;
}
unsigned long PersonMan::getNextId() const {
    return next_id;
}
QString PersonMan::getFilename() const {
    return filename;
}

int PersonMan::removePerson(unsigned long id) {
    for (auto p = persons.begin(); p != persons.end(); ++p) {
        if ((*p)->getId() == id) {
            persons.erase(p);
            return 0;
        }
    }
    return -1;
}

std::shared_ptr<Person> PersonMan::operator[](unsigned long idx) {
    if (idx < (unsigned long) persons.size()) {
        return persons[idx];
    }
    return nullptr;
}
std::ostream& operator<<(std::ostream& os, const PersonMan& pm) {
    os << "== PersonMan: " << pm.getFilename().toStdString() << " ==" << std::endl;
    for (const auto& person : pm.getPersons()) {
        os << *person << std::endl;
    }
    return os;
}

