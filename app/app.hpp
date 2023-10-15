#ifndef TODO_APP_HPP
#define TODO_APP_HPP


#include "../engine/dom/node.hpp"
#include "../engine/dom/text.hpp"
#include "../engine/dom/element.hpp"
#include "../engine/dom/column.hpp"
#include "../engine/dom/input.hpp"

#include "../engine/style/style_node.hpp"
#include "../engine/layout/layout_node.hpp"
#include "../engine/render/display_buffer.hpp"
#include "../engine/events/event.hpp"
#include "card.hpp"


#include "../engine/render/display_buffer.hpp"


#include "utils.hpp"
#include <SDL2/SDL.h>



static int idx = 0; 



class TodoApp
{
private:
    Element root;
    Element add_btn;
    Text    logo_text;
    Text    date_text;
    Text time_text;

    Input* text_input;

    Column todo_container;

    

    DisplayBuffer buffer;
    LayoutNode layout_tree;
public:
    TodoApp();
    ~TodoApp();

    void update(float dt);
    void render();

    void fire_event(Event::Event event);
};

TodoApp::TodoApp() {
    root = Element("app");
    root.style->props["width"] = std::to_string(Globals::width).c_str();
    setStyle(
        root,
        "color : 0,0,0,255;"
        "margin_top : 0;"
        "margin_bottom : 0;"
    );

    logo_text = Text("DoMe");
    setStyle(
        logo_text,
        "display : " DisplayInline ";"
        "color : 255,0,0,255;"
        "margin_left :   10;"
        "margin_right :  230;"
        "margin_bottom : 20;"
    ); 


    date_text = Text(get_date_str());
    setStyle(
        date_text,
        "display:" DisplayInline ";"
        "color: 255,0,0,255;"   
        "margin_right: 20;"   
    );

    time_text = Text(get_time_str());
    setStyle(
        time_text,
        "display: " DisplayInline ";"
        "color: 255,0,0,255"
    );


    setStyle(
        todo_container,
        "color : 0,255,0,0;"
        "height : 280;"
        "margin_bottom: 20;"
    );

    add_btn = Element("add-btn");
    setStyle(
        add_btn,
        "width :  30;"
        "height : 30;"
        "color : 255,0,0,255;"
        "display : "  DisplayInline ";"
    );





    text_input = new Input("text-input");
    text_input->max_chars = 32;
    setStyle(
        *text_input,
        "display : "  DisplayInline ";"
        "margin_left: 25;"
        "margin_right: 25;"
    );


    root.push(&logo_text);
    root.push(&date_text);
    root.push(&time_text);
    root.push(&todo_container);
    root.push(text_input);
    root.push(&add_btn);




    todo_container.onScrollCallbackLambda = [&](Event::Event event) { 
        todo_container.scroll_y(event.scroll_dir_y * 25);
    };

    add_btn.onClickCallbackLambda = [&](void* userdata) {
        auto card = new Card(idx,text_input->text_element.text);
        text_input->text_element.update_text("");

        // create todo
        card->delete_btn.onClickCallbackLambda = [&](void* userdata) {
            int idx = userdata_to(int,userdata);
            int i = 0;
            for (i = 0; i < todo_container.children.size(); i++) {
                if(((Card*)todo_container.children[i])->uid == idx) {
                    break;
                }
            }
            delete todo_container.children[i];
            todo_container.children.erase(todo_container.children.begin() + i);            
        }; 

        // remove todo
        todo_container.push(card);
        idx++;
    };

    layout_tree = LayoutNode(&root);
    layout_tree.is_root = true;
}

TodoApp::~TodoApp() {
}

void TodoApp::fire_event(Event::Event event) {
    layout_tree.fire_event(event);
}


void TodoApp::update(float dt = -1) {
    time_text.text = get_time_str();
    text_input->update(dt);


    layout_tree.clear();
    layout_tree.build_tree();
    layout_tree.lay_it_out();


    buffer.build_buffer(&layout_tree);

    
}

void TodoApp::render() {
    buffer.render(Globals::renderer);
}





#endif // TODO_APP_HPP