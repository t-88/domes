#include <stdio.h>
#include <time.h>

#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../engine/dom/node.hpp"
#include "../engine/dom/text.hpp"
#include "../engine/dom/element.hpp"
#include "../engine/dom/column.hpp"

#include "../engine/style/style_node.hpp"
#include "../engine/layout/layout_node.hpp"
#include "../engine/render/display_buffer.hpp"
#include "../engine/events/event.hpp"

#include "../globals.h"

#include "card.hpp"
#include "utils.hpp"
#include "state.hpp"


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




static Column todo_container;
static int idx = 0; 


void onRemoveTodo(void* userdata) {
    int idx = userdata_to(int,userdata);
    int i = 0;
    for (i = 0; i < todo_container.children.size(); i++) {
        if(((Card*)todo_container.children[i])->uid == idx) {
            break;
        }
    }
    delete todo_container.children[i];
    todo_container.children.erase(todo_container.children.begin() + i);
}
void onFinishTodo(void* index) {
    printf("todo %d is finished\n",userdata_to(int,index));
}




void onScroll(Event::Event event) {
    todo_container.scroll_y(event.scroll_dir_y * 25);
}
void addTodo(void* userdata) {
    todo_container.push(new Card(idx));
    idx++;
}

int main() {
    init();

    onRemoveTodoCallback = onRemoveTodo;
    onFinishTodoCallback = onFinishTodo;

    Element root("app");
    root.style->props["width"] = std::to_string(Globals::width).c_str();
    setStyle(
        root,
        "color : 0,0,0,255;"
        "margin_top : 0;"
        "margin_bottom : 0;"
    );

    Text logo_text("DoMe");
    setStyle(
        logo_text,
        "display : " DisplayInline ";"
        "color : 255,0,0,255;"
        "margin_left :   10;"
        "margin_right :  230;"
        "margin_bottom : 20;"
    ); 

    Text date_text(get_date_str());
    setStyle(
        date_text,
        "display:" DisplayInline ";"
        "color: 255,0,0,255;"   
        "margin_right: 20;"   
    );

    Text time_text(get_time_str());
    setStyle(
        time_text,
        "display: " DisplayInline ";"
        "color: 255,0,0,255"
    );

    root.children.push_back(&logo_text);
    root.children.push_back(&date_text);
    root.children.push_back(&time_text);



    setStyle(
        todo_container,
        "color : 0,255,0,0;"
        "height : 280;"
        "margin_bottom: 20;"
    );
    todo_container.onScrollCallback = onScroll;

    root.push(&todo_container);

    Element add_btn("add-btn");
    setStyle(
        add_btn,
        "width :  30;"
        "height : 30;"
        "color : 255,0,0,255;"
    );
    add_btn.onClickCallback = addTodo;
    root.push(&add_btn);





    LayoutNode layout_tree(&root);
    layout_tree.is_root = true;



    DisplayBuffer buffer;



    Uint32 start = SDL_GetTicks();
    Uint32 end = start;
    float dt;
    float frame_rate = 1 / 60.f;

    bool done = false;
    while (!done)
    {
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
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
                layout_tree.fire_event((Event::Event) {
                    .type = Event::MouseClickEvent,
                    .type_state = Event::MouseDown,
                    .x = event.button.x,
                    .y = event.button.y,
                    .button = event.button.button,
                });
            break;
            case SDL_MOUSEWHEEL:
                layout_tree.fire_event((Event::Event) {
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
        layout_tree.clear();
        layout_tree.build_tree();
        layout_tree.lay_it_out();
        time_text.text = get_time_str();
        buffer.build_buffer(&layout_tree);
        




        // render
        SDL_SetRenderDrawColor(renderer, hex_to_color(0x000000));
        SDL_RenderClear(renderer);
        buffer.render(renderer);




        SDL_RenderPresent(renderer);


    }


    // free all
    quit();

    return 0;
}


