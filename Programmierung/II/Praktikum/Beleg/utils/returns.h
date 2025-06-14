#ifndef RETURNS_H
#define RETURNS_H
#include <QString>

struct Result {
    bool success;
    QString msg;
    int errorCode;

    static Result Success(const QString& msg = QString("Operation successful")) {
        return {true, msg, 0};
    }

    static Result Error(const QString& msg, int errorCode = -1) {
        return {false, msg, errorCode};
    }

    // when converting to bool, it returns true if success is true
    operator bool() const {
        return success;
    }
    // string conversion
    operator QString() const {
        return msg;
    }

    operator int() const {
        return errorCode;
    }
};

#endif // RETURNS_H