#ifndef ENGINE_LAYOUT_NODE_H
#define ENGINE_LAYOUT_NODE_H

#include <vector>

#include "../style/style_node.hpp"
#include "../dom/node.hpp"


const static int LAYOUT_TREE_PRINT_DEPTH_SPACING = 2;

#define UNINITIALIZED_DISPLAY "un-init-display"


class LayoutNode;
static void travese_layout_tree(LayoutNode* style_root);


class LayoutNode {
public:
    std::vector<LayoutNode> nodes;
    StyleNode* style_node;
    std::string display_box = UNINITIALIZED_DISPLAY;

    LayoutNode(){

    }
    LayoutNode(StyleNode* sn):style_node(sn) {
        display_box = sn->dom_node->style.props["display"];
    }
    
    ~LayoutNode() {}


    void build_tree(StyleNode* style_root) {
        style_node = style_root;
        display_box = style_root->dom_node->style.props["display"];
        travese_layout_tree(this);
    }


};



static void travese_layout_tree(LayoutNode* root) { 
    auto style_root = root->style_node;
    

    LayoutNode annoymous_layout;

    bool found_prev_inline = false;
    for (size_t i = 0; i < style_root->nodes.size(); i++) {
        if(style_root->nodes[i].dom_node->style.props["display"] == DisplayInline) {

            if(!found_prev_inline) {
                found_prev_inline = true;
                annoymous_layout = LayoutNode();
                annoymous_layout.display_box = DisplayAnonymous;
            }


            annoymous_layout.nodes.push_back(LayoutNode(&style_root->nodes[i]));
            continue;
        }
        
        if(found_prev_inline) {
            found_prev_inline = false;
            root->nodes.push_back(annoymous_layout);
        }
        root->nodes.push_back(LayoutNode(&style_root->nodes[i]));
    }
    
    if(found_prev_inline) {
        root->nodes.push_back(annoymous_layout);
    }

    for (size_t i = 0; i < root->nodes.size(); i++) {
        travese_layout_tree(&root->nodes[i]);
    }
    
}



void layout_tree_print(LayoutNode layout_root,int depth = 0) { 
    if(depth == 0) {
        printf("%*s%s\n",depth * STYLE_TREE_PRINT_DEPTH_SPACING,"",layout_root.display_box.c_str());
        depth++;
    }
    
    for (size_t i = 0; i < layout_root.nodes.size(); i++) {
        printf("%*s%s\n",depth * STYLE_TREE_PRINT_DEPTH_SPACING,"",layout_root.nodes[i].display_box.c_str());
        layout_tree_print(layout_root.nodes[i], depth + 1);
    }
}








#endif //ENGINE_LAYOUT_NODE_H