#ifndef RENDER_NODE_HPP
#define RENDER_NODE_HPP

#include <SDL2/SDL.h>
#include "../layout/layout_node.hpp"
#include "../layout/box.hpp"

class RNode {
public:
    LayoutNode* lnode;

    RNode(LayoutNode* lnode) : lnode(lnode) {}
    ~RNode() {}


    virtual void render(SDL_Renderer* renderer)  {

    }
};

#endif // RENDER_NODE_HPP