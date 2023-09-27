#ifndef UI_COLOR_H_
#define UI_COLOR_H_

#include "ui_shared_headers.h"

namespace Color {
    SDL_Color White = {255, 255 , 255,255}; 
    SDL_Color Red = {255, 0 , 0,255}; 
    SDL_Color Green = {0, 255 , 0,255}; 
    SDL_Color Blue = {0, 0 , 255,255}; 
    SDL_Color Purple = {255, 0 , 255,255}; 


    SDL_Color inner_bbox = Red;
    SDL_Color outer_bbox = Green;
    SDL_Color bbox = Blue;
}


#endif // UI_COLOR_H_