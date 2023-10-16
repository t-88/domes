#ifndef TODO_APP_HPP
#define TODO_APP_HPP

#include <SDL2/SDL.h>


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
    std::vector<std::string> todos;

    int idx = 0; 


    

    DisplayBuffer buffer;
    LayoutNode layout_tree;
public:
    TodoApp();
    ~TodoApp();

    void update(float dt);
    void render();

    void fire_event(Event::Event event);
    void add_todo(std::string todo = "",bool loading = false);

    void save_todos();
    void load_todos();
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
        "margin_bottom: 15;"
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
    text_input->selected = true;
    text_input->max_chars = 32;
    setStyle(
        *text_input,
        "display : "  DisplayInline ";"
        "margin_left: 45;"
    );


    root.push(&logo_text);
    root.push(&date_text);
    root.push(&time_text);
    root.push(&todo_container);
    root.push(text_input);




    todo_container.onScrollCallbackLambda = [&](Event::Event event) { 
        todo_container.scroll_y(event.scroll_dir_y * 25);
    };
    add_btn.onClickCallbackLambda = [&](void* userdata) { add_todo(); };
    text_input->onSubmitCallbackLambda = [&]() { add_todo();};

    layout_tree = LayoutNode(&root);
    layout_tree.is_root = true;
}

TodoApp::~TodoApp() {}



void TodoApp::add_todo(std::string todo,bool loading) {
    if(text_input->text_element.text != "") { todo = text_input->text_element.text;}
    if(todo == "") return;


    if(!loading)
        todos.push_back(todo);
    

    auto card = new Card(idx,todo);
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

            todos.erase(todos.begin() + i);
            todo_container.children.erase(todo_container.children.begin() + i);            
        }; 

        todo_container.push(card);
        idx++;
}
void TodoApp::fire_event(Event::Event event) {
    layout_tree.fire_event(event);
}

void TodoApp::save_todos() {

    FILE* f;
    f = fopen("todos.db","w"); 
    for (size_t i = 0; i < todos.size(); i++) {
        fwrite(todos[i].c_str(),1,todos[i].size(),f);
        fwrite("\n",1,1,f);
    }
    fclose(f);
}
void TodoApp::load_todos() {
    FILE* f;
    f = fopen("todos.db","r");
    if(!f) { return; } // file dosent exist
    

    fseek(f,0,SEEK_END);
    int fsize = ftell(f);
    fseek(f,0,SEEK_SET);

    
    if(!fsize) return; // file is empty

    todos.clear();

    std::string todo;

    char c;
    do
    {
        c =  fgetc(f);
        if(c == EOF) break;
        if(c == '\n') {
            todos.push_back(todo);
            todo.clear();
        } else {
            todo.push_back(c);
        }
    } while (c != EOF);
    
    assert(feof(f) != EOF && "something gone wrong with ur file");
    fclose(f);

    for (size_t i = 0; i < todos.size(); i++) {
        add_todo(todos[i],true);
    }
    
    
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