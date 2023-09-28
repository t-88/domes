#ifndef ENGINE_DOM_TEXT_H
#define ENGINE_DOM_TEXT_H

#include <vector>
#include <string>

#include "node.hpp"






class Text : public Node
{
public:
    std::string text;

    Text() : Node(NodeTypes::Text) { 
        id = "text"; 
    }
    Text(std::string t) : Node(NodeTypes::Text) {
        text = t;
        id = "text";
    }
    ~Text(){}
};










#endif // ENGINE_DOM_TEXT_H