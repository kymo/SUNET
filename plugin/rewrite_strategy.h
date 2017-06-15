/**
 * @file rewrite_strategy.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief abstract class of strategy 
 *  
 **/

#ifndef _REWRITE_STRTEGY_H_
#define _REWRITE_STRTEGY_H_

#include <map>
#include <iostream>
#include <string.h>
#include <vector>

#include "istrategy.h"
#include "define.h"
#include "http_parse.h"

namespace sub_framework {
    
class RewriteStrategy : public IStrategy {
// wordseg , normalization, and number normalized
public:
    RewriteStrategy();
    virtual ~RewriteStrategy();

    int _process(const Request& req, Json::Value& root, const int& level);
    int _init();

};

}

#endif

