
#ifndef UI_TEXT_H_
#define UI_TEXT_H_

#include "ui_shared_headers.h"
#include "rect.h"
#include "style.h"
#include "base_rect.h"

#include "../globals.h"

#include <vector>
#include <assert.h>

class UIText : public UIContainer {
public:
    SDL_Texture* texture = 0;
    char* text;
    SDL_Color text_color;

    UIText() : UIContainer(0,0,0,0) { 
        text_color = Color::White;
    }
    UIText(std::string text,SDL_Color _color = Color::White) : UIContainer(0,0,0,0) { 
        text_color = _color;
    }
    
    void print_text(SDL_Renderer* renderer, const char* _text) {
        text = (char*) _text;

        SDL_Surface* text_surface = TTF_RenderText_Solid(Globals::font,text,text_color);
        
        texture = SDL_CreateTextureFromSurface(renderer,text_surface);
        
        main_rect.set_rect(text_surface->clip_rect.x,text_surface->clip_rect.y,text_surface->clip_rect.w,text_surface->clip_rect.h);

        SDL_FreeSurface(text_surface);       
    }


    void calc_outer_bbox() override {
        outer_bbox = rect;
        Offset p1(rect.x,rect.y);
        Offset p2(rect.x + rect.w,rect.y + rect.h);

        StyleContext context = {rect.w,rect.h};
        RelativeStyleSetting r_style = styles.parse_style(context);

        // // top
        // if(p1.y - r_style.margin[0] >= parent->inner_bbox.y) {
        //     p1.y -= r_style.margin[0];
        // } else {
        //     p1.y = parent->inner_bbox.y;
        //     p2.y += r_style.margin[0];
        // }

        // // buttom
        // if(p2.y + r_style.margin[2] <= parent->inner_bbox.y + parent->inner_bbox.h) {
        //     p2.y += r_style.margin[2];
        // } else {
        //     p2.y = parent->inner_bbox.y + parent->inner_bbox.h;
        //     rect.y -= r_style.margin[2];
        // }

        // // right
        // if(p2.x + r_style.margin[1] <= parent->inner_bbox.x + parent->inner_bbox.w) {
        //     p2.x -= r_style.margin[1];
        // } else {
        //     p2.x = parent->inner_bbox.x + parent->inner_bbox.w;
        //     p1.x -= r_style.margin[1];
        //     rect.x = p1.x;
        // }

        // // left
        // if(p2.x - r_style.margin[3] >= 0) {
        //     p1.x -= r_style.margin[3];
        // } else {
        //     // p2.y = parent->inner_bbox.y + parent->inner_bbox.h;
        //     // rect.y -= r_style.margin[2];
        //     assert(false && "not implemented");
        // }


        // outer_bbox.x = p1.x;
        // outer_bbox.y = p1.y;
        // outer_bbox.w = p2.x - p1.x;
        // outer_bbox.h = p2.y - p1.y;

        // rect.x = outer_bbox.x;
        // rect.y += r_style.margin[0];
    }

    void update(Offset l_offset = ZeroOffset,Offset r_offset = ZeroOffset) override {
        UIBaseRect::update(l_offset,r_offset);
    }
    void render(SDL_Renderer* renderer) override {
        UIBaseRect::render(renderer);


        if(!texture) return;
        SDL_Rect temp_rect = rect;

        SDL_RenderCopy(renderer,texture,0,&temp_rect);
    }


    ~UIText() {
        SDL_DestroyTexture(texture);
    }

};





#endif //UI_TEXT_H_