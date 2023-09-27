#include <stdio.h>
#include <time.h>

#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "globals.h"


#include "ui/base_rect.h"
#include "ui/container.h"
#include "ui/text.h"


// consts
#define WIDTH 600
#define HEIGHT 400

// namespaces

// globals
static time_t cur_time;
static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Event event;
static TTF_Font* font;



// functinos
// init
void init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    Globals::load_font("font/IBMPlexMono-Bold.ttf");

    window = SDL_CreateWindow(
                        "Domes",
                        SDL_WINDOWPOS_CENTERED, 
                        SDL_WINDOWPOS_CENTERED, 
                        WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);    

}
// quit
void quit();
// get_cur_date
tm* get_cur_date() {
    cur_time = time(0);
    return localtime(&cur_time);;
}
// get_date_str
std::string get_date_str(const char* seprator = "/") {
    tm* parsed_time = get_cur_date();
    char date[30];
    sprintf(date,"%d%s%d%s%d",
                parsed_time->tm_mday,
                seprator,
                parsed_time->tm_mon + 1,
                seprator,
                parsed_time->tm_year + 1900);

    std::string date_string(date);
    return date_string;
}
// get_time_str
std::string get_time_str(const char* seprator = ":") {
    tm* parsed_time = get_cur_date();
    char date[30];
    sprintf(date,"%02d%s%02d%s%02d",
                parsed_time->tm_hour,
                seprator,
                parsed_time->tm_min,
                seprator,
                parsed_time->tm_sec);

    std::string date_string(date);
    return date_string;
}



int main()
{
    init();

    UIContainer app(0,0,WIDTH,HEIGHT);
    app.styles.set_padding("10","20","10","10");
    app.styles.set_margin("10","10","10","10");

    UIText text_date;
    text_date.print_text(renderer,"2022-10-15");
    text_date.styles.set_margin("10","10","20","10");
    text_date.styles.Horz = Right;

    UIText text_time;
    text_time.styles.set_margin("10","10","20","10");
    text_time.styles.Horz = Right;


    app.append(&text_date);
    app.append(&text_time);


    // children

    bool done = false;
    while (!done)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
            case SDL_QUIT: done = true; break;
            case SDL_KEYDOWN:
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    done = true;
                }
            }
            break;
            
            default:
                break;
            }
        }

        
        // update
        text_date.print_text(renderer,get_date_str().c_str());
        text_time.print_text(renderer,get_time_str().c_str());
        app.update();


        // render
        SDL_SetRenderDrawColor(renderer, hex_to_color(0x000000));
        SDL_RenderClear(renderer);


        app.render(renderer);

        // AppBody.render(renderer);

        SDL_RenderPresent(renderer);
    }


    // free all
    quit();

    return 0;
}


void quit() { 

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    Globals::memory_free();

    TTF_Quit();
    SDL_Quit();

}
