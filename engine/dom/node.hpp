#ifndef ENGINE_DOM_NODE_H
#define ENGINE_DOM_NODE_H

#include <vector>
#include "../style/style.hpp"

namespace NodeTypes {
    const int Text = 0;
    const int Elem = 0;
} 
typedef int NodeType ;



class Node
{

public:
    std::vector<Node*> children;
    NodeType node_type;
    Style style;

    std::string id = "node";
    std::string type = "node";


    void (*onClickCallback)() = nullptr;


    Node(){}
    Node(NodeType t) : node_type(t) { }
    ~Node(){}

    void set_style(std::string ident,std::string value) {
        style.props[ident] = value;
    }

    void onClick() {
        if(!onClickCallback) return;
        onClickCallback();
    }
};








#endif // ENGINE_DOM_NODE_H