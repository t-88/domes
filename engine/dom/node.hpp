#ifndef ENGINE_DOM_NODE_H
#define ENGINE_DOM_NODE_H

#include <vector>
#include "../style/style.hpp"
#include "../events/event.hpp"
#include "../utils.hpp"
#include "functional"

namespace NodeTypes {
    const int Text = 0;
    const int Elem = 0;
} 
typedef int NodeType ;



class Node
{

public:
    std::vector<Node*> children = {};
    NodeType node_type;
    Style* style;

    std::string id = "node";
    std::string type = "node";

    int offset_x = 0;
    int offset_y = 0;

    void* userdata = nullptr;
    
    void (*onClickCallback)(void* userdata) = nullptr;
    std::function<void(void* userdata)> onClickCallbackLambda;

    void (*onClickOutCallback)(void* userdata) = nullptr;
    std::function<void(void* userdata)> onClickOutCallbackLambda;


    void (*onKeyPressCallback)(Event::Event event) = nullptr;
    std::function<void(Event::Event event)> onKeyPressCallbackLambda;

    void (*onScrollCallback)(Event::Event event) = nullptr;
    std::function<void(Event::Event event)> onScrollCallbackLambda;


    bool scrollable = false;
    bool overflow = true;

    Node(){
        style = new Style();
        onClickCallbackLambda = [](void* userdata){return;};
        onKeyPressCallbackLambda = [](Event::Event event){return;};
        onScrollCallbackLambda = [](Event::Event event){return;};
    }
    Node(NodeType t) : Node()  { 
        node_type=  t;
    }
    ~Node(){
    }

    void set_style(std::string ident,std::string value) {
        style->props[ident] = value;
    }

    void onClick() {
        // called when clicked inside element
        if(onClickCallback) {
            onClickCallback(userdata);
        }
        if(onClickCallbackLambda) {
            onClickCallbackLambda(userdata);
        }
    }
    void onClickOut() {
        // called when clicked outside element
        if(onClickOutCallback) {
            onClickOutCallback(userdata);
        }
        if(onClickOutCallbackLambda) {
            onClickOutCallbackLambda(userdata);
        }
    }    

    void onKeyPress(Event::Event event) {
        if(onKeyPressCallback) {
            onKeyPressCallback(event);
        }
        onKeyPressCallbackLambda(event);
    }

    void onScroll(Event::Event event) {
        if(onScrollCallback) onScrollCallback(event);
        onScrollCallbackLambda(event);

    }



    void push(Node* node) {
        children.push_back(node);
    }

};








#endif // ENGINE_DOM_NODE_H