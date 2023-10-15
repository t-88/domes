#ifndef ENGINE_DOM_INPU_H
#define ENGINE_DOM_INPU_H

#include <vector>
#include <string>

#include "node.hpp"


#include "../dom/element.hpp"
#include "../utils.hpp"

class Input : public Node
{
public:
    bool selected = false;
    Element rect;
    Text text_element;

    bool blink_visible = false;
    float blink_timer = 0;
    float blink_delay = 0.4f;

public:
    Input() : Node(NodeTypes::Elem) {
        type = "input";
        id = "input";
    }
    Input(std::string id) :  Input() {
        children.clear();


        setStyle(
            *this,
            "color: 0,0,0,0;"
            "border_color : 255,255,255,255;"
            "height : 35;" 
        );
        style->props["width"] = std::to_string(Globals::width - 100);

     



        rect = Element("element");
        setStyle(
            rect,
            "display :" DisplayInline ";"
            "width :  3;"
            "height : 25;"
            "margin_top : 5;"
            "margin_left : 5;"
        );
        


        text_element = Text("");
        setStyle(
            text_element,
            "display :" DisplayInline ";"
        );


        this->push(&text_element);
        this->push(&rect);


        auto onSelectTextInput = [&](void* userdata) { selected = true; };
        this->onClickCallbackLambda = onSelectTextInput;
        
        auto onUnSelectTextInput = [&](void* userdata) { selected = false;};        
        this->onClickOutCallbackLambda = onUnSelectTextInput;
        
        auto onPressKey = [&](Event::Event event) {
            if(selected) {
                if(event.text) {
                    // if event is text
                    text_element.text.push_back(event.text[0]);
                    text_element.update_text(text_element.text);
                } else {
                    if(event.key == 42) {
                        // remove last char, backspace
                        if(!text_element.text.size()) return;
                        text_element.text.pop_back();
                        text_element.update_text(text_element.text);
                    }
                }
            }
        };   
        this->onKeyPressCallbackLambda = onPressKey; 


    }
    ~Input() {}


    void update(float dt = -1) {
        assert(dt != -1);


        if(selected) {
            blink_timer += dt;
            if(blink_timer > blink_delay) {
                blink_timer = 0;
                blink_visible = !blink_visible;
            }
            if(blink_visible) {
                setStyle(
                    rect,
                    "color : 255,255,255,255;"
                );
            } else if(!blink_visible) {
                setStyle(
                    rect,
                    "color : 0,0,0,0;"
                );

            }
        } else {
            setStyle(
                rect,
                "color : 255,255,255,255;"
            );            
        }
    }



};



#endif // ENGINE_DOM_INPU_H
