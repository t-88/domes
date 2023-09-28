#ifndef ENGINE_LAYOUT_BOX_H
#define ENGINE_LAYOUT_BOX_H

#include "rect.hpp"
#include "edge.hpp"



class Box {
public:
    Rect rect;
    Edge padding;
    Edge margin;


    Box(/* args */) {
        
    }
    ~Box() {}
};

#endif // ENGINE_LAYOUT_BOX_H