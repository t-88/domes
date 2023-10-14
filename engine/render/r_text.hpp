#ifndef RENDER_TEXT_HPP
#define RENDER_TEXT_HPP

#include <SDL2/SDL.h>
#include "r_node.hpp"
#include "../layout/rect.hpp"


class RText : RNode {
public:
    char* text;
    SDL_Color text_color;
    Rect* p_surface;    

    SDL_Rect clip_rect;

    void print_text(const char* _text) {
        text = (char*) _text;
        SDL_Surface* text_surface = TTF_RenderText_Solid(Globals::font,text,(SDL_Color){255,255,255,255});
        
        texture = SDL_CreateTextureFromSurface(Globals::renderer,text_surface);

        SDL_FreeSurface(text_surface);       
    }

    RText(LayoutNode* lnode,Rect* _p_rect = nullptr,int _offset_y = 0): RNode(lnode,_p_rect,_offset_y) {
        print_text(((Text*)lnode->node)->text.c_str());
    }

    ~RText() {
        SDL_DestroyTexture(texture);
    }



    void caculate_render() {
        main_rect = (SDL_Rect) lnode->box.get_rect();
        margin_rect = (SDL_Rect) lnode->box.get_margin_rect();


        main_rect.y += offset_y;
        margin_rect.y += offset_y;

        clip_rect = main_rect;
        clip_rect.x = 0; clip_rect.y = 0;


        auto props = lnode->node->style->props;
        main_color = to_color(lnode->node->style->props["color"]);
        margin_color = to_color(lnode->node->style->props["margin_color"]);




        if(!p_rect) return;

        // TODO: i dont handle margin rect overflow


        if(main_rect.x < p_rect->x) {
            main_rect.w -= p_rect->x - main_rect.x; 
            if(main_rect.w < 0) main_rect.w = 0;

            clip_rect.x = p_rect->x - main_rect.x; 
            main_rect.x = p_rect->x;
        }
        if(main_rect.y < p_rect->y) {
            main_rect.h -= p_rect->y - main_rect.y; 
            if(main_rect.h < 0) main_rect.h = 0;
            clip_rect.y = p_rect->y - main_rect.y; 


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


        clip_rect.w = main_rect.w;
        clip_rect.h = main_rect.h;
    }


    void render(SDL_Renderer* renderer) override {
        if(re_render) {
            caculate_render();
            re_render = false;
        }



        SDL_RenderCopy(renderer,texture,&clip_rect,&main_rect);
    }

};

#endif // RENDER_TEXT_HPP