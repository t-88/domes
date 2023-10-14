#ifndef ENGINE_LAYOUT_NODE_H
#define ENGINE_LAYOUT_NODE_H

#include <vector>
#include <assert.h>
#include "../dom/node.hpp"
#include "./box.hpp"

#include "../events/event.hpp"

const static int LAYOUT_TREE_PRINT_DEPTH_SPACING = 2;
#define UNINITIALIZED_DISPLAY "un-init-display"


class LayoutNode;
static void travese_layout_tree(LayoutNode* root);



class LayoutNode {
public:
    std::vector<LayoutNode*> nodes;
    LayoutNode* parent = nullptr;
    Node* node = nullptr;

    std::string display_box = DisplayBlock;
    Box box;


    LayoutNode(){}
    LayoutNode(Node* _node) {
        display_box = _node->style->props["display"];
        node = _node;
    }


    void fire_event(Event::Event event) {
        switch (event.type)
        {
            case Event::MouseClickEvent: 
                switch (event.type_state)
                {
                    case Event::MouseDown: 
                        onClick(event);
                    break;
                }
            break;
            case Event::MouseScrollEvent:
                onScroll(event);
            break;

            default:
                break;
        }

    }


    void traverse_onClick(Event::Event event,int offset = 0) {
        for (size_t i = 0; i < nodes.size(); i++) {
            nodes[i]->onClick(event,offset);
        }
    }
    void onClick(Event::Event event,int offset_y = 0) {
        if(box.collide(event.x ,event.y - offset_y)) {
            node->onClick();
            traverse_onClick(event,node->offset_y + offset_y);
        }
    }


    void traverse_onScroll(Event::Event event) {

    }
    void onScroll(Event::Event event) {
        if(box.collide(event.x,event.y)) {
            if(node->scrollable) {
                node->onScroll(event);
            }
            for (size_t i = 0; i < nodes.size(); i++) {
                nodes[i]->onScroll(event);
            }            
        }

    } 
    

    void append(LayoutNode* ln) {
        ln->parent = this;
        nodes.push_back(ln);
    }
    void build_tree() {
        assert(node && "layout node is not given\n");
        travese_layout_tree(this);
    }

    void lay_it_out() {
        if(parent == nullptr) {
            box.rect.w = to_px(node->style->props["width"]);
            box.rect.h = to_px(node->style->props["height"]);
            // height can be the same as the children but the width depends on the total width of the screen 
            assert((box.rect.w != 0) && "root width is not given");
        }
        if(display_box == DisplayBlock) {
            block_layout();
        } else if(display_box == DisplayAnonymous){
            anonymous_layout();
        } else {
            inline_layout();
        }

    }

    void calc_vertical_margins() {
        Style style = *node->style;


        std::string margin_bottom = AUTO;
        std::string margin_top = AUTO;


        if(style.props.count("margin_top") != 0 && style.props["margin_top"] != AUTO) 
            { margin_top = style.props["margin_top"]; } 
        if(style.props.count("margin_bottom") != 0 && style.props["margin_bottom"] != AUTO) 
            { margin_bottom = style.props["margin_bottom"]; } 


        box.margin.top =  to_px(margin_top);
        box.margin.bottom = to_px(margin_bottom);
    }
    void calc_horizontal_margins() {
        Style style = *node->style;

        std::string width = AUTO;
        std::string margin_left = AUTO;
        std::string margin_right = AUTO;


        if(style.props.count("margin_left") != 0 && style.props["margin_left"] != "auto") 
            { margin_left = style.props["margin_left"]; } 
        if(style.props.count("margin_right") != 0 && style.props["margin_right"] != "auto") 
            { margin_right = style.props["margin_right"]; } 


        box.margin.left =  to_px(margin_left);
        box.margin.right = to_px(margin_right);
    }


