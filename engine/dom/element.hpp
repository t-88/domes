#ifndef ENGINE_DOM_ELEMENT_H
#define ENGINE_DOM_ELEMENT_H

#include <vector>
#include <string>
#include <unordered_map>

#include "node.hpp"


class Element : public Node
{
public:
    std::string tag_name;

    Element() { 
    }


    Element(std::string tn) : Node(NodeTypes::Elem) {
        tag_name = tn;
        id = tn;
        type = "element";
    }
    

    ~Element() {}
};



#endif // ENGINE_DOM_ELEMENT_H
