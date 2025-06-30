#include <QDate>
#include <QVector>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QApplication>
#include <QSortFilterProxyModel>
#include <QSizePolicy>
#include <QLineEdit>
#include <QMouseEvent>
#include <QMessageBox>
#include <iostream>
#include <QStyleFactory>
#include "media.h"
#include "text.h"
#include "video.h"
#include "audio.h"
#include "borrower.h"
#include "person.h"
#include "artist.h"
#include "libitem.h"
#include "transaction.h"
#include "transactionman.h"
#include "mediaman.h"
#include "personman.h"
#include "libitemman.h"
#include "tablewidget.h"
#include "mainw.h"
#include "library.h"
#include "persontablemodel.h"
#include "libitemtablemodel.h"
#include "transactiontablemodel.h"
#include "custfiltproxmodel.h"
// include the variant type
#include <variant>

int test_media_file_management() {
    // Test the Media class
    Media m1 = Media((unsigned long) 1, QString("Test Title"), QDate::currentDate(), QVector<unsigned long>{1, 2}, QString("Test Publisher"), QString("Test Description"), QString("Test Genre"), QVector<QString>{"English"}, QMap<QString, QVariant>{});
    Text t1 = Text((unsigned long) 1, QString("Test Title"), QDate::currentDate(), QVector<unsigned long>{1, 2}, QString("Test Publisher"), QString("Test Description"), QString("Test Genre"), QVector<QString>{"English"}, QMap<QString, QVariant>{}, 5, QString("1234567890123"), QString("Paperback"), QString("Book"));
    Video v1 = Video((unsigned long) 1, QString("Test Title"), QDate::currentDate(), QVector<unsigned long>{1, 2}, QString("Test Publisher"), QString("Test Description"), QString("Test Genre"), QVector<QString>{"English"}, QMap<QString, QVariant>{}, 120, QString("Movie"), 1080, QString("H.264"), QString("16:9"), QString("Stereo"));
    Audio a1 = Audio((unsigned long) 1, QString("Test Title"), QDate::currentDate(), QVector<unsigned long>{1, 2}, QString("Test Publisher"), QString("Test Description"), QString("Test Genre"), QVector<QString>{"English"}, QMap<QString, QVariant>{}, 300, QString("Song"), 320, 44100, QString("Stereo"), QString("MP3"), QString("Test Album"), QVector<QString>{"Track 1", "Track 2"});

    // create a magic pointer array on the m1 and t1 objects
    std::shared_ptr<Media> mediaPtr1 = std::make_shared<Media>(m1);
    std::shared_ptr<Text> textPtr1 = std::make_shared<Text>(t1);
    std::shared_ptr<Video> videoPtr1 = std::make_shared<Video>(v1);
    std::shared_ptr<Audio> audioPtr1 = std::make_shared<Audio>(a1);
    // create a magic pointer array on the m1 and t1 objects
    std::vector<std::shared_ptr<Media>> mediaPtrs = {mediaPtr1, textPtr1, videoPtr1, audioPtr1};

    // // save all objects to a file
    QString filename = QString("media.ndjson");
    QFile file(filename);
    // clear the file beforehand
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Error: Could not open file for writing" << std::endl;
        return -1;
    }

    if (file.exists()) {
        file.remove();
    }

    for (int i = 0; i < (int) mediaPtrs.size(); ++i) {
        std::cout << "Saving object " << i << " to file: " << *mediaPtrs[i] << std::endl;
        mediaPtrs[i]->toFile(file);
    }
    
    // load all objects from a file by using the factory method from the media class
    // clear the mediaPtrs vector
    mediaPtrs.clear();

    // std::cout << "Loading objects from file" << std::endl;
    while (!file.atEnd()) {
        std::cout << "Loading object from file" << std::endl;
        std::shared_ptr<Media> mediaPtr = Media::fromFile(file);
        if (mediaPtr) {
            mediaPtrs.push_back(mediaPtr);
        }
        if (mediaPtr) {
            std::cout << "Loaded object from file: " << *mediaPtr << std::endl;
        } else {
            std::cerr << "Failed to load object from file" << std::endl;
            break;
        }
    }
    return 0;
}

