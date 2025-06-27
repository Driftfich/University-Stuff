/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Implementation file for the Person class, which holds information and logic related to persons.
*/

#include <iostream>
#include <QDate>
#include <QVector>
#include <QString>
// #include <regex>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QFile>

#include "person.h"
#include "artist.h"
#include "borrower.h"

using namespace std;

int Person::setFname(const QString& fname) {
    if (fname.isEmpty()) {
        cerr << "Error: First name is empty" << endl;
        return -1;
    }
    this->fname = fname;
    return 0;
}

int Person::setLname(const QString& lname) {
    if (lname.isEmpty()) {
        cerr << "Error: Last name is empty" << endl;
        return -1;
    }
    this->lname = lname;
    return 0;
}

int Person::setEname(const QString& ename) {
    this->ename = ename;
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

int Person::setGender(const QString& gender) {
    this->gender = gender;
    return 0;
}

QString Person::getGenderString() const  {
    return this->gender;
}

int Person::setNote(const QString& note) {
    this->note = note;
    return 0;
}

int Person::setLocation(const QString& location) {
    this->location = location;
    return 0;
}

int Person::setEmail(const QString& email) {
    // Check the email with regex for a valid email address
    if (!email.isEmpty()) {
        QRegularExpression email_regex(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
        if (!email_regex.match(email).hasMatch()) {
            cerr << "Error: Email is not a valid email address" << endl;
            return -1;
        }
    }
    
    this->email = email;
    return 0;
}

int Person::setTel(const QString& tel) {
    // Check the tel with regex for a valid tel number
    if (!tel.isEmpty()) {
        QRegularExpression tel_regex(R"(^\+?[1-9][\d\s\-\(\)\.]{1,14}$)");
        if (!tel_regex.match(tel).hasMatch()) {
            cerr << "Error: Tel is not a valid number" << endl;
            return -1;
        }
    }
    this->tel = tel;
    return 0;
}

// copy constructor
Person::Person(const Person& other) {
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
    // as artist and borrower are unique_ptrs, we need to recreate them
    this->artist = other.artist ? std::make_unique<Artist>(*other.artist) : nullptr;
    this->borrower = other.borrower ? std::make_unique<Borrower>(*other.borrower) : nullptr;
}

// constructor for loading from JSON
Person::Person(const QJsonObject& json) {
    if (loadLocalParams(json) != 0) {
        throw std::runtime_error("Issues loading person parameters");
    }
}

// copy assignment operator
Person& Person::operator=(const Person& other) {
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
        this->artist = other.artist ? std::make_unique<Artist>(*other.artist) : nullptr;
        this->borrower = other.borrower ? std::make_unique<Borrower>(*other.borrower) : nullptr;
    }
    return *this;
}

Person::~Person() {
}

// Collects all parameters into a QJsonObject
QJsonObject Person::getJson() const {
    QJsonObject json;
    json["id"] = static_cast<qint64>(id);
    json["fname"] = fname;
    json["lname"] = lname;
    json["ename"] = ename;
    json["birthday"] = birthday.toString(Qt::ISODate);
    json["gender"] = gender;
    json["note"] = note;
    json["location"] = location;
    json["email"] = email;
    json["tel"] = tel;
    if (artist) {
        json["artist"] = artist->getSubclassParams();
    }
    else json["artist"] = QJsonObject();
    if (borrower) {
        json["borrower"] = borrower->getSubclassParams();
    }
    else json["borrower"] = QJsonObject();
    return json;
}

// Writes the JSON object to a file
void Person::toFile(QFile& file) const {
    if (!file.isOpen()) file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QJsonDocument doc(getJson());
    file.write(doc.toJson(QJsonDocument::Compact) + "\n");
}

// Loads local parameters from a JSON object
int Person::loadLocalParams(const QJsonObject& json) {
    if (json.contains("id")) id = static_cast<unsigned long>(json["id"].toVariant().toLongLong());
    if (json.contains("fname")) fname = json["fname"].toString();
    if (json.contains("lname")) lname = json["lname"].toString();
    if (json.contains("ename")) ename = json["ename"].toString();
    if (json.contains("birthday")) birthday = QDate::fromString(json["birthday"].toString(), Qt::ISODate);
    if (json.contains("gender")) gender = json["gender"].toString();
    if (json.contains("note")) note = json["note"].toString();
    if (json.contains("location")) location = json["location"].toString();
    if (json.contains("email")) email = json["email"].toString();
    if (json.contains("tel")) tel = json["tel"].toString();
    if (json.contains("artist")) {
        QJsonObject artistJson = json["artist"].toObject();
        if (!artistJson.isEmpty()) artist = std::make_unique<Artist>(artistJson);
    } else {
        artist = nullptr;
    }
    if (json.contains("borrower")) {
        QJsonObject borrowerJson = json["borrower"].toObject();
        if (!borrowerJson.isEmpty()) borrower = std::make_unique<Borrower>(borrowerJson);
    } else {
        borrower = nullptr;
    }
    return 0;
}

// Loads a Person object from a file
std::shared_ptr<Person> Person::fromFile(QFile& file) {
    if (!file.isOpen()) file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray line = file.readLine();
    QJsonObject obj = QJsonDocument::fromJson(line).object();
    return PersonFactory(obj);
}

// Factory method to create a Person object from a QJsonObject
std::shared_ptr<Person> Person::PersonFactory(const QJsonObject& json) {
    return std::make_shared<Person>(json);
}

// schema methods
QJsonObject Person::getLocalSchema(bool ArtistChecked, bool BorrowerChecked) {
    QJsonObject schema;

    schema["id"] = QJsonObject{{"type", "integer"}, {"readonly", true}, {"rename", "ID"}, {"description", "Unique identifier for the person"}, {"required", true}};
    schema["fname"] = QJsonObject{{"type", "string"}, {"rename", "First Name"}, {"description", "First name of the person"}, {"required", true}};
    schema["lname"] = QJsonObject{{"type", "string"} , {"rename", "Last Name"}, {"description", "Last name of the person"}, {"required", true}};
    schema["ename"] = QJsonObject{{"type", "string"}, {"rename", "Extension Name"}, {"description", "Extension name of the person"}};
    schema["birthday"] = QJsonObject{{"type", "string"}, {"format", "date"}, {"rename", "Birthday"}, {"description", "Birthday of the person"}, {"required", true}};
    schema["gender"] = QJsonObject{
        {"type", "string"},
        {"enum", QJsonArray{"Female", "Male", "Diverse", "Unknown"}},
        {"rename", "Gender"},
        {"description", "Gender of the person"}};
    schema["note"] = QJsonObject{{"type", "string"}, {"rename", "Note"}, {"description", "Note about the person"}};
    schema["location"] = QJsonObject{{"type", "string"}, {"rename", "Location"}, {"description", "Location of the person"}};
    schema["email"] = QJsonObject{{"type", "string"}, {"format", "email"}, {"rename", "Email"}, {"description", "Email address of the person"}};
    schema["tel"] = QJsonObject{{"type", "string"}, {"format", "tel"}, {"rename", "Telephone"}, {"description", "Telephone number of the person"}};
    schema["artist"] = Artist::getSubclassSchema(ArtistChecked);
    schema["borrower"] = Borrower::getSubclassSchema(BorrowerChecked);

    return schema;
}

QJsonObject Person::getSchema(bool ArtistChecked, bool BorrowerChecked) {
    QJsonObject schema;
    QJsonObject properties;
    schema.insert("type", "object");
    schema.insert("properties", getLocalSchema(ArtistChecked, BorrowerChecked));
    schema.insert("rename", "Person");
    return schema;
}

QJsonObject Person::getSchemaAuto() {
    bool hasArtist=false, hasBorrower=false;
    if (isBorrower()) hasBorrower = true;
    if (isArtist()) hasArtist = true;

    return getSchema(hasArtist, hasBorrower);
}

void Person::printbase(std::ostream& os) const {
    os << "ID: " << id << "\n"
    << "First Name: " << fname.toStdString() << "\n"
    << "Last Name: " << lname.toStdString() << "\n"
    << "E(xtension)Name: " << ename.toStdString() << "\n"
    << "Birthday: " << birthday.toString(Qt::ISODate).toStdString() << "\n"
    << "Gender: " << gender.toStdString() << "\n"
    << "Note: " << note.toStdString() << "\n"
    << "Location: " << location.toStdString() << "\n"
    << "Email: " << email.toStdString() << "\n"
    << "Telephone: " << tel.toStdString() << "\n";
    if (artist) {
        os << "Artist Type: " << artist->getArtistType().toStdString() << "\n";
        // os << "Media IDs: ";
        // for (const auto& media_id : artist->getMediaIds()) {
        //     os << media_id << " ";
        // }
        // os << "\n";
    }
    if (borrower) {
        os << "Borrower Limit: " << borrower->getLimit() << "\n";
        os << "\n";
    }
}

void Person::print(std::ostream& os) const {
    printbase(os);
    // printSubclass(os);
    os << "----------------------------------------" << "\n";
}

// print the person object
std::ostream& operator<<(std::ostream& os, const Person& person) {
    person.print(os);
    return os;
}

// Role management method implementations
int Person::createArtistRole(const QString& artist_type) { // , const QVector<unsigned long>& media_ids
    try {
        artist = std::make_unique<Artist>(artist_type); // , media_ids
        return 0;
    } catch (...) {
        return -1;
    }
}

int Person::createBorrowerRole(unsigned int limit) {
    try {
        borrower = std::make_unique<Borrower>(limit);
        return 0;
    } catch (...) {
        return -1;
    }
}