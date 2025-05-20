#include <QDate>
#include <QVector>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QApplication>
#include <QSortFilterProxyModel>
#include <QSizePolicy>
#include <QLineEdit>
#include <iostream>
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
    Person p1 = Person((unsigned long) 1, QString("John"), QString("Doe"), QString(""), QDate::currentDate(), Gender::male, QString("Test Note"), QString("Test Location"), QString("john.doe@example.com"), QString("+123456789"));
    Borrower b1 = Borrower((unsigned long) 1, QString("John"), QString("Doe"), QString(""), QDate::currentDate(), Gender::male, QString("Test Note"), QString("Test Location"), QString("john.doe@example.com"), QString("+123456789"), (unsigned int) 5, (unsigned long) 1);
    Artist a1 = Artist((unsigned long) 1, QString("John"), QString("Doe"), QString(""), QDate::currentDate(), Gender::male, QString("Test Note"), QString("Test Location"), QString("john.doe@example.com"), QString("+123456789"), QString("Test Art Style"), QVector<unsigned long>{1, 2});

    // create a magic pointer array on the p1 and b1 objects
    std::shared_ptr<Person> personPtr1 = std::make_shared<Person>(p1);
    std::shared_ptr<Person> borrowerPtr1 = std::make_shared<Borrower>(b1);
    std::shared_ptr<Person> artistPtr1 = std::make_shared<Artist>(a1);

    // create a magic pointer array on the p1 and b1 objects
    std::vector<std::shared_ptr<Person>> personPtrs = {personPtr1, borrowerPtr1, artistPtr1};

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
    Libitem l1 = Libitem((unsigned long) 1, (unsigned long) 1, (unsigned long) 5, (unsigned long) 0, QString("Shelf A"), QString("New"));
    Libitem l2 = Libitem((unsigned long) 2, (unsigned long) 2, (unsigned long) 3, (unsigned long) 1, QString("Shelf B"), QString("Used"));
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
        std::cerr << "Fehler beim Öffnen zum Lesen\n";
        return -1;
    }


    // // reset file cursor to the beginning
    // std::cout << "Loading object from file" << std::endl;
    while (!file.atEnd()) {
        std::cout << "Loading object from file" << std::endl;
        std::shared_ptr<Libitem> libitemPtr = Libitem::fromFile(file);
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
        std::cerr << "Fehler beim Öffnen zum Lesen\n";
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
    std::shared_ptr<Person> p1 = std::make_shared<Person>(Person(pm.getNextId(), QString("John"), QString("Doe"), QString(""), QDate::currentDate(), Gender::male, QString("Test Note"), QString("Test Location"), QString("john.doe@example.com"), QString("+123456789")));
    pm.addPerson(p1);

    return 0;
}

int test_libitemman() {
    // Test the LibitemMan class
    LibitemMan lm = LibitemMan(QString("libitem.ndjson"));
    std::cout << lm << std::endl;

    // add a new libitem
    std::shared_ptr<Libitem> l1 = std::make_shared<Libitem>(Libitem(lm.getNextId(), (unsigned long) 1, (unsigned long) 5, (unsigned long) 0, QString("Shelf A"), QString("New")));
    lm.addLibitem(l1);

    return 0;
}

int test_ui(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 1) Core-Daten laden
    Library lib(QCoreApplication::applicationDirPath());
    lib.load();

    // 2) UI aufbauen
    MainWindow w = MainWindow();
    w.setWindowTitle("Library Management System");
    w.setMinimumSize(800, 600);

    // 3) TableModels erstellen
    auto *personModel = new PersonTableModel(lib.getPersonManager(), &w);
    auto *libitemModel = new LibItemTableModel(lib.getLibitemManager(), lib.getMediaManager(), &w);
    auto *transactionModel = new TransactionTableModel(lib.getTransactionManager(), lib.getPersonManager(), lib.getLibitemManager(), lib.getMediaManager(), &w);

    // 4) Proxy-Models als Filter/Sortierer
    auto *personProxy = new QSortFilterProxyModel(&w);
    personProxy->setSourceModel(personModel);
    personProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    personProxy->setDynamicSortFilter(true);

    auto *libitemProxy = new QSortFilterProxyModel(&w);
    libitemProxy->setSourceModel(libitemModel);
    libitemProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    libitemProxy->setDynamicSortFilter(true);
    
    auto *transactionProxy = new QSortFilterProxyModel(&w);
    transactionProxy->setSourceModel(transactionModel);
    transactionProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    transactionProxy->setDynamicSortFilter(true);


    // 5) Proxies an die Views hängen
    w.getTableWidgetUi()->persontab->setModel(personProxy);
    w.getTableWidgetUi()->persontab->setSortingEnabled(true);
    w.getTableWidgetUi()->itemtab->setModel(libitemProxy);
    w.getTableWidgetUi()->itemtab->setSortingEnabled(true);
    w.getTableWidgetUi()->transtab->setModel(transactionProxy);
    w.getTableWidgetUi()->transtab->setSortingEnabled(true);

    auto tv = w.getTableWidgetUi()->persontab;
    tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tv->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // dasselbe für die anderen Tabs:
    auto iv = w.getTableWidgetUi()->itemtab;
    iv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto tv2 = w.getTableWidgetUi()->transtab;
    tv2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 6) Toolbar-Signals mit den Proxies verbinden
    // (angenommen die toolbar.h hat searchEdit, columns, sort)
    QObject::connect(w.getToolbarUi()->searchbar, &QLineEdit::textChanged,
                     personProxy, &QSortFilterProxyModel::setFilterFixedString);
    QObject::connect(w.getToolbarUi()->columns, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [&personProxy](int col){ personProxy->setFilterKeyColumn(col); });
    QObject::connect(w.getToolbarUi()->sort, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [&personProxy](int idx){
                         personProxy->sort(personProxy->sortColumn(),
                                           idx==0 ? Qt::AscendingOrder : Qt::DescendingOrder);
                     });
    
                     
    // … analog für libitemProxy …
    QObject::connect(w.getToolbarUi()->searchbar, &QLineEdit::textChanged,
                     libitemProxy, &QSortFilterProxyModel::setFilterFixedString);
    QObject::connect(w.getToolbarUi()->columns, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [&libitemProxy](int col){ libitemProxy->setFilterKeyColumn(col); });
    QObject::connect(w.getToolbarUi()->sort, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [&libitemProxy](int idx){
                         libitemProxy->sort(libitemProxy->sortColumn(),
                                            idx==0 ? Qt::AscendingOrder : Qt::DescendingOrder);
                     });
    // … analog für transactionProxy …
    QObject::connect(w.getToolbarUi()->searchbar, &QLineEdit::textChanged,
                     transactionProxy, &QSortFilterProxyModel::setFilterFixedString);
    QObject::connect(w.getToolbarUi()->columns, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [&transactionProxy](int col){ transactionProxy->setFilterKeyColumn(col); });
    QObject::connect(w.getToolbarUi()->sort, QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [&transactionProxy](int idx){
                         transactionProxy->sort(transactionProxy->sortColumn(),
                                                idx==0 ? Qt::AscendingOrder : Qt::DescendingOrder);
                     });

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