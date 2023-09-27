
#ifndef UI_CONTAINER_H_
#define UI_CONTAINER_H_

#include "ui_shared_headers.h"
#include "rect.h"
#include "style.h"
#include "base_rect.h"

#include <vector>
#include <assert.h>


class UIContainer : public UIBaseRect {
public:
    // max of ten elements for every container
    std::vector<UIBaseRect*> children;
    Offset left_offset;
    Offset right_offset;

    int max_children = 11;

    int l_idx_counter = 0;
    int r_idx_counter = 10;


    UIContainer() : UIBaseRect(0,0,0,0) { 
        children.resize(max_children);
    }
    UIContainer(int x,int y,int w,int h) : UIBaseRect(x,y,w,h){ 
        children.resize(max_children);
    }
    UIContainer(int w,int h) : UIBaseRect(w,h){ 
        children.resize(max_children);
    }


    void append(UIBaseRect* child) {
        assert(l_idx_counter < r_idx_counter && "too much elements bruh");
        child->parent = this;


        if(child->styles.Horz == Left) {
            children[l_idx_counter] = child;
            child->index = l_idx_counter;
            l_idx_counter++;
        } else {
            children[r_idx_counter] = child;
            child->index = r_idx_counter;
            r_idx_counter--;
        }
    }

    void calc_outer_bbox() override {
        outer_bbox = rect;

        StyleContext context = {rect.w,rect.h};
        RelativeStyleSetting r_style = styles.parse_style(context);



    }

    
    void update(Offset l_offset = ZeroOffset,Offset r_offset = ZeroOffset) override {
        UIBaseRect::update(l_offset,r_offset);
        left_offset.set(inner_bbox.x,inner_bbox.y);
        right_offset.set(inner_bbox.x + inner_bbox.w,inner_bbox.y);

        for (size_t i = 0; i < children.size(); i++) {
            if(children[i]) {
                children[i]->update(        left_offset,right_offset);
                children[i]->update_offset(&left_offset,&right_offset);
            }
        }
        
    }
    void render(SDL_Renderer* renderer) override {
        UIBaseRect::render(renderer);

        for (size_t i = 0; i < children.size(); i++) {
            if(children[i]) {
                children[i]->render(renderer);
            }
        }
    }

};



#endif //UI_CONTAINER_H_