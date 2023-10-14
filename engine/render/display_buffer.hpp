#ifndef RENDER_DISPLAY_BUFFER_HPP
#define RENDER_DISPLAY_BUFFER_HPP

#include <SDL2/SDL.h>
#include <typeinfo>
#include "../layout/layout_node.hpp"
#include "../layout/box.hpp"
#include "./r_node.hpp"
#include "./r_rect.hpp"
#include "./r_text.hpp"
#include "./r_container.hpp"

class DisplayBuffer {
public:
    Box b_box;
    std::vector<RNode*> buffer;

    DisplayBuffer()   {}
    ~DisplayBuffer() {}


    void build_buffer(LayoutNode* lr,Rect* rect = nullptr,int offset_y = 0) {
        if(!lr) return;
        if(lr->display_box == DisplayAnonymous) {
            for (size_t i = 0; i < lr->nodes.size(); i++) {
                build_buffer(lr->nodes[i],rect,offset_y);
            }                
            return;
        };

        

        std::string node_type = lr->node->type; 
    
        if(node_type == "text") {
            buffer.push_back((RNode*) (new RText(lr,rect,offset_y)));
        } 
        
        else if (node_type == "column") {
            buffer.push_back((RNode*) (new RRect(lr,rect,offset_y)));

            for (size_t i = 0; i < lr->nodes.size(); i++) {
                build_buffer(lr->nodes[i],&lr->box.rect,lr->node->offset_y);
            }                
        } 
        
        else {
            buffer.push_back((RNode*) (new RRect(lr,rect,offset_y)));
            for (size_t i = 0; i < lr->nodes.size(); i++) {
                build_buffer(lr->nodes[i],rect,offset_y);
            }                
        }


    }




    void render(SDL_Renderer* renderer) {
        for (size_t i = 0; i < buffer.size(); i++){
            buffer[i]->render(renderer);
        }
    }


};

#endif // RENDER_DISPLAY_BUFFER_HPP