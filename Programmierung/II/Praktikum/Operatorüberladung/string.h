// Franz Rehschuh

#ifndef _STRING_H_
#define _STRING_H_

#include <iostream>

using namespace std;

class String
    {
        char * pBuf;
        int    len;
        int * refcount;
    public:
        String ();
        String (const char* pStr);
        String (char c, int n);
        String (const String& other); // copy constructor
        String& operator=(const String& other); // copy assignment operator
        ~String(); // destructor
        int* getRefCount () {return this->refcount;};
        void show();
        void show(ostream& os);
        // char& CIdx(int i); // liefert eine Referenz auf das i-te Zeichen
    };


#endif