    // block layout
    void block_layout() {
        block_calc_width();
        calc_vertical_margins();
        calc_horizontal_margins();
        block_calc_position();
        block_lay_children();
        block_calc_height();
    }
    void block_calc_width() {
        Style style = *node->style;

        std::string width = AUTO;
        std::string margin_left = AUTO;
        std::string margin_right = AUTO;

        if(style.props.count("width") && style.props["width"] != AUTO) 
            width = style.props["width"]; 
        if(style.props.count("margin_left") && style.props["margin_left"] != AUTO) 
            margin_left = style.props["margin_left"]; 
        if(style.props.count("margin_right") && style.props["margin_right"] != AUTO) 
            margin_right = style.props["margin_right"]; 
        
        



        int sum = to_px(margin_left) + to_px(margin_right) + to_px(width);

        Box parent_box = box;
        if(parent != nullptr) 
            parent_box = parent->box; 

        if(width != AUTO && sum > parent_box.rect.w) {
            if(margin_left == AUTO) margin_left = "0";
            if(margin_right == AUTO) margin_right = "0";
        }

        int underflow = parent_box.rect.w - sum;

        int state  = (((width == AUTO) ? 1 : 0) << 2) | 
                     (((margin_left == AUTO) ? 1 : 0) << 1) | (((width == AUTO) ? 1 : 0) << 0);

        if(state >= 0x004) {
            if(margin_left == AUTO) margin_left = ZERO;
            if(margin_right == AUTO) margin_right = ZERO;

            if(underflow >= 0) {
                width = std::to_string(underflow);
            } else {
                width = ZERO;
                margin_right = std::to_string(to_px(margin_right) + underflow);
            }
        } else {
            switch (state) {
                case 0x0:
                    margin_right = std::to_string(to_px(margin_right) + underflow);
                break;
                case 0x1:
                    margin_right = std::to_string(underflow);
                break;
                case 0x2:
                    margin_left = std::to_string(underflow);
                break;
                case 0x3:
                    margin_right = std::to_string((int) underflow / 2);
                    margin_left = std::to_string((int) underflow  / 2);
                break;
                default:
                    assert(false && "no way it hits here");
                break;
            } 
        }


        node->style->props["width"] = width;
        node->style->props["margin_left"] = margin_left;
        node->style->props["margin_right"] = margin_right;
        box.rect.w = to_px(width);
    }
    void block_calc_position() { 
        Box containg_box = parent ? parent->box : Box();


        box.rect.x = containg_box.rect.x + box.margin.left; 
        box.rect.y = containg_box.rect.y + containg_box.rect.h + box.margin.top;

    }
    void block_lay_children() {
        box.rect.h = 0;
        for (size_t i = 0; i < nodes.size(); i++){
            nodes[i]->lay_it_out();
            box.rect.h += nodes[i]->box.get_margin_rect().h;
        }
        if(node->type == "column") {
            ((Column*)node)->children_h = box.rect.h - nodes[nodes.size() - 1]->box.get_margin_rect().h;
        }

    }
    void block_calc_height() {
        Style style = *node->style;


        if(style.props.count("height") != 0 && style.props["height"] != "auto") { 
            box.rect.h = to_px(style.props["height"]); 
        
        
            if(node->type == "column") {
                ((Column*)node)->children_h -= box.rect.h - box.margin.bottom;
            }

        } 
    } 

    // anonymous layout
    void anonymous_layout() {
        anonymous_calc_width();
        block_calc_position();

        anonymous_lay_children();
    }
    void anonymous_calc_width() {
        if(parent) {
            node->style->props["width"] = std::to_string(parent->box.rect.w);
        } else {
            node->style->props["width"] = Globals::width;
        }

        node->style->props["margin_left"] = ZERO;
        node->style->props["margin_right"] = ZERO;
        box.rect.w = to_px(node->style->props["width"]);
    }
    void anonymous_lay_children() {
        int max_height = -1;

        for (size_t i = 0; i < nodes.size(); i++) {
            nodes[i]->lay_it_out();
            box.rect.x += nodes[i]->box.get_margin_rect().w; 

            if(nodes[i]->box.get_margin_rect().h > max_height) {
                max_height = nodes[i]->box.get_margin_rect().h;
            } 
        }
        box.rect.x = 0;
        box.rect.h = max_height;
    }


    // inline layout
    void inline_layout() {
        inline_calc_width();
        calc_vertical_margins();
        calc_horizontal_margins();
        block_lay_children();
        block_calc_position();
        block_calc_height();
    }

    void inline_calc_width() {
        Style style = *node->style;
        std::string width = AUTO;
        std::string margin_left =  ZERO;
        std::string margin_right = ZERO;

        if(style.props.count("width") != 0 && style.props["width"] != AUTO)
            width = style.props["width"];
        if(style.props.count("margin_left") != 0 && style.props["margin_left"] != AUTO)
            margin_left = style.props["margin_left"];
        if(style.props.count("margin_right") != 0 && style.props["margin_right"] != AUTO)
            margin_right = style.props["margin_right"];


        Box parent_box = box;
        if(parent)  parent_box = parent->box;

        int sum = to_px(width) + to_px(margin_left) + to_px(margin_right);
        int underflow =  (parent_box.rect.w - sum) > 0;

        if(!underflow  && width != AUTO) {
            margin_left = ZERO;
            margin_right = ZERO;
        }

        if(width == AUTO) {
            if(!underflow) {
                width = ZERO;
                margin_right = std::to_string(underflow);
            }
        }
    
        node->style->props["width"] = width;
        node->style->props["margin_left"] = margin_left;
        node->style->props["margin_right"] = margin_right;


        box.rect.w = to_px(width);
    }
};



static void travese_layout_tree(LayoutNode* root) { 
    auto node = root->node;


    LayoutNode* annoymous_layout;
    

    bool found_prev_inline = false;
    for (size_t i = 0; i < node->children.size(); i++) {
        LayoutNode* lnp = new LayoutNode(node->children[i]);

        if(node->children[i]->style->props["display"] == DisplayInline) {

            if(!found_prev_inline) {
                found_prev_inline = true;

                auto element = new Element("hidden-container");

                annoymous_layout = new LayoutNode(element);
                annoymous_layout->display_box = DisplayAnonymous;
            }

            annoymous_layout->append(lnp);
            continue;
        }
        
        if(found_prev_inline) {
            found_prev_inline = false;
            root->append(annoymous_layout);
        }


        root->append(lnp);
    }
    
    if(found_prev_inline) {
        root->append(annoymous_layout);
    }


    for (size_t i = 0; i < root->nodes.size(); i++) {
        travese_layout_tree(root->nodes[i]);
    }
}






#endif //ENGINE_LAYOUT_NODE_H