int test_person_file_management() {
    // Test the Person class
    // Create a Person object
    Person p1 = Person((unsigned long) 1, QString("John"), QString("Doe"), QString(""), QDate::currentDate(), QString("Male"), QString("Test Note"), QString("Test Location"), QString("john.doe@example.com"), QString("+123456789"));
    Person p2 = Person((unsigned long) 2, QString("Jane"), QString("Smith"), QString("Ms."), QDate::currentDate(), QString("Female"), QString("Test Note"), QString("Test Location"), QString("jane.smith@example.com"), QString("+987654321"));
    Person p3 = Person((unsigned long) 3, QString("Alice"), QString("Johnson"), QString("Dr."), QDate::currentDate(), QString("Non-binary"), QString("Test Note"), QString("Test Location"), QString("alice.johnson@example.com"), QString("+123456789"));

    // make the p2 a borrower
    p2.setBorrower(std::make_unique<Borrower>(Borrower((unsigned long) 2)));
    // make the p3 an artist
    p3.setArtist(std::make_unique<Artist>(Artist(QString("Painter")))); // , QVector<unsigned long>{1, 2}

    // create a magic pointer array on the p1 and b1 objects
    std::vector<std::shared_ptr<Person>> personPtrs = {std::make_shared<Person>(p1), std::make_shared<Person>(p2), std::make_shared<Person>(p3)};

    // save all objects to a file
    QString filename = QString("person.ndjson");
    QFile file(filename);

    // clear the file beforehand
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Error: Could not open file for writing" << std::endl;
        return -1;
    }

    if (file.exists()) {
        file.remove();
    }

    for (int i = 0; i < (int) personPtrs.size(); ++i) {
        std::cout << "Saving object " << i << " to file: " << *personPtrs[i] << std::endl;
        personPtrs[i]->toFile(file);
    }

    // load all objects from a file by using the factory method from the person class
    // clear the personPtrs vector
    personPtrs.clear();
    // std::cout << "Loading objects from file" << std::endl;
    while (!file.atEnd()) {
        std::cout << "Loading object from file" << std::endl;
        std::shared_ptr<Person> personPtr = Person::fromFile(file);
        if (personPtr) {
            personPtrs.push_back(personPtr);
        }
        if (personPtr) {
            std::cout << "Loaded object from file: " << *personPtr << std::endl;
        } else {
            std::cerr << "Failed to load object from file" << std::endl;
            break;
        }
    }

    return 0;

}

int test_libitem_file_management() {
    // Test the Libitem class
    // Create a Libitem object
    Libitem l1 = Libitem((unsigned long) 1, (unsigned long) 1, (unsigned long) 5, QString("Shelf A"), QString("New"), [](unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId) {
        Q_UNUSED(libitemId);
        std::cout << "Libitem with ID " << libitemId << " changed media from " << oldMediaId << " to " << newMediaId << std::endl;
    });
    Libitem l2 = Libitem((unsigned long) 2, (unsigned long) 2, (unsigned long) 3, QString("Shelf B"), QString("Used"), [](unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId) {
        Q_UNUSED(libitemId);
        std::cout << "Libitem with ID " << libitemId << " changed media from " << oldMediaId << " to " << newMediaId << std::endl;
    });
    // create a magic pointer array on the l1 object
    std::shared_ptr<Libitem> libitemPtr1 = std::make_shared<Libitem>(l1);
    std::shared_ptr<Libitem> libitemPtr2 = std::make_shared<Libitem>(l2);

    // create a magic pointer array on the l1 object
    std::vector<std::shared_ptr<Libitem>> libitemPtrs = {libitemPtr1, libitemPtr2};

    // save all objects to a file
    QString filename = QString("libitem.ndjson");
    QFile file(filename);
    // clear the file beforehand
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Error: Could not open file for writing" << std::endl;
        return -1;
    }

    if (file.exists()) {
        file.remove();
    }

    // save all objects to a file
    for (int i = 0; i < (int) libitemPtrs.size(); ++i) {
        std::cout << "Saving object " << i << " to file: " << *libitemPtrs[i] << std::endl;
        libitemPtrs[i]->toFile(file);
    }

    // file.close();

    // load all objects from a file by using the factory method from the libitem class
    // clear the libitemPtrs vector
    libitemPtrs.clear();

    file.close();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: Could not open file for reading" << std::endl;
        return -1;
    }


    // // reset file cursor to the beginning
    // std::cout << "Loading object from file" << std::endl;
    while (!file.atEnd()) {
        std::cout << "Loading object from file" << std::endl;
        std::shared_ptr<Libitem> libitemPtr = Libitem::fromFile(file, [](unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId) {
            Q_UNUSED(libitemId);
            Q_UNUSED(oldMediaId);
            Q_UNUSED(newMediaId);
        });
        if (libitemPtr) {
            libitemPtrs.push_back(libitemPtr);
        }
        if (libitemPtr) {
            std::cout << "Loaded object from file: " << *libitemPtr << std::endl;
        } else {
            std::cerr << "Failed to load object from file" << std::endl;
            break;
        }
    }

    file.close();

    return 0;
}

