#ifndef APP_CARD_HPP
#define APP_CARD_HPP


#include "../engine/dom/node.hpp"
#include "utils.hpp"

class Card : public Node
{
public:
    Card() : Node(NodeTypes::Elem) {
        this->type = "todo";
        this->id = "todo";

        setStyle(
            *this,
            "color: 255,255,0,255;"
            "margin_left : 25;" 
            "margin_top : 20;" 
            "height : 70;" 
            
        );
        this->style.props["width"] = std::to_string(Globals::width - 50);
    }

    ~Card(){

    }
    
    

};




#endif // APP_CARD_HPP