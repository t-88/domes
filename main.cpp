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
    if(depth == 0) {
        // layout_root->style_node->dom_node->style.print();
        printf("%*s%s\n",depth * 4,"",layout_root->style_node->dom_node->style.props["width"].c_str());
        depth++;
    }
    
    for (size_t i = 0; i < layout_root->nodes.size(); i++) {
        printf("%*s%s\n",depth * 4,"",layout_root->style_node->dom_node->style.props["width"].c_str());
        // layout_root->style_node->dom_node->style.print();
        prop_tree_print(layout_root->nodes[i], depth + 1);
    }
}


int main() {
    Globals::set_window_sizing(600,400);


    Element root("app");
    root.style.props["width"] =  std::to_string(Globals::width);
    root.style.props["height"] = std::to_string(Globals::height);

    // root.style.props["margin"] = "10 10 10 10";
    // root.style.props["padding"] = "10 10 10 10";


    Element body("body");
    
    Text t1("Hello Mada fucka");
    t1.style.props["display"] = DisplayBlock;
    Text t2("Hello Mada fucka");
    t2.style.props["display"] = DisplayBlock;


    Element body1("body1");



    body.children.push_back(&t1);
    root.children.push_back(&body);
    root.children.push_back(&body1);

    StyleNode style_tree;
    style_tree.build_tree(&root);
    style_tree_print(style_tree);

    printf("\n");

    LayoutNode layout;
    layout.build_tree(&style_tree);

    printf("\n");
    layout_tree_print(&layout);  

    layout.lay_it_out();  
  
    return 1;
    init();
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

        

        // render
        SDL_SetRenderDrawColor(renderer, hex_to_color(0x000000));
        SDL_RenderClear(renderer);


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
