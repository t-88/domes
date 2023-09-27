#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace Globals {
    void memory_free();


    TTF_Font* font;
    int font_size = 20;

    void load_font(const char* file_path) {
        if(font) memory_free();
        font = TTF_OpenFont(file_path,font_size);
    }


    void memory_free() {
        TTF_CloseFont(font);
    }

} // namespace Globals




#endif // GLOBALS_H