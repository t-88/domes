#ifndef RENDER_DISPLAY_BUFFER_HPP
#define RENDER_DISPLAY_BUFFER_HPP

#include <SDL2/SDL.h>
#include <typeinfo>
#include "../layout/layout_node.hpp"
#include "../layout/box.hpp"
#include "./r_node.hpp"
#include "./r_rect.hpp"
#include "./r_text.hpp"

class DisplayBuffer {
public:
    Box b_box;
    std::vector<RNode*> buffer;

    DisplayBuffer()   {}
    ~DisplayBuffer() {}


    void build_buffer(LayoutNode* lr) {
        if(!lr) return;

        std::string node_type = lr->style_node->dom_node->type; 
        if(node_type == "element") {
            buffer.push_back((RNode*) (new RRect(lr)));
        } else if(node_type == "text") {
            buffer.push_back((RNode*) (new RText(lr)));
        } else {
            printf("%s\n",node_type.c_str());
            assert(false && "Unreachable!!");
        }

        for (size_t i = 0; i < lr->nodes.size(); i++) {
            build_buffer(lr->nodes[i]);
        }
    }


    void render(SDL_Renderer* renderer) {
        for (size_t i = 0; i < buffer.size(); i++){
            buffer[i]->render(renderer);
        }
    }


};

#endif // RENDER_DISPLAY_BUFFER_HPP