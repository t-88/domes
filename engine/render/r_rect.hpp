#ifndef RENDER_RECT_HPP
#define RENDER_RECT_HPP

#include <SDL2/SDL.h>
#include "r_node.hpp"

class RRect : RNode {
public:

    RRect(LayoutNode* lnode): RNode(lnode) {}
    ~RRect() {}


    void render(SDL_Renderer* renderer) override {
        SDL_Rect main_rect = (SDL_Rect) lnode->box.get_rect();
        SDL_Rect margin_rect = (SDL_Rect) lnode->box.get_margin_rect();

        auto margin_color = to_color(lnode->style_node->dom_node->style.props["margin_color"]);
        SDL_SetRenderDrawColor(renderer,margin_color.r,margin_color.g,margin_color.b,margin_color.a);
        SDL_RenderFillRect(renderer,&margin_rect);


        auto main_color = to_color(lnode->style_node->dom_node->style.props["color"]);
        SDL_SetRenderDrawColor(renderer,main_color.r,main_color.g,main_color.b,main_color.a);
        SDL_RenderFillRect(renderer,&main_rect);
    }

};

#endif // RENDER_RECT_HPP