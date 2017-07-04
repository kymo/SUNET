/**
 * @file suggest_strategy.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief abstract class of strategy 
 *  
 **/

#ifndef _SUGGEST_STRTEGY_H_
#define _SUGGEST_STRTEGY_H_

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
#include "trie.h"

namespace sub_framework {


class SuggestStrategy : public IStrategy {

private:
    static const int MAX_SUGGEST_CNT = 20;
	Trie<int>* _suggest_dict;

public:
    SuggestStrategy();
    virtual ~SuggestStrategy();

    int _process(const Request& req, Json::Value& root, const int& level);
    int _init();

};

}

#endif

