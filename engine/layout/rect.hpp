#ifndef ENGINE_LAYOUT_RECT_H
#define ENGINE_LAYOUT_RECT_H

#include <SDL2/SDL.h>


class Rect {
public:
    int x, y, w, h;

    Rect():x(0),y(0),w(0),h(0) { }
    Rect(int _x,int _y,int _w,int _h):x(_x),y(_y),w(_w),h(_h) {

    }

    ~Rect() {}

    void print() {
        printf("%d %d %d %d\n",x,y,w,h);
    }

    operator SDL_Rect() {
        return (SDL_Rect) {x,y,w,h};
    }
};

#endif // ENGINE_LAYOUT_RECT_H