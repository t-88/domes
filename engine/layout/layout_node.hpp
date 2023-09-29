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
            assert((box.rect.w == 0 || box.rect.h == 0) && "sizing for the root node is not set");
        }
        calc_width();
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

        std::string padding_left = "auto";
        if(style.props.count("padding_left") != 0 && style.props["padding_left"] != "auto") 
            { padding_left = style.props["padding_left"]; } 
        std::string padding_right = "auto";
        if(style.props.count("padding_right") != 0 && style.props["padding_right"] != "auto") 
            { padding_right = style.props["padding_right"]; } 

    
    
        int sum = to_px(margin_left) + to_px(margin_right) +
                  to_px(padding_left) + to_px(padding_right) +
                  to_px(width);

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
                    printf("->  0x%X\n",state);
                    assert(false && "no way it hits here");
                break;
            } 
        }


        style_node->dom_node->style.props["width"] = width;
        style_node->dom_node->style.props["margin_left"] = margin_left;
        style_node->dom_node->style.props["margin_right"] = margin_right;
        style_node->dom_node->style.props["padding_left"] = padding_left;
        style_node->dom_node->style.props["padding_right"] = padding_right;
        box.rect.w = to_px(width);


        for (size_t i = 0; i < nodes.size(); i++) {
            nodes[i]->lay_it_out();
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