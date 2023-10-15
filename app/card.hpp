#ifndef APP_CARD_HPP
#define APP_CARD_HPP


#include "../engine/dom/node.hpp"
#include "utils.hpp"
#include "state.hpp"


static char buffer[256];
class Card : public Node
{
public:
    Element delete_btn;
    Text todo_text_element;
    int uid;

    Card() : Card(0,""){}

    Card(int _uid,std::string todo_text) : Node(NodeTypes::Elem) {
        children.clear();

        type = "todo";
        id = "todo";
        uid = _uid;

        setStyle(
            *this,
            "color: 0,0,0,0;"
            "border_color : 255,255,255,255;"
            "margin_left : 25;" 
            "margin_top : 20;" 
            "height : 70;" 
        );
        style->props["width"] = std::to_string(Globals::width - 50);


        delete_btn = Element("delete-btn");
        setStyle(
            delete_btn,
            "color: 255,0,0,255;"
            "display : " DisplayInline ";"
            "margin_top : 20;"
            "margin_left : 10;"
            "margin_right : 10;"
            "height : 30;" 
            "width : 30;"
        );

        delete_btn.userdata = &uid;






        todo_text_element = Text(todo_text.c_str());
        setStyle(
            todo_text_element,
            "display:" DisplayInline ";"
            "margin_top : 15;" 

        );

        this->push(&delete_btn);
        this->push(&todo_text_element);
    }   

    ~Card(){
    }


};




#endif // APP_CARD_HPP