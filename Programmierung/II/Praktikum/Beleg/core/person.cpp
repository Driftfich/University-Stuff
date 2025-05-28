#include <iostream>
#include <QDate>
#include <QVector>
#include <QString>
#include <string>
// #include <math.h>
#include <regex>
#include "person.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include "artist.h"
#include "borrower.h"

// Return values
// return -1 <=> error
// return 0 <=> fine

using namespace std;

int Person::setFname(const QString& fname) {
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
        this->fname = fname.left(MAX_FNAME_LENGTH);
    } else {
        this->fname = fname;
    }
    return 0;
}

int Person::setLname(const QString& lname) {
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
        this->lname = lname.left(MAX_LNAME_LENGTH);
    } else {
        this->lname = lname;
    }
    return 0;
}

int Person::setEname(const QString& ename) {
    // Shorten the ename if it exceeds MAX_ENAME_LENGTH
    if (MAX_ENAME_LENGTH != -1 && MAX_ENAME_LENGTH < ename.length()) {
        cerr << "Warning: E(xtension)name gets cut to " << MAX_ENAME_LENGTH << " characters from " << ename.length() << " characters" << endl;
        this->ename = ename.left(MAX_ENAME_LENGTH);
    } else {
        this->ename = ename;
    }
    return 0;
}

int Person::setBirthday(const QDate birthday) {
    if (MIN_AGE != -1 && MAX_AGE != -1 && MIN_AGE > MAX_AGE) {
        cerr << "Error: MIN_AGE > MAX_AGE" << endl;
        return -1;
    }

    QDate today = QDate::currentDate();
    // calculate the age
    int age = today.year() - birthday.year();
    
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

QString Person::getGenderString() const  {
    switch (gender) {
        case Gender::female: return "Female";
        case Gender::male: return "Male";
        case Gender::diverse: return "Diverse";
        case Gender::unknown: return "Unknown";
    }
    return "Unknown";
}

int Person::setNote(const QString& note) {
    // Check that the note is not too long
    if (note.length() > MAX_NOTE_LENGTH && MAX_NOTE_LENGTH != -1) {
        cerr << "Error: Note is too long" << endl;
        return -1;
    }
    this->note = note;
    return 0;
}

int Person::setLocation(const QString& location) {
    // Check that the location is not too long
    if (location.length() > MAX_LOCATION_LENGTH && MAX_LOCATION_LENGTH != -1) {
        cerr << "Error: Location is too long" << endl;
        return -1;
    }
    this->location = location;
    return 0;
}

int Person::setEmail(const QString& email) {
    // Check that the email is not too long
    if (email.length() > MAX_EMAIL_LENGTH && MAX_EMAIL_LENGTH != -1) {
        cerr << "Error: Email is too long" << endl;
        return -1;
    }

    // Check the email with regex for a valid email address
    if (!email.isEmpty()) {
        std::regex email_regex(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
        if (!std::regex_match(email.toStdString(), email_regex)) {
            cerr << "Error: Email is not a valid email address" << endl;
            return -1;
        }
    }
    
    this->email = email;
    return 0;
}

int Person::setTel(const QString& tel) {
    // Check that the tel is not too long
    if (tel.length() > MAX_TEL_LENGTH && MAX_TEL_LENGTH != -1) {
        cerr << "Error: Tel is too long" << endl;
        return -1;
    }

    // Check the tel with regex for a valid tel number
    if (!tel.isEmpty()) {
        std::regex tel_regex("^\\+?[1-9]\\d{1,14}$");
        if (!std::regex_match(tel.toStdString(), tel_regex)) {
            cerr << "Error: Tel is not a valid number" << endl;
            return -1;
        }
    }
    this->tel = tel;
    return 0;
}

// constructor using setters
// Person::Person(unsigned long id, const QString& fname, const QString& lname, const QString& ename,
//                const QDate birthday, Gender gender, const QString& note,
//                const QString& location, const QString& email, const QString& tel) {
//     setId(id);
//     setFname(fname);
//     setLname(lname);
//     setEname(ename);
//     setBirthday(birthday);
//     setGender(gender);
//     setNote(note);
//     setLocation(location);
//     setEmail(email);
//     setTel(tel);
// }

// copy constructor
Person::Person(const Person& other) {
    // deep copy is handled by the copy constructor of the QString class
    // setter method overhead is not needed as already ensured in the other object
    this->id = other.id;
    this->fname = other.fname;
    this->lname = other.lname;
    this->ename = other.ename;
    this->birthday = other.birthday;
    this->gender = other.gender;
    this->note = other.note;
    this->location = other.location;
    this->email = other.email;
    this->tel = other.tel;
}

// constructor for loading from JSON
Person::Person(const QJsonObject& json) {
    if (loadLocalParams(json) != 0) {
        throw std::runtime_error("Issues loading person parameters");
    }
}

// copy assignment operator
Person& Person::operator=(const Person& other) {
    // deep copy is handled by the copy assignment operator of the QString class
    // setter method overhead is not needed as already ensured in the other object
    // self-assignment check
    if (this != &other) {
        this->id = other.id;
        this->fname = other.fname;
        this->lname = other.lname;
        this->ename = other.ename;
        this->birthday = other.birthday;
        this->gender = other.gender;
        this->note = other.note;
        this->location = other.location;
        this->email = other.email;
        this->tel = other.tel;
    }
    return *this;
}

Person::~Person() {
    // destructor
    // no need to delete anything as QString and QDate are handled by the C++ standard library
}

QJsonObject Person::getLocalParams() const {
    QJsonObject json;
    json["id"] = static_cast<qint64>(id);
    json["fname"] = fname;
    json["lname"] = lname;
    json["ename"] = ename;
    json["birthday"] = birthday.toString(Qt::ISODate);
    json["gender"] = static_cast<int>(gender);
    json["note"] = note;
    json["location"] = location;
    json["email"] = email;
    json["tel"] = tel;
    return json;
}

QJsonObject Person::getJson() const {
    QJsonObject obj;
    obj["person"] = getLocalParams();
    // qDebug() << "Person JSON:" << QJsonDocument(obj).toJson(QJsonDocument::Indented);
    obj["subclass_type"] = getSubclassParams().isEmpty() ? QString("Person") : getSubclassType();
    obj["subclass_params"] = getSubclassParams();
    return obj;
}

void Person::toFile(QFile& file) const {
    if (!file.isOpen()) file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QJsonDocument doc(getJson());
    file.write(doc.toJson(QJsonDocument::Compact) + "\n");
}

int Person::loadLocalParams(const QJsonObject& json) {
    if (json.contains("id")) id = static_cast<unsigned long>(json["id"].toVariant().toLongLong());
    if (json.contains("fname")) fname = json["fname"].toString();
    if (json.contains("lname")) lname = json["lname"].toString();
    if (json.contains("ename")) ename = json["ename"].toString();
    if (json.contains("birthday")) birthday = QDate::fromString(json["birthday"].toString(), Qt::ISODate);
    if (json.contains("gender")) gender = static_cast<Gender>(json["gender"].toInt());
    if (json.contains("note")) note = json["note"].toString();
    if (json.contains("location")) location = json["location"].toString();
    if (json.contains("email")) email = json["email"].toString();
    if (json.contains("tel")) tel = json["tel"].toString();
    return 0;
}

std::shared_ptr<Person> Person::fromFile(QFile& file) {
    if (!file.isOpen()) file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray line = file.readLine();
    QJsonObject obj = QJsonDocument::fromJson(line).object();
    return PersonFactory(obj);
}

std::shared_ptr<Person> Person::PersonFactory(const QJsonObject& json) {
    auto base = json["person"].toObject();
    auto type = json["subclass_type"].toString();
    if (type == "Artist") {
        return std::make_shared<Artist>(json);
    } else if (type == "Borrower") {
        return std::make_shared<Borrower>(json);
    }
    else if (type == "Person") {
        return std::make_shared<Person>(base);
    }
    else {
        throw std::runtime_error("Unknown subclass type: " + type.toStdString());
    }
}


void Person::printbase(std::ostream& os) const {
    os << "ID: " << id << "\n"
    << "First Name: " << fname.toStdString() << "\n"
    << "Last Name: " << lname.toStdString() << "\n"
    << "E(xtension)Name: " << ename.toStdString() << "\n"
    << "Birthday: " << birthday.toString(Qt::ISODate).toStdString() << "\n"
    << "Gender: " << (gender == Gender::male ? "Male" : gender == Gender::female ? "Female" : "Diverse") << "\n"
    << "Note: " << note.toStdString() << "\n"
    << "Location: " << location.toStdString() << "\n"
    << "Email: " << email.toStdString() << "\n"
    << "Telephone: " << tel.toStdString() << "\n";
}

void Person::printSubclass(std::ostream& os) const {
    // iterate over the map from the subclass parameters
    os << "Subclass type: " << getSubclassType().toStdString() << "\n";
    QJsonObject subclass_params = getSubclassParams();
    for (auto it = subclass_params.begin(); it != subclass_params.end(); ++it) {
        os << it.key().toStdString() << ": " << it.value().toVariant().toString().toStdString() << "\n";
    }
} 

void Person::print(std::ostream& os) const {
    printbase(os);
    printSubclass(os);
    os << "----------------------------------------" << "\n";
}

// print the person object
std::ostream& operator<<(std::ostream& os, const Person& person) {
    person.print(os);
    return os;
}