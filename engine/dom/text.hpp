#ifndef ENGINE_DOM_TEXT_H
#define ENGINE_DOM_TEXT_H

#include <vector>
#include <string>

#include "../../globals.h"
#include "node.hpp"




class Text : public Node
{
public:
    std::string text;

    Text() : Node(NodeTypes::Text) { 
        id = "text"; 
        type = "text";
    }

    Text(std::string t) : Node(NodeTypes::Text) {
        update_text(t);
    }


    void update_text(std::string new_text) {
        text = new_text;
        id = "text";
        type = "text";

        char out_height[128];
        sprintf(out_height,"%d",Globals::font_height);
        style->props["height"] = out_height;

        char out_width[128];
        sprintf(out_width,"%ld",Globals::font_width * new_text.size());
        style->props["width"] = out_width;
    } 
    ~Text(){}
};










#endif // ENGINE_DOM_TEXT_H