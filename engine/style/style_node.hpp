#ifndef ENGINE_STYLE_NODE_H
#define ENGINE_STYLE_NODE_H

#include <vector>

#include "style.hpp"
#include "../dom/node.hpp"


const static int STYLE_TREE_PRINT_DEPTH_SPACING = 2;


class StyleNode;
static void travese_dom_tree(StyleNode* style_root);

class StyleNode {
public:
    std::vector<StyleNode> nodes;
    Node* dom_node;

    StyleNode(){}
    StyleNode(Node* dm):dom_node(dm) {
    }
    
    ~StyleNode() {

    }

    void build_tree(Node* dom_root) {
        this->dom_node = dom_root;
        travese_dom_tree(this);
    }


};



static void travese_dom_tree(StyleNode* style_root) { 
    auto dom_root = style_root->dom_node;
    
    for (size_t i = 0; i < dom_root->children.size(); i++) {
        style_root->nodes.push_back(StyleNode(dom_root->children[i]));
    }

    for (size_t i = 0; i < style_root->nodes.size(); i++) {
        travese_dom_tree(&style_root->nodes[i]);
    }
}


void style_tree_print(StyleNode style_root,int depth = 0) { 
    auto dom_root = style_root.dom_node;
    if(depth == 0) {
        printf("%*s%s\n",depth * STYLE_TREE_PRINT_DEPTH_SPACING,"",dom_root->id.c_str());
        depth++;
    }
    
    for (size_t i = 0; i < dom_root->children.size(); i++) {
        printf("%*s%s\n",depth * STYLE_TREE_PRINT_DEPTH_SPACING,"",dom_root->children[i]->id.c_str());
        style_tree_print(style_root.nodes[i], depth + 1);
    }
}





#endif //ENGINE_STYLE_NODE_H