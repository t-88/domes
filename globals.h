#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#define hex_to_color(color) ((color) & 0xFF >> 8 * 2) ,  ((color) & 0xFF >> 8 * 1) , ((color) & 0xFF >> 8 * 0) , 0xFF 
#define color_spread(color) (color).r,(color).g,(color).b,(color).a 


#define FONT_PATH "./font/SpaceMono-Regular.ttf"
#define TODOS_DB_PATH "todos.db"


namespace Globals {
    int width , height;
    int font_width;
    int font_height;

    SDL_Renderer* renderer;

    void set_window_sizing(int w, int h) {
        width = w;
        height = h;
    }


    void memory_free();


    TTF_Font* font;
    int font_size = 25;

    void load_font(const char* file_path) {
        if(font) memory_free();
        font = TTF_OpenFont(file_path,font_size);
        assert(font);
        TTF_SizeText(font,"A",&font_width,&font_height);
    }


    void memory_free() {
        TTF_CloseFont(font);
    }

} 




#endif // GLOBALS_H