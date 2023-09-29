#ifndef ENGINE_STYLE_H
#define ENGINE_STYLE_H

#include <vector>
#include <string>
#include <unordered_map>


typedef std::unordered_map<std::string,std::string> StyleProps; 

#define DisplayBlock "block"
#define DisplayInline "inline"
#define DisplayAnonymous "anonymous"

#define AUTO "auto"



class Style {
public:
    StyleProps props;


    Style(){
        default_style();
    }
    Style(StyleProps p){
        default_style();
        props = p;
    }
    ~Style() { }



    void default_style() {
        props["display"] = DisplayBlock;
        
        props["width"] = AUTO;
        props["height"] = AUTO;
        
        props["margin_left"] = AUTO;
        props["margin_right"] = AUTO;
    
        props["padding_left"] = AUTO;
        props["padding_right"] = AUTO;
    }

    void print() {
        for(auto& prop : props) {
            printf("%s %s ",prop.first.c_str(),prop.second.c_str());
        }
        printf("\n");

    }
};

int to_px(std::string in) {
    if(in == "auto" || in == "") return 0;
    return std::stoi(in,0,10);
}






#endif //ENGINE_STYLE_H