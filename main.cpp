#include <stdio.h>
#include <time.h>

#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "globals.h"

#include "engine/dom/node.hpp"
#include "engine/dom/text.hpp"
#include "engine/dom/element.hpp"

#include "engine/style/style_node.hpp"
#include "engine/layout/layout_node.hpp"

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
                        Globals::width, Globals::height, 0);
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




void prop_tree_print(LayoutNode* layout_root,int depth = 0) {
    auto prop_print = [](int depth = 0,LayoutNode* lr) {
        printf("%*s%s x: %d y: %d w: %d h: %d\n",depth * 4,"",lr->style_node->dom_node->id.c_str(),lr->box.rect.x,lr->box.rect.y,lr->box.rect.w,lr->box.rect.h);
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

void layout_tree_render(SDL_Renderer* renderer,LayoutNode lr) {
    SDL_Rect main_rect = (SDL_Rect) lr.box.get_rect();
    SDL_Rect margin_rect = (SDL_Rect) lr.box.get_margin_rect();



    auto margin_color = to_color(lr.style_node->dom_node->style.props["margin_color"]);
    SDL_SetRenderDrawColor(renderer,margin_color.r,margin_color.g,margin_color.b,margin_color.a);
    SDL_RenderFillRect(renderer,&margin_rect);


    auto main_color = to_color(lr.style_node->dom_node->style.props["color"]);
    SDL_SetRenderDrawColor(renderer,main_color.r,main_color.g,main_color.b,main_color.a);
    SDL_RenderFillRect(renderer,&main_rect);


    for (size_t i = 0; i < lr.nodes.size(); i++) {
        layout_tree_render(renderer,*lr.nodes[i]);
    }

}


int main() {
    Globals::set_window_sizing(600,400);


    Element root("app");
    root.style.props["width"] =  std::to_string(Globals::width);
    root.style.props["margin_top"] = "0";
    root.style.props["margin_bottom"] = "0";


    Element body("body");
    body.set_style("color","0 255 0 255");
    body.set_style("margin_color","0 0 0 255");
    body.style.props["height"] = "50";  
    body.style.props["margin_top"] = "10";
    body.style.props["margin_bottom"] = "10";

    Element body1("body1");
    body1.set_style("color","0 255 255 255");
    body1.set_style("margin_color","0 0 0 255");
    body1.style.props["height"] = "50";  
    body1.style.props["margin_top"] = "100";
    body1.style.props["margin_bottom"] = "10";



    root.children.push_back(&body);
    root.children.push_back(&body1);



    printf("style tree:\n");
    StyleNode style_tree;
    style_tree.build_tree(&root);
    style_tree_print(style_tree);

    printf("\n");
    printf("layout tree:\n");
    LayoutNode layout_tree;
    layout_tree.build_tree(&style_tree);
    layout_tree_print(&layout_tree);

    layout_tree.lay_it_out();

    printf("\n");
    prop_tree_print(&layout_tree);

    init();

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

        

        // render
        SDL_SetRenderDrawColor(renderer, hex_to_color(0x000000));
        SDL_RenderClear(renderer);

        layout_tree_render(renderer,layout_tree);

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
