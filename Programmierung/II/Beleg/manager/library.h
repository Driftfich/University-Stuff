/*
Author: Franz Rehschuh
Date: 2025-06-20

Description: Header file for the library class, which holds the manager classes for media, libitems, persons, and transactions
at one place. Used to load and save the data from and to files at once.
*/

#ifndef _LIBRARY_H
#define _LIBRARY_H

#include <iostream>
#include <QDir>

#include "mediaman.h"
#include "transactionman.h"
#include "personman.h"
#include "libitemman.h"

class Library : public QObject {
    Q_OBJECT

    QString dir; // directory for saving the files

    std::unique_ptr<MediaMan> mediaManager;
    std::unique_ptr<LibitemMan> libitemManager;
    std::unique_ptr<PersonMan> personManager;
    std::unique_ptr<TransactionMan> transactionManager;

    public:
        Library(const QString& dir, bool load);
        ~Library();

        // setters
        int setDir(const QString& dir);

        // getters
        QString getDir() const;
        MediaMan* getMediaManager() const;
        LibitemMan* getLibitemManager() const;
        PersonMan* getPersonManager() const;
        TransactionMan* getTransactionManager() const;

        // load and save functions
        int load();
        int save();

        // print functions
        friend std::ostream& operator<<(std::ostream& os, const Library& lib);
};

#endif