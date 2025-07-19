#ifndef _PERSON_H_
#define _PERSON_H_

#include "date.h"

class Person {
    char name [30+1];
    char surname [30+1];
    date birthday;

    public:
        Person(const char* name, const char* surname, int d, int m, int y);
        Person(const char* name, const char* surname);
        ~Person() {};
        
        void setBirthday(int d, int m, int y);
        void setName(const char* name);
        void setSurname(const char* surname);
        
        const date getBirthday() const {return birthday;};
        const char* getName() const {return name;};
        const char* getSurname() const {return surname;};
        
        void show(ostream& os);
};

#endif