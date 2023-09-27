#ifndef RECT_H_
#define RECT_H_

#include "ui_shared_headers.h"


class Rect {
public:
    int x,y,w,h;

    Rect(): x(0),y(0),w(0),h(0)  {}
    Rect(Rect& rect): x(rect.x),y(rect.y),w(rect.w),h(rect.h)  {}
    Rect(int _x ,int _y ,int _w,int _h): x(_x),y(_y),w(_w == -1 ? 0 : _w),h(_h == -1 ? 0 : _h)  {}
    ~Rect() {}


    operator SDL_Rect() const {
        return (SDL_Rect){x,y,w,h};
    }


    void set_size(int _w = -1 , int _h = -1) {
        w = _w == -1 ? w : _w;
        h = _h == -1 ? h : _h;
    }

    void set_pos(int _x , int _y) { 
        x = _x;
        y = _y;
    }


    void set_rect(int _x ,int _y, int _w = -1 , int _h = -1) {
        set_pos(_x,_y);
        set_size(_w,_h);
    }


    void print() {
        printf("%d %d %d %d\n",x,y,w,h);
    }
};


typedef struct Offset {
    int x , y;

    Offset() {x = 0; y = 0;}
    Offset(int _x , int _y) {x = _x; y = _y;}

    void set(int _x, int _y) {
        x = _x; 
        y = _y;
    }
} Offset;

static const Offset ZeroOffset = Offset(); 

#endif // RECT_H_