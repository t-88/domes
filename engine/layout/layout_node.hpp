#ifndef ENGINE_LAYOUT_NODE_H
#define ENGINE_LAYOUT_NODE_H

#include <vector>
#include <assert.h>
#include "../style/style_node.hpp"
#include "../dom/node.hpp"
#include "./box.hpp"

const static int LAYOUT_TREE_PRINT_DEPTH_SPACING = 2;

#define UNINITIALIZED_DISPLAY "un-init-display"


class LayoutNode;
static void travese_layout_tree(LayoutNode* style_root);


class LayoutNode {
public:
    std::vector<LayoutNode*> nodes;
    LayoutNode* parent = nullptr;
    StyleNode* style_node;


    std::string display_box = UNINITIALIZED_DISPLAY;
    Box box;

    LayoutNode(){}
    LayoutNode(StyleNode* sn):style_node(sn) {
        display_box = sn->dom_node->style.props["display"];
    }
    
    
    ~LayoutNode() {}


    void print_element() {
        printf("%s\n",style_node->dom_node->id.c_str());
    }
    void append(LayoutNode* ln) {
        ln->parent = this;
        nodes.push_back(ln);
    }


    void build_tree(StyleNode* style_root) {
        style_node = style_root;
        display_box = style_root->dom_node->style.props["display"];
        travese_layout_tree(this);
    }

    void lay_it_out() {
        if(display_box == DisplayBlock || display_box == DisplayAnonymous) {
            block_layout();
        } else {
            assert(false && "not here yet use ur brain");
        }
    }


    void block_layout() {
        if(parent == nullptr) {
            box.rect.w = to_px(style_node->dom_node->style.props["width"]);
            box.rect.h = to_px(style_node->dom_node->style.props["height"]);

            // height can be the same as the children but the width depends on the total width of the screen 
            assert((box.rect.w != 0) && "root width is not given");
        }

        
        calc_width();
        calc_margin();
        lay_children();
        calc_position();
        calc_height();
    }


    void calc_width() {
        Box parent_box = box;
        if(parent != nullptr) { 
            parent_box = parent->box;
        } 

        auto style = style_node->dom_node->style;

        std::string width = "auto";
        if(style.props.count("width") != 0 && style.props["width"] != "auto") { width = style.props["width"]; } 

        std::string margin_left = "auto";
        if(style.props.count("margin_left") != 0 && style.props["margin_left"] != "auto") 
            { margin_left = style.props["margin_left"]; } 
        std::string margin_right = "auto";
        if(style.props.count("margin_right") != 0 && style.props["margin_right"] != "auto") 
            { margin_right = style.props["margin_right"]; } 


    
        int sum = to_px(margin_left) + to_px(margin_right) + to_px(width);

        if(width != "auto" && sum > parent_box.rect.w) {
            if(margin_left == "auto") margin_left = "0";
            if(margin_right == "auto") margin_right = "0";
        }

        int underflow = parent_box.rect.w - sum;

        int state  = (((width == "auto") ? 1 : 0) << 2) | 
                     (((margin_left == "auto") ? 1 : 0) << 1) | (((width == "auto") ? 1 : 0) << 0);

        if(state >= 0x004) {
            if(margin_left == "auto") margin_left = "0";
            if(margin_right == "auto") margin_right = "0";

            if(underflow >= 0) {
                width = std::to_string(to_px(width) + underflow);
            } else {
                width = "0";
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


        style_node->dom_node->style.props["width"] = width;
        style_node->dom_node->style.props["margin_left"] = margin_left;
        style_node->dom_node->style.props["margin_right"] = margin_right;
        box.rect.w = to_px(width);
    }

    void calc_margin() {
        auto style = style_node->dom_node->style;

        std::string width = "auto";
        if(style.props.count("width") != 0 && style.props["width"] != "auto") { width = style.props["width"]; } 

        std::string margin_top = "auto";
        if(style.props.count("margin_top") != 0 && style.props["margin_top"] != "auto") 
            { margin_top = style.props["margin_top"]; } 
        std::string margin_bottom = "auto";
        if(style.props.count("margin_bottom") != 0 && style.props["margin_bottom"] != "auto") 
            { margin_bottom = style.props["margin_bottom"]; } 


        box.margin.top =  to_px(margin_top);
        box.margin.bottom = to_px(margin_bottom);
    }

    void calc_position() { 




        Box containg_box = parent ? parent->box : Box();
        box.rect.x = containg_box.rect.x + box.margin.left + box.padding.left; 
        box.rect.y = containg_box.rect.h + containg_box.rect.y + box.margin.top + box.padding.top; 
    }

    void lay_children() {
        box.rect.h = box.get_margin_rect().h;
        for (size_t i = 0; i < nodes.size(); i++){
            nodes[i]->lay_it_out();
            box.rect.h += nodes[i]->box.get_margin_rect().h;

            nodes[i]->print_element();
            nodes[i]->box.get_margin_rect().print();
            nodes[i]->box.margin.print();
            printf("----\n");
        }
        
    }

    void calc_height() {
        auto style = style_node->dom_node->style;
        if(style.props.count("height") != 0 && style.props["height"] != "auto") { 
            box.rect.h = to_px(style.props["height"]); 
        } 
    }


};



static void travese_layout_tree(LayoutNode* root) { 
    auto style_root = root->style_node;


    LayoutNode* annoymous_layout;

    bool found_prev_inline = false;
    for (size_t i = 0; i < style_root->nodes.size(); i++) {


        LayoutNode* lnp = new LayoutNode(&style_root->nodes[i]);

        if(style_root->nodes[i].dom_node->style.props["display"] == DisplayInline) {
            if(!found_prev_inline) {
                found_prev_inline = true;
                annoymous_layout = new LayoutNode(&style_root->nodes[i]);
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



void layout_tree_print(LayoutNode* layout_root,int depth = 0) { 
    if(depth == 0) {
        printf("%*s%s\n",depth * STYLE_TREE_PRINT_DEPTH_SPACING,"",layout_root->display_box.c_str());
        depth++;
    }
    
    for (size_t i = 0; i < layout_root->nodes.size(); i++) {
        printf("%*s%s\n",depth * STYLE_TREE_PRINT_DEPTH_SPACING,"",layout_root->nodes[i]->display_box.c_str());
        layout_tree_print(layout_root->nodes[i], depth + 1);
    }
}








#endif //ENGINE_LAYOUT_NODE_H