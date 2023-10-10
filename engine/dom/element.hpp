#ifndef ENGINE_DOM_ELEMENT_H
#define ENGINE_DOM_ELEMENT_H

#include <vector>
#include <string>
#include <unordered_map>

#include "node.hpp"


typedef std::unordered_map<std::string,std::string> Attribs;

class Element : public Node
{
public:
    Attribs attribs;
    std::string tag_name;

    Element(std::string tn) : Node(NodeTypes::Elem) {
        tag_name = tn;
        id = tn;
        type = "element";
    }
    Element(Attribs a) : Node(NodeTypes::Elem) {
        attribs = a;
        id = "element";
    }
    

    ~Element() {}
};



#endif // ENGINE_DOM_ELEMENT_H
