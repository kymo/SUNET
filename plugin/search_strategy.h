/**
 * @file search_strategy.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief abstract class of strategy 
 *  
 **/

#ifndef _SEARCH_STRTEGY_H_
#define _SEARCH_STRTEGY_H_

#include <map>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <math.h>

#include "istrategy.h"
#include "define.h"
#include "http_parse.h"
#include "wordseg.h"
#include "config.h"
#include "dat.h"

namespace sub_framework {


class SearchStrategy : public IStrategy {

private:
    static const int PAGE_RESULT_CNT = 20;
    // DatImpl<std::vector<reverse_index> > *_dat;
    std::map<std::string, std::vector<reverse_index> > _index_dict;
    std::map<std::string, int> _word_freq_dict;
    std::map<std::string, int> _word_doc_freq_dict;
    std::map<int, int> _doc_length_dict;

public:
    SearchStrategy();
    virtual ~SearchStrategy();

    int _process(const Request& req, Json::Value& root, const int& level);
    int _init();

};

}

#endif

