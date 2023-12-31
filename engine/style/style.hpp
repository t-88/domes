#ifndef ENGINE_STYLE_H
#define ENGINE_STYLE_H

#include <vector>
#include <string>
#include <map>
#include <assert.h>


#include "SDL2/SDL.h"

typedef std::map<std::string,std::string> StyleProps; 

#define DisplayBlock "block"
#define DisplayInline "inline"
#define DisplayAnonymous "anonymous"

#define AUTO "auto"
#define ZERO "0"






class Style {
public:
    StyleProps props;
    std::string style_src;


    Style(){
        props.clear();
        default_style();
    }
    ~Style() { }



    void default_style() {
        props["display"] = DisplayBlock;
        
        props["width"] = AUTO;
        props["height"] = AUTO;
        
        props["margin_left"] = ZERO;
        props["margin_right"] = ZERO;
    
        props["padding_left"] = ZERO;
        props["padding_right"] = ZERO;


        props["color"] = "255,255,255,255";
        props["margin_color"] = "0,0,0,0";
        props["border_color"] = "0,0,0,0";
        
    }
    void print() {
        printf("style start:\n");
        for (auto& it : props) {
            printf("    %s : %s\n",it.first.c_str(),it.second.c_str());
        }
        printf("style end;\n");

        
    }
    
    void parse_style() {
        int cur_idx = 0; 
        std::string line;
        while (cur_idx < style_src.size()) {
            char cur_char = style_src[cur_idx];
            cur_idx++;
            if(cur_char == ';') {
                int split_idx =  line.find(":");
                std::pair<std::string,std::string> prop_value;

                for (size_t i = 0; i < split_idx; i++) {
                    prop_value.first.push_back(line[i]);
                }
                for (size_t i = split_idx + 1; i < line.size(); i++) {
                    prop_value.second.push_back(line[i]);
                }

                props[prop_value.first] = prop_value.second;

                line = "";
            } else {
                if(cur_char == ' ') continue;
                line.push_back(cur_char);
            }
        }
    }
};

int to_px(std::string in) {
    if(in == "auto" || in == "") return 0;
    return std::stoi(in,0,10);

}

SDL_Color to_color(std::string in) {
    //TODO: why is color = ""
    assert(in != "" && "empty string!!");

    // if(in == "") return;


    // printf("    in = %s\n",in.c_str());
    in.push_back(',');
    
    SDL_Color color;
    int i = 0;
    
    std::string number;
    Uint8 comps[4];
    int cur_comp = 0;

    while (i < in.size()) {
        if(in.at(i) == ',') {
            comps[cur_comp] = std::stoi(number,0,10);
            number = "";
            cur_comp++;
        } else {
            number.push_back(in.at(i));
        }
        i++;
    }

    return (SDL_Color){comps[0],comps[1],comps[2],comps[3]};
}







#endif //ENGINE_STYLE_H