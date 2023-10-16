#include <stdio.h>
#include <time.h>

#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>



#include "../globals.h"

#include "utils.hpp"
#include "state.hpp"


#include "./app.hpp"

// namespaces

// globals
static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Event event;
static TTF_Font* font;


// init
void init() {
    Globals::set_window_sizing(600,400);
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    Globals::load_font("font/SpaceMono-Regular.ttf");

    window = SDL_CreateWindow(
                        "Domes",
                        SDL_WINDOWPOS_CENTERED, 
                        SDL_WINDOWPOS_CENTERED, 
                        Globals::width, Globals::height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);    


    Globals::renderer = renderer;
}
// quit
void quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    Globals::memory_free();

    TTF_Quit();
    SDL_Quit();

}




int main() {
    init();
    TodoApp app;
    app.load_todos();


    Uint32 start = SDL_GetTicks();
    Uint32 end = start;
    float dt;
    float frame_rate = 1 / 60.f;

    bool done = false;
    while (!done) {
        start = SDL_GetTicks();
        dt = (start - end) / 1000.f;
        if(frame_rate > dt) { continue; }
        end = start;


        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
            case SDL_QUIT: done = true; break;
            case SDL_KEYDOWN:
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    done = true;
                }
                app.fire_event((Event::Event) {
                    .type = Event::KeyPressEvent,
                    .key = event.key.keysym.scancode
                });

            }

            break;
            case SDL_TEXTINPUT:
                app.fire_event((Event::Event) {
                    .type = Event::KeyPressEvent,
                    .text = event.text.text,
                });
            break;

            case SDL_MOUSEBUTTONDOWN:
                app.fire_event((Event::Event) {
                        .type = Event::MouseClickEvent,
                        .type_state = Event::MouseDown,
                        .x = event.button.x,
                        .y = event.button.y,
                        .button = event.button.button,
                    }
                );
            break;
            case SDL_MOUSEWHEEL:
                app.fire_event((Event::Event) {
                    .type = Event::MouseScrollEvent,
                    .type_state = Event::MouseDown,
                    .x = event.wheel.mouseX,
                    .y = event.wheel.mouseY,
                    .scroll_dir_x = (int) event.wheel.preciseX,
                    .scroll_dir_y = (int) event.wheel.preciseY,
                });
            break;
            default:
                break;
            }
        }


        // update
        app.update(dt);
        

        // render
        SDL_SetRenderDrawColor(renderer, hex_to_color(0x000000));
        SDL_RenderClear(renderer);
        app.render();



        SDL_RenderPresent(renderer);
    }


    app.save_todos();

    // free all
    quit();

    return 0;
}


