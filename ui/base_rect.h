#ifndef UI_BASE_RECT_H
#define UI_BASE_RECT_H

#include "ui_shared_headers.h"
#include "rect.h"
#include "style.h"

class UIBaseRect {
public:
    Rect main_rect;
    Rect rect;
    Rect inner_bbox;
    Rect outer_bbox;

    SDL_Color color_inner_bbox = Color::inner_bbox;
    SDL_Color color_outer_bbox = Color::outer_bbox;
    SDL_Color color = Color::bbox;
    StyleSetting styles;

    UIBaseRect* parent;

    int index;

    int max_children = 11;
    std::vector<UIBaseRect*> children;



    UIBaseRect() { main_rect.set_rect(0,0,0,0); }
    UIBaseRect(int x,int y,int w,int h) { main_rect.set_rect(x,y,w,h); }
    UIBaseRect(int w,int h) { main_rect.set_size(w,h); }


    void calc_inner_bbox() {
        inner_bbox = rect;

        StyleContext context = {rect.w,rect.h};
        RelativeStyleSetting r_style = styles.parse_style(context);
        
        inner_bbox.y += r_style.padding[0];
        inner_bbox.w -= r_style.padding[1] + r_style.padding[3];
        inner_bbox.h -= r_style.padding[2] + r_style.padding[0];
        inner_bbox.x += r_style.padding[3];
    }
    
    virtual void calc_outer_bbox() {
        if(!parent) {
            rect = main_rect;
            outer_bbox = rect;

            StyleContext context = {rect.w,rect.h};
            RelativeStyleSetting r_style = styles.parse_style(context);

            rect.y += r_style.margin[0];
            rect.w -= r_style.margin[1] + r_style.margin[3];
            rect.h -= r_style.margin[2] + r_style.margin[0];
            rect.x += r_style.margin[3];

        } else {
            UIBaseRect* collided_with;
            if(((UIContainer*)parent)->child_collided(this,collided_with)) {

            }

        }

    }

    void update_offset(Offset* l_offset,Offset* r_offset) { 
        if(styles.Horz == Left) {
            if(!l_offset) return;
            l_offset->set(outer_bbox.x + outer_bbox.w,outer_bbox.y);
        } else if(styles.Horz == Right) {
            if(!r_offset) return;
            r_offset->set(outer_bbox.x,outer_bbox.y);
        }
    }


    virtual void update(Offset l_offset = ZeroOffset,Offset r_offset = ZeroOffset) {
        outer_bbox = main_rect;
        calc_outer_bbox();





        calc_inner_bbox();



        // rect.print();
        // inner_bbox.print();
    }
    
    virtual void render(SDL_Renderer* renderer) {

        SDL_Rect sdl_outer_bbox = outer_bbox;
        SDL_SetRenderDrawColor(renderer,color_spread(color_outer_bbox));
        SDL_RenderDrawRect(renderer,&sdl_outer_bbox);        

        SDL_Rect sdl_inner_rect = inner_bbox;
        SDL_SetRenderDrawColor(renderer,color_spread(color_inner_bbox));
        SDL_RenderDrawRect(renderer,&sdl_inner_rect);


        SDL_Rect sdl_rect = rect;
        SDL_SetRenderDrawColor(renderer,color_spread(color));
        SDL_RenderDrawRect(renderer,&sdl_rect);

    }
};


#endif