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
    Element finish_btn;
    Text todo_text;
    int uid;

    Card() : Card(0){}

    Card(int _uid) : Node(NodeTypes::Elem) {
        children.clear();

        type = "todo";
        id = "todo";
        uid = _uid;
        printf("    %d\n",uid);


        setStyle(
            *this,
            "color: 125,50,0,255;"
            "margin_left : 25;" 
            "margin_top : 20;" 
            "height : 70;" 
        );
        style->props["width"] = std::to_string(Globals::width - 50);


        delete_btn = Element("delete-btn");
        setStyle(
            delete_btn,
            "color: 255,0,0,255;"
            "margin_left : 10;"
            "margin_top : 10;"
            "height : 20;" 
            "width : 20;"
        );
        children.push_back(&delete_btn);

        delete_btn.userdata = &uid;
        delete_btn.onClickCallback = onRemoveTodoCallback;



        finish_btn =  Element("finish-btn");
        setStyle(
            finish_btn,
            "display:" DisplayInline ";"
            "color: 0,255,0,255;"
            "margin_left : 10;"
            "margin_top : 10;"
            "height : 20;" 
            "width : 20;"
        );
        children.push_back(&finish_btn);
        finish_btn.userdata = &uid;
        finish_btn.onClickCallback = onFinishTodoCallback;


        sprintf(buffer,"todo number %d",uid);

        todo_text = Text(buffer);
        setStyle(
            todo_text,
            "display:" DisplayInline ";"
        );
        children.push_back(&todo_text);
    }   

    ~Card(){
    }


};




#endif // APP_CARD_HPP