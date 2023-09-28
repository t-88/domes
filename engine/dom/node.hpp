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


    Node(){}
    Node(NodeType t) : node_type(t) { }
    ~Node(){}
};








#endif // ENGINE_DOM_NODE_H