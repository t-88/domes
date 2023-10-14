#ifndef ENGINE_DOM_NODE_H
#define ENGINE_DOM_NODE_H

#include <vector>
#include "../style/style.hpp"
#include "../events/event.hpp"

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
    void (*onScrollCallback)(Event::Event event) = nullptr;


    bool scrollable = false;
    bool overflow = true;

    Node(){
        style = new Style();
    }
    Node(NodeType t) : node_type(t) { 
        style = new Style();
    }
    ~Node(){
    }

    void set_style(std::string ident,std::string value) {
        style->props[ident] = value;
    }

    void onClick() {
        if(!onClickCallback) return;
        onClickCallback(userdata);
    }
    void onScroll(Event::Event event) {
        if(!onScrollCallback) return;

        onScrollCallback(event);
    }


    void push(Node* node) {
        children.push_back(node);
    }

};








#endif // ENGINE_DOM_NODE_H