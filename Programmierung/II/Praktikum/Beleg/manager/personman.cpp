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
        if (person->getId() >= next_id) {
            setNextId(person->getId() + 1);
        }
        person_map.insert(person->getId(), person);
        return 0;
    }
    return -1;
}

QVector<std::shared_ptr<Person>> PersonMan::getPersons() const {
    return persons;
}
std::shared_ptr<Person> PersonMan::getPerson(unsigned long id) const {
    auto it = person_map.find(id);
    if (it != person_map.end()) {
        return it.value();
    }
    return nullptr;
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
            person_map.remove(id);
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

