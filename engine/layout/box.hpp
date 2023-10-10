#ifndef ENGINE_LAYOUT_BOX_H
#define ENGINE_LAYOUT_BOX_H

#include "rect.hpp"
#include "edge.hpp"



class Box {
public:
    Rect rect;
    Edge padding;
    Edge margin;


    Box() {}
    ~Box() {}


    Rect get_rect() { 
        return rect;
    }

    Rect get_margin_rect() {
        return Rect(
            rect.x - margin.left,
            rect.y - margin.top,
            rect.w + margin.left + margin.right,
            rect.h + margin.top + margin.bottom
        );
    }
};

#endif // ENGINE_LAYOUT_BOX_H