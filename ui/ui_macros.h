#ifndef UI_MACROS_H
#define UI_MACROS_H

#include "color.h"

#define hex_to_color(color) ((color) & 0xFF >> 8 * 2) ,  ((color) & 0xFF >> 8 * 1) , ((color) & 0xFF >> 8 * 0) , 0xFF 
#define color_spread(color) (color).r,(color).g,(color).b,(color).a 

#endif // UI_MACROS_H