int test_transaction_file_management() {
    // Test the Transaction class
    // Create a Transaction object
    Transaction t1 = Transaction((unsigned long) 1, (unsigned long) 1, (unsigned long) 1, QDateTime::currentDateTime());
    Transaction t2 = Transaction((unsigned long) 2, (unsigned long) 4, (unsigned long) 3, QDateTime::currentDateTime());
    // create a magic pointer array on the t1 object
    std::shared_ptr<Transaction> transactionPtr1 = std::make_shared<Transaction>(t1);
    std::shared_ptr<Transaction> transactionPtr2 = std::make_shared<Transaction>(t2);

    // save all objects to a file
    QString filename = QString("transaction.ndjson");
    QFile file(filename);
    // clear the file beforehand
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Error: Could not open file for writing" << std::endl;
        return -1;
    }

    if (file.exists()) {
        file.remove();
    }

    std::cout << "Saving object to file: " << *transactionPtr1 << std::endl;
    transactionPtr1->toFile(file);
    std::cout << "Saving object to file: " << *transactionPtr1 << std::endl;
    transactionPtr2->toFile(file);

    // load all objects from a file by using the factory method from the transaction class
    // clear the transactionPtrs vector
    // transactionPtrs.clear();

    file.close();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: Could not open file for reading" << std::endl;
        return -1;
    }

    // reset file cursor to the beginning
    // std::cout << "Loading object from file" << std::endl;
    while (!file.atEnd()) {
        std::cout << "Loading object from file" << std::endl;
        std::shared_ptr<Transaction> transactionPtr = Transaction::fromFile(file);
        if (transactionPtr) {
            // transactionPtrs.push_back(transactionPtr);
            std::cout << "Loaded object from file: " << *transactionPtr << std::endl;
        } else {
            std::cerr << "Failed to load object from file" << std::endl;
            break;
        }
    }

    return 0;
}

int test_transactionman() {
    // Test the TransactionMan class
    TransactionMan tm = TransactionMan(QString("transaction.ndjson"));
    std::cout << tm << std::endl;

    // add a new transaction
    std::shared_ptr<Transaction> t1 = std::make_shared<Transaction>(Transaction(tm.getNextId(), (unsigned long) 1, (unsigned long) 1, QDateTime::currentDateTime()));

    tm.addTransaction(t1);

    return 0;
}

int test_mediaman() {
    // Test the MediaMan class
    MediaMan mm = MediaMan(QString("media.ndjson"));
    std::cout << mm << std::endl;

    // add a new media
    std::shared_ptr<Media> m1 = std::make_shared<Media>(Media(mm.getNextId(), QString("Test Title"), QDate::currentDate(), QVector<unsigned long>{1, 2}, QString("Test Publisher"), QString("Test Description"), QString("Test Genre"), QVector<QString>{"English"}, QMap<QString, QVariant>{}));
    mm.addMedia(m1);

    return 0;
}

int test_personman() {
    // Test the PersonMan class
    PersonMan pm = PersonMan(QString("person.ndjson"));
    std::cout << pm << std::endl;

    // add a new person
    std::shared_ptr<Person> p1 = std::make_shared<Person>(Person(pm.getNextId(), QString("John"), QString("Doe"), QString(""), QDate::currentDate(), QString("Male"), QString("Test Note"), QString("Test Location"), QString("john.doe@example.com"), QString("+123456789")));
    pm.addPerson(p1);

    return 0;
}

int test_libitemman() {
    // Test the LibitemMan class
    LibitemMan lm = LibitemMan(QString("libitem.ndjson"));
    std::cout << lm << std::endl;

    // add a new libitem
    std::shared_ptr<Libitem> l1 = std::make_shared<Libitem>(Libitem(lm.getNextId(), (unsigned long) 1, (unsigned long) 5, QString("Shelf A"), QString("New"), [](unsigned long libitemId, unsigned long oldMediaId, unsigned long newMediaId) {
        Q_UNUSED(libitemId);
        std::cout << "Libitem with ID " << libitemId << " changed media from " << oldMediaId << " to " << newMediaId << std::endl;
    }));
    lm.addLibitem(l1);

    return 0;
}

static void loadQssTheme(QApplication &app, const QString &resourcePath) {
    QFile f(resourcePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not load theme:" << resourcePath;
        return;
    }
    app.setStyleSheet(QString::fromUtf8(f.readAll()));
}

/**
 * @brief Starts the MainWindow UI in modern Material look.
 * @param argc Number of command line arguments.
 * @param argv Command line arguments.
 * @return app.exec()
 */
int test_ui(int argc, char *argv[]) {
    Q_INIT_RESOURCE(ressources);
    QApplication app(argc, argv);

    // moderner Basis-Style
    app.setStyle(QStyleFactory::create("Fusion"));
    // eigenes Material-QSS drüberlegen
    loadQssTheme(app, ":/ui/material.qss");

    MainWindow w;
    w.show();
    return app.exec();
}

int main(int argc, char *argv[]) {
    // Test the Media class
    // test_media_file_management();
    // test_person_file_management();
    // test_libitem_file_management();
    // test_transaction_file_management();
    // test_transactionman();
    // test_mediaman();
    // test_personman();
    // test_libitemman();
    test_ui(argc, argv);
}