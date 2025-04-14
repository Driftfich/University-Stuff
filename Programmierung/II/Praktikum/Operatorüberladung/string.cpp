// Franz Rehschuh

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "string.h"

using namespace std;

String::String() {
    this->pBuf = NULL;
    this->len = 0;
    this->refcount = new int(1);
    return;
}

String::String(const char* pStr) {
    // this->pBuf = (char *) pStr;
    this->pBuf = new char[strlen(pStr) + 1];
    strcpy(this->pBuf, pStr);
    this->len = strlen(pStr);
    this->refcount = new int(1);
    return;
}

String::String(char c, int n) {
    this->pBuf = new char[n+1];
    for (int i=0; i<n; i++) {
        pBuf[i] = c;
    }
    this->pBuf[n] = '\0';
    this->len = n;
    this->refcount = new int(1);
}

String::String(const String& other) {
    this->pBuf = other.pBuf;
    this->len = other.len;
    this->refcount = other.refcount;
    if (this->refcount) {
        // Increment the value pointed to by the shared reference counter
        (*this->refcount)++;
    }
    return;
}

String& String::operator=(const String& other) {
    if (this != &other) {
        if (this->refcount && --(*this->refcount) == 0) {
            delete[] this->pBuf;
            delete this->refcount;
        }

        this->pBuf = other.pBuf;
        this->len = other.len;
        this->refcount = other.refcount;

        if (this->refcount) {
            // Increment the value pointed to by the shared reference counter
            (*this->refcount)++;
        }
    }
    return *this;
}

String::~String() {

    if (this->refcount){
        (*this->refcount)--;

        if ((*this->getRefCount()) == 0) {
            delete[] this->pBuf;
            delete this->refcount;
        }
    }

    return;
}

void String::show(ostream& os) {
    if (this->pBuf == NULL) return;
    else os << this->pBuf;
    return;
}

void String::show() {
    this->show(cout);
    return;
}
