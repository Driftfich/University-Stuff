#include <iostream>
#include <cstring>
#include "stringg.h"

using namespace std;

STRING::STRING() {
    this->pBuf = NULL;
    this->Len = 0;
}

STRING::STRING(const char* pStr) {
    this->Len = strlen(pStr);
    this->pBuf = new char[this->Len+1];
    strcpy(this->pBuf, pStr);
}

STRING::STRING(const STRING& other) {
    STRING(other.pBuf);
}

STRING::STRING(char C, int n) {
    this->pBuf = new char[n+1];
    for (int i=0; i<n; i++) {
        pBuf[i] = C;
    }
    this->pBuf[n] = '\0';
    this->Len = n;
}

void STRING::show() {
    cout << this->pBuf;
}

ostream& operator<<(ostream& os, const STRING& val) {
    os << val.pBuf;
    return os;
}

STRING& STRING::operator=(const STRING& other) {
    cout << "Calling assingment" << endl;
    this->pBuf = new char[other.Len + 1];
    strcpy(this->pBuf, other.pBuf);
    this->Len = other.Len;
    return *this;
}

STRING operator+(const STRING& f, const STRING& s) {
    char* str = new char[f.Len + s.Len + 1];
    strcpy(str, f.pBuf);
    strcpy(str + f.Len, s.pBuf);
    return STRING(str);
}

STRING& STRING::operator+=(const STRING& s) {
    int nlen = this->Len + s.Len + 1;
    char *tmp = new char[nlen];
    strcpy(tmp, this->pBuf);
    strcpy(tmp + this->Len, s.pBuf);
    this->Len = nlen - 1;
    return *this;
}

char& STRING::operator[](const int idx) {
    return this->pBuf[idx];
}

STRING::~STRING() {
    delete[] this->pBuf;
}