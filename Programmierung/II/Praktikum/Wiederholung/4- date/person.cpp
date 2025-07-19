#include <iostream>
#include <iomanip>
#include <cstring>
#include "person.h"
#include "date.h"

using namespace std;

Person::Person(const char* name, const char* surname, int d, int m, int y) : birthday(d, m, y) {
    setName(name);
    setSurname(surname);
}

Person::Person(const char* name, const char* surname) : birthday() {
    strcpy(this->name, name);
    strcpy(this->surname, surname);
}

void Person::setBirthday(int d, int m, int y) {
    date d1 = date(d, m, y);
    this->birthday = d1;
}

void Person::setName(const char* name) {
    strcpy(this->name, name);
}

void Person::setSurname(const char* surname) {
    strcpy(this->surname, surname);
}

void Person::show(ostream& os) {
    cout << name << " " << surname << endl << "Birthday: ";
    birthday.show(os);
}

