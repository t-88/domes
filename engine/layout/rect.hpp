#ifndef ENGINE_LAYOUT_RECT_H
#define ENGINE_LAYOUT_RECT_H




class Rect {
public:
    int x, y, w, h;

    Rect():x(0),y(0),w(0),h(0) { }
    Rect(int _x,int _y,int _w,int _h):x(_x),y(_h),w(_w),h(_h) {

    }

    ~Rect() {}
};

#endif // ENGINE_LAYOUT_RECT_H