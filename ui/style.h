#ifndef UI_STYLE_H_
#define UI_STYLE_H_

#include "ui_shared_headers.h"



typedef struct StyleContext {
    int width , height;
} StyleContext;

typedef enum Float {
    Left , Right, Center 
} Float;

typedef struct RelativeStyleSetting {
    int padding[4];
    int margin[4];
    int top , buttom, left , right;
    int Horz , Vecrt;
} RelativeStyleSetting;

typedef struct StyleSetting {
    std::string padding[4];
    std::string margin[4];

    std::string top , buttom, left , right;

    Float Horz , Vecrt;


    StyleSetting() {
        for (size_t i = 0; i < 4; i++){
            padding[i] = "0";
            margin[i] = "0";
        }
        top = "0", buttom= "0", left = "0", right = "0";

        Horz = Left;
        Vecrt = Left;
    }

    void set_padding(std::string top = "",std::string right = "",std::string buttom = "",std::string left = "") {
        padding[0] = top != "" ? top : padding[0]     ;
        padding[1] = right != "" ? right : padding[1]   ;
        padding[2] = buttom != "" ? buttom : padding[2] ;  
        padding[3] = left != "" ? left : padding[3]    ;
    }
    void set_margin(std::string top = "",std::string right = "",std::string buttom = "",std::string left = "") {
        margin[0] = top != "" ? top : margin[0]     ;
        margin[1] = right != "" ? right : margin[1]   ;
        margin[2] = buttom != "" ? buttom : margin[2] ;  
        margin[3] = left != "" ? left : margin[3]    ;
    }    

    int parse_unit(std::string str ,StyleContext context,bool horizontal = true) {
        if(*str.end() == '%') {
            if(horizontal) {
                return  std::stoi(str) / 100 * context.height;
            }
            return  std::stoi(str) / 100 * context.width;
        }
        return std::stoi(str);
    }
    RelativeStyleSetting parse_style(StyleContext context) {
        RelativeStyleSetting r_style; // relative style

        // padding
        for (size_t i = 0; i < 4; i++){
            r_style.padding[i] = parse_unit(padding[i],context,i == 1 || i == 3);
        }

        // margin
        for (size_t i = 0; i < 4; i++){
            r_style.margin[i] = parse_unit(margin[i],context,i == 1 || i == 3);
        }        

        return r_style;
    }
} StyleSetting;




#endif