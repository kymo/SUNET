/**
 * @file utils.h
 * @date 2017/06/12 19:10:25
 * @version $Revision$ 
 * @brief 
 *  
 **/
#ifndef __UTIL_H
#define __UTIL_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace sub_framework {

class StringUtil {

public:

    template<typename T>
    static std::string num_to_str(T& v) {
        std::string ret = "";
        std::stringstream ss;
        ss << v;
        ss >> ret;
    }

    static void split(const std::string& str, 
            const std::string& delim,
            std::vector<std::string>& ret) {
        if (str.size() <= 0 || delim.size() <= 0) {
            ret.clear();
            return;
        }
        ret.clear();
        int last = 0;
        int index = str.find_first_of(delim, last);
        while (index != -1) {
            ret.push_back(str.substr(last, index - last));
            last = index + 1;
            index = str.find_first_of(delim, last);
        }
        if (index == -1 && str[str.size() - 1] != '\t') {
            ret.push_back(str.substr(last, str.size() - last));
        }
    }

};
}

#endif 

/* vim: set ts=4 sw=4 sts=4 tw=100 */
