
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
    Offset left_offset;
    Offset right_offset;


    int l_idx_counter = 0;
    int r_idx_counter = 10;


    UIContainer() : UIBaseRect(0,0,0,0) { 
        children.resize(max_children);
        for (size_t i = 0; i < max_children; i++){
            children[i] = 0;
        }

        parent = 0;
        

    }
    UIContainer(int x,int y,int w,int h) : UIBaseRect(x,y,w,h){ 
        children.resize(max_children);
        for (size_t i = 0; i < max_children; i++){
            children[i] = 0;
        }
        parent = 0;
        
    }
    UIContainer(int w,int h) : UIBaseRect(w,h){ 
        children.resize(max_children);
        for (size_t i = 0; i < max_children; i++){
            children[i] = 0;
        }
        parent = 0;
        
    }


    bool child_collided(UIBaseRect* curr, UIBaseRect* out) {
        for (size_t i = 0; i < children.size(); i++) {
            // if(AABB(cur))
        }
        
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

    virtual void calc_outer_bbox() override {
        outer_bbox = rect;

        Offset p1(rect.x,rect.y);
        Offset p2(rect.x + rect.w,rect.y + rect.h);

        StyleContext context = {rect.w,rect.h};
        RelativeStyleSetting r_style = styles.parse_style(context);
        

        if(parent) {
            // top
            if(p1.y - r_style.margin[0] >= parent->inner_bbox.y) {
                p1.y -= r_style.margin[0];
            } else {
                p1.y = parent->inner_bbox.y;
                rect.y += r_style.margin[0];
            }
            // buttom
            if(p2.y + r_style.margin[2] <= parent->inner_bbox.y + parent->inner_bbox.h) {
                p2.y += r_style.margin[2];
            } else {
                p2.y = parent->inner_bbox.y + parent->inner_bbox.h;
                rect.y -= r_style.margin[2];
            }




            if(r_style.margin[1]) {
                UIBaseRect* right_bro;
                if(index + 1 == parent->max_children ) { // right most  child
                    right_bro = parent;
                } else {
                    right_bro = parent->children[index + 1];
                }
                if(!right_bro) {right_bro = parent;}


                // right
                if(p2.x + r_style.margin[1] <= parent->inner_bbox.x + parent->inner_bbox.w) {
                    p2.x -= r_style.margin[1];
                } else {
                    p2.x = parent->inner_bbox.x + parent->inner_bbox.w;
                    p1.x -= r_style.margin[1];
                    rect.x = p1.x;
                }                

            } 

            if(r_style.margin[3]) {
                UIBaseRect* left_bro; 
                if(parent && index == 0) { // left most  child
                    left_bro = parent;
                } else {
                    left_bro = ((UIContainer*)parent)->children[index - 1];
                }
                if(!left_bro) {left_bro = parent;}
                printf("%d %d\n",p1.x - r_style.margin[3],left_bro->index);


                // left

                if(p1.x - r_style.margin[3] >= left_bro->rect.x) {
                    p1.x -= r_style.margin[3];
                } else {
                    // p1.x = left_bro->rect.x;
                    // rect.x += r_style.margin[2];
                }
            }

            outer_bbox.x = p1.x;
            outer_bbox.y = p1.y;
            outer_bbox.w = p2.x - p1.x;
            outer_bbox.h = p2.y - p1.y;

            // rect.print();

            return;
        }

        UIBaseRect::calc_outer_bbox();

    }


    virtual void update(Offset l_offset = ZeroOffset,Offset r_offset = ZeroOffset) override {
        UIBaseRect::update(l_offset,r_offset);

        for (size_t i = 0; i < children.size(); i++) {
            left_offset.set(0,0);
            right_offset.set(0,0);

            if(children[i]) {
                children[i]->update(        left_offset,right_offset);
                // children[i]->update_offset(&left_offset,&right_offset);
            }
        }
        
    }
    virtual void render(SDL_Renderer* renderer) override {
        UIBaseRect::render(renderer);

        for (size_t i = 0; i < children.size(); i++) {
            if(children[i]) {
                children[i]->render(renderer);
            }
        }
    }

};



#endif //UI_CONTAINER_H_