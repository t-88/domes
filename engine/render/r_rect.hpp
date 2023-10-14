#ifndef RENDER_RECT_HPP
#define RENDER_RECT_HPP

#include <SDL2/SDL.h>
#include "r_node.hpp"

class RRect : RNode {
public:

    RRect(LayoutNode* lnode,Rect* _p_rect = nullptr,int _offset_y = 0): RNode(lnode,_p_rect,_offset_y) {}
    ~RRect() {}


};

#endif // RENDER_RECT_HPP