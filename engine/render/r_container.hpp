#ifndef RENDER_CONTAINER_HPP
#define RENDER_CONTAINER_HPP

#include <SDL2/SDL.h>
#include "../layout/layout_node.hpp"
#include "../layout/box.hpp"
#include "r_node.hpp"

class RContainer : public RNode {
public:
    LayoutNode* lnode;

    RContainer(LayoutNode* _lnode)  : RNode(lnode) {
    }
    ~RContainer() {
    }


    void render(SDL_Renderer* renderer) override  {
        return;
        // if(surface) {
        //     SDL_FreeSurface(surface);
        //     surface = nullptr;
        // }

        // SDL_Rect main_rect = (SDL_Rect) lnode->box.get_rect();
        // SDL_Rect margin_rect = (SDL_Rect) lnode->box.get_margin_rect();

        // surface = SDL_CreateRGBSurface(0,margin_rect.w,margin_rect.h,32,0,0,0,0);

        // auto props = lnode->node->style->props;

        // auto margin_color = to_color(lnode->node->style->props["margin_color"]);
        // if(margin_color.a != 0) {
        //     SDL_FillRect(
        //                  surface,&margin_rect,
        //                  margin_color.r << 12 | margin_color.g << 8 | margin_color.b << 4 | margin_color.a << 0
        //                 );
        // }


        // auto main_color = to_color(lnode->node->style->props["color"]);
        // if(main_color.a != 0) {
        //     SDL_FillRect(
        //                  surface,&main_rect,
        //                  margin_color.r << 12 | margin_color.g << 8 | margin_color.b << 4 | margin_color.a << 0
        //                 );
        // } 
    }
};

#endif // RENDER_CONTAINER_HPP