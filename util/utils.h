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
    static std::string num_to_str(T v) {
        std::string ret = "";
        std::stringstream ss;
        ss << v;
        ss >> ret;
        return ret;
    }
	
	static int split_ch_words(const std::string& word, std::vector<std::string>& cn_words)  {
		std::string cn_word = "";
		std::string not_cn_word = "";
		int i = 0;
		cn_words.clear();
		int len = word.length();
		while ( i < len ) {
			if ( (int) (word[i] & 0xf0 ) == 0xe0 ) {
				if ( (int) (word[i + 1] & 0xc0 ) == 0x80 && \
						(int) (word[i + 2] & 0xc0 ) == 0x80 ) {
					cn_words.push_back(word.substr(i, 3));
					i += 3;
				} else {        
					i += 1;
				}
			} else if ( word[i] != ' ') {
				if ((int)(word[i]) <= 127) {
					cn_words.push_back(std::string(1, word[i]));
					i ++;
				} else {
					int j = i;
					while ( j <= word.length() && !(word[j] >= 0 && word[j] <= 127 ) ) {
						j ++;
					}
					cn_words.push_back(word.substr(i, j - i));
					i = j;
				}
			} else {
				i += 1;
			}
		}    
		return 1;
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
