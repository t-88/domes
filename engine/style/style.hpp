#ifndef ENGINE_STYLE_H
#define ENGINE_STYLE_H

#include <vector>
#include <string>
#include <unordered_map>


typedef std::unordered_map<std::string,std::string> StyleProps; 

#define DisplayBlock "block"
#define DisplayInline "inline"
#define DisplayAnonymous "anonymous"



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
    }
};






#endif //ENGINE_STYLE_H