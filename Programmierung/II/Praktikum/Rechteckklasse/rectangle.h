#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <math.h>

class rect
{
    float a;
    float b;
    float posx;
    float posy;
    public:
        rect(float a, float b) {this->rect(a,b, 0, 0);};
        rect(float a, float b, float x, float y) {this->setsize(a,b); this->setpos(x,y);};
        // static rect* create();
        float aera() {return a*b;};
        float circumref() {return 2*(a+b);};
        int setsize(float a, float b) {this->a=fabsf(a); this->b=fabsf(b);};
        float getsize() {return 0.0;};
        int setpos(float x, float y) {this->posx=x; this->posy=y;};
        float getpos() {return 0.0;};
        int draw();
};

#endif