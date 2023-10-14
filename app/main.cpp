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



void prop_tree_print(LayoutNode* layout_root,int depth = 0) {
    auto prop_print = [](int depth = 0,LayoutNode* lr) {
        printf("%*s%s x: %d y: %d w: %d h: %d\n",depth * 4,"",lr->node->id.c_str(),lr->box.rect.x,lr->box.rect.y,lr->box.rect.w,lr->box.rect.h);
    };

    if(depth == 0) {
        prop_print(depth,layout_root);
        depth++;
    }
    for (size_t i = 0; i < layout_root->nodes.size(); i++) {
        prop_print(depth,layout_root->nodes[i]);
        prop_tree_print(layout_root->nodes[i], depth + 1);
    }
    
}

void logo() {
    printf("logo\n");
}
void todo() {
    printf("todo\n");
}

void onRemoveTodo(void* index) {
    printf("removing %d\n",userdata_to(int,index));

}
void onFinishTodo(void* index) {
    printf("todo %d is finished\n",userdata_to(int,index));
}


Column todo_container;

void onScroll(Event::Event event) {
    todo_container.scroll_y(event.scroll_dir_y * 10);
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
        "height : 345;"
        "margin_bottom: 20;"
    );
    todo_container.onScrollCallback = onScroll;



    for (size_t i = 0; i < 10; i++) {
        todo_container.push(new Card(i));
    }
    root.push(&todo_container);





    LayoutNode layout_tree(&root);
    layout_tree.build_tree();
    layout_tree.lay_it_out();




    DisplayBuffer buffer;
    buffer.build_buffer(&layout_tree);



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


