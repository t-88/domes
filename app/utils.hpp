#ifndef APP_UTILS_HPP
#define APP_UTILS_HPP


#include <stdio.h>
#include <time.h>

#include <vector>
#include <string>

#include "../globals.h"


#define setStyle(node,src)  (node).style.style_src = src; \
                            (node).style.parse_style();


static time_t cur_time;

// get_cur_date
tm* get_cur_date() {
    cur_time = time(0);
    return localtime(&cur_time);;
}
// get_date_str
std::string get_date_str(const char* seprator = "/") {
    tm* parsed_time = get_cur_date();
    char date[30];
    sprintf(date,"%d%s%d%s%d",
                parsed_time->tm_mday,
                seprator,
                parsed_time->tm_mon + 1,
                seprator,
                parsed_time->tm_year + 1900);

    std::string date_string(date);
    return date_string;
}
// get_time_str
std::string get_time_str(const char* seprator = ":") {
    tm* parsed_time = get_cur_date();
    char date[30];
    sprintf(date,"%02d%s%02d%s%02d",
                parsed_time->tm_hour,
                seprator,
                parsed_time->tm_min,
                seprator,
                parsed_time->tm_sec);

    std::string date_string(date);
    return date_string;
}



#endif