#ifndef RENDER_NODE_HPP
#define RENDER_NODE_HPP

#include <SDL2/SDL.h>
#include "../layout/layout_node.hpp"
#include "../layout/box.hpp"

class RNode {
public:
    LayoutNode* lnode;
    SDL_Texture* texture = 0;

    SDL_Rect main_rect;
    SDL_Rect margin_rect;
    SDL_Color margin_color;
    SDL_Color main_color;

    bool re_render = true;
    Rect* p_rect = nullptr;

    int offset_y = 0;

        

    RNode(LayoutNode* lnode,Rect* _p_rect = nullptr,int _offset_y = 0) : lnode(lnode) {
        p_rect = _p_rect;
        offset_y = _offset_y;
    }
    ~RNode() {}

    void caculate_render() {
        main_rect = (SDL_Rect) lnode->box.get_rect();
        margin_rect = (SDL_Rect) lnode->box.get_margin_rect();


        main_rect.y += offset_y;
        margin_rect.y += offset_y;


        auto props = lnode->node->style->props;
        main_color = to_color(lnode->node->style->props["color"]);
        margin_color = to_color(lnode->node->style->props["margin_color"]);



        if(!p_rect) return;

        // TODO: i dont handle margin rect overflow


        if(main_rect.x < p_rect->x) {
            main_rect.w -= p_rect->x - main_rect.x; 
            if(main_rect.w < 0) main_rect.w = 0;
            main_rect.x = p_rect->x;
        }
        if(main_rect.y < p_rect->y) {
            main_rect.h -= p_rect->y - main_rect.y; 
            if(main_rect.h < 0) main_rect.h = 0;

            main_rect.y = p_rect->y;
        }

        int overflow_w = p_rect->x + p_rect->w - (main_rect.x + main_rect.w); 
        if(main_rect.w != 0 && overflow_w < 0) {
            main_rect.w += overflow_w;
            if(main_rect.w < 0) main_rect.w = 0;
        }

        int overflow_h = p_rect->y + p_rect->h - (main_rect.y + main_rect.h); 
        if(main_rect.h != 0 &&overflow_h < 0) {
            main_rect.h += overflow_h;
            if(main_rect.h < 0) main_rect.h = 0;
        }
        
    }

    virtual void render(SDL_Renderer* renderer)  {
        if(re_render) {
            caculate_render();
            re_render = false;
        }


        if(margin_color.a != 0) {
            SDL_SetRenderDrawColor(renderer,margin_color.r,margin_color.g,margin_color.b,margin_color.a);
            SDL_RenderFillRect(renderer,&margin_rect);
        }
        if(main_color.a != 0) {
            SDL_SetRenderDrawColor(renderer,main_color.r,main_color.g,main_color.b,main_color.a);
            SDL_RenderFillRect(renderer,&main_rect);
        }
    }
};

#endif // RENDER_NODE_HPP