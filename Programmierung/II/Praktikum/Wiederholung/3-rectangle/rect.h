#ifndef _RECT_H_
#define _RECT_H_

#include <iostream>

using namespace std;

class Rect {
    private:
        int width;
        int heigth;
        int posx;
        int posy;
    
    public:
        Rect(int x, int y, int posx, int posy) {setWidth(x); setHeight(y); setPosx(posx); setPosy(y);}

        void setWidth(int width) {this->width = width;}
        void setHeight(int heigth) {this->heigth = heigth;}
        void setPosx(int x) {this->posx = x;}
        void setPosy(int y) {this->posy = y;}

        int getWidth() const {return width;}
        int getHeight() const {return heigth;}
        int getPosx() const {return posx;}
        int getPosy() const {return posy;}

        void draw() const;
        void print() const;
        void move(int x, int y) {posx += x; posy += y;}
        void scale(int factor) {width *= factor; heigth *= factor;}

        friend ostream& operator<<(ostream& os, const Rect& r);
};

#endif