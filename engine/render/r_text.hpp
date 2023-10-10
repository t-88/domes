#ifndef RENDER_TEXT_HPP
#define RENDER_TEXT_HPP

#include <SDL2/SDL.h>
#include "r_node.hpp"

class RText : RNode {
public:
    SDL_Texture* texture = 0;
    char* text;
    SDL_Color text_color;
    

    void print_text(const char* _text) {
        text = (char*) _text;
        SDL_Surface* text_surface = TTF_RenderText_Solid(Globals::font,text,(SDL_Color){255,255,255,255});
        
        texture = SDL_CreateTextureFromSurface(Globals::renderer,text_surface);

        SDL_FreeSurface(text_surface);       
    }

    RText(LayoutNode* lnode): RNode(lnode) {
        print_text(((Text*)lnode->style_node->dom_node)->text.c_str());
    }
    ~RText() {
        SDL_DestroyTexture(texture);
    }


    void render(SDL_Renderer* renderer) override {
        SDL_Rect main_rect = (SDL_Rect) lnode->box.get_rect();
        SDL_RenderCopy(renderer,texture,NULL,&main_rect);
    }

};

#endif // RENDER_TEXT_HPP