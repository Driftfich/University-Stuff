#ifndef _STRINGG_H_
#define _STRINGG_H_

#include <iostream>

using namespace std;

class STRING
{
        char * pBuf;
        int    Len;
    public:
        STRING ();
        STRING (const char* pStr);
        STRING (const STRING& other);
        STRING (char C, int n);
        ~STRING();

        int GetLength() {return this->Len;};
        void show();
        friend ostream& operator<<(ostream& os, const STRING& other);
        char& operator[](int index);
        friend STRING operator+(const STRING& f, const STRING& s);
        STRING& operator+=(const STRING& s);
        STRING& operator=(const STRING& other);
};

#endif