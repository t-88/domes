#ifndef ENGINE_DOM_COLUMN_H
#define ENGINE_DOM_COLUMN_H

#include <vector>
#include <string>

#include "node.hpp"


class Column : public Node
{
public:
    int children_h = 0;
public:
    Column() : Node(NodeTypes::Elem) {
        scrollable = true;
        overflow = false;

        type = "column";
        id = "column";
        
    }
    Column(std::string id) :  Column() {
        id = id;

    }
    ~Column() {}


    void scroll_y(int y) {
        if(offset_y + y > 0) { return; }
        if(-y - offset_y  > children_h) { return; }


        offset_y += y;
    }

};



#endif // ENGINE_DOM_COLUMN_H
