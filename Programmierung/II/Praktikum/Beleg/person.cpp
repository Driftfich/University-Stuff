#include <iostream>
#include <chrono>
#include <string>
// #include <math.h>
#include "person.h"

// Return values
// return -1 <=> error
// return 0 <=> fine

using namespace std;

int Person::setFname(const string& fname) {
    if (MAX_FNAME_LENGTH != -1 && MIN_FNAME_LENGTH != -1 && MAX_FNAME_LENGTH < MIN_FNAME_LENGTH) {
        cerr << "Error: MAX_FNAME_LENGTH has to be >= MIN_FNAME_LENGTH" << endl;
        return -1;
    }

    // Check that the fname exists and isnt to short
    if (MIN_FNAME_LENGTH != -1 && fname.length() < MIN_FNAME_LENGTH) {
        cerr << "Error: First name needs to be min " << MIN_FNAME_LENGTH << " characters long" << endl;
        return -1;
    }

    // Shorten the fname if it exceeds MAX_FNAME_LENGTH
    if (MAX_FNAME_LENGTH != -1 && MAX_FNAME_LENGTH < fname.length()) {
        cerr << "Warning: First name gets cut to " << MAX_FNAME_LENGTH << " characters from " << fname.length() << " characters" << endl;
        this->fname = fname.substr(0, min(fname.length(), MAX_FNAME_LENGTH));
    } else {
        this->fname = fname;
    }
    return 0;
}

int Person::setLname(const string& lname) {
    if (MAX_LNAME_LENGTH != -1 && MIN_LNAME_LENGTH != -1 && MAX_LNAME_LENGTH < MIN_LNAME_LENGTH) {
        cerr << "Error: MAX_LNAME_LENGTH has to be >= MIN_LNAME_LENGTH" << endl;
        return -1;
    }

    // Check that the lname exists and isnt to short
    if (MIN_LNAME_LENGTH != -1 && lname.length() < MIN_LNAME_LENGTH) {
        cerr << "Error: Last name needs to be min " << MIN_LNAME_LENGTH << " characters long" << endl;
        return -1;
    }

    // Shorten the lname if it exceeds MAX_LNAME_LENGTH
    if (MAX_LNAME_LENGTH != -1 && MAX_LNAME_LENGTH < lname.length()) {
        cerr << "Warning: Last name gets cut to " << MAX_LNAME_LENGTH << " characters from " << lname.length() << " characters" << endl;
        this->lname = lname.substr(0, min(lname.length(), MAX_LNAME_LENGTH));
    } else {
        this->lname = lname;
    }
    return 0;
}

int Person::setEname(const string& ename) {
    // Shorten the ename if it exceeds MAX_ENAME_LENGTH
    if (MAX_ENAME_LENGTH != -1 && MAX_ENAME_LENGTH < ename.length()) {
        cerr << "Warning: E(xtension)name gets cut to " << MAX_ENAME_LENGTH << " characters from " << ename.length() << " characters" << endl;
        this->ename = ename.substr(0, min(ename.length(), MAX_ENAME_LENGTH));
    } else {
        this->ename = ename;
    }
    return 0;
}

int Person::setBirthday(const std::chrono::year_month_day birthday) {
    if (MIN_AGE != -1 && MAX_AGE != -1 && MIN_AGE > MAX_AGE) {
        cerr << "Error: MIN_AGE > MAX_AGE" << endl;
        return -1;
    }

    std::chrono::year_month_day today = std::chrono::year_month_day(std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now()));
    std::chrono::years age = today.year() - birthday.year();

    // check that birthday is not in the future
    if (birthday > today) {
        cerr << "Error: Birthday is in the future" << endl;
        return -1;
    }

    // check that today - birthday <= MAX_AGE
    if (MAX_AGE != -1 && MAX_AGE >= 0 && age > MAX_AGE) {
        cerr << "Error: Person is older than " << MAX_AGE << " years" << endl;
        return -1;
    }
    // check that today - birthday >= MIN_AGE
    if (MIN_AGE != -1 && age < MIN_AGE) {
        cerr << "Error: Person is younger than " << MIN_AGE << " years" << endl;
        return -1;
    }

    this->birthday = birthday;
    return 0;
}

int Person::setGender(const Gender gender) {
    this->gender = gender;
    return 0;
}

int Person::setNote(const std::string& note) {
    // Check that the note is not too long
    if (note.length() > MAX_NOTE_LENGTH && MAX_NOTE_LENGTH != -1) {
        cerr << "Error: Note is too long" << endl;
        return -1;
    }
    this->note = note;
    return 0;
}

int Person::setLocation(const std::string& location) {
    // Check that the location is not too long
    if (location.length() > MAX_LOCATION_LENGTH && MAX_LOCATION_LENGTH != -1) {
        cerr << "Error: Location is too long" << endl;
        return -1;
    }
    this->location = location;
    return 0;
}

int Person::setEmail(const std::string& email) {
    // Check that the email is not too long
    if (email.length() > MAX_EMAIL_LENGTH && MAX_EMAIL_LENGTH != -1) {
        cerr << "Error: Email is too long" << endl;
        return -1;
    }
    this->email = email;
    return 0;
}