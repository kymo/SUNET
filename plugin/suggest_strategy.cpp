/**
 * @file suggest_strategy.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 *  
 **/

#include "suggest_strategy.h"
#include "sub_log.h"
#include "dict_mgr.h"
namespace sub_framework {

SuggestStrategy::SuggestStrategy() {
    _init();
}

SuggestStrategy::~SuggestStrategy() {
}

int SuggestStrategy::_process(const Request& req, Json::Value& root, const int& level) {
    
    std::string q = req.get_query();
    Json::Value result;
    
    int idx = 0;
       std::vector<std::pair<std::string, int> > ret;
    _suggest_dict->search(q, ret);
    for (int i = 0; i < ret.size() && i < MAX_SUGGEST_CNT; i++) {
        Json::Value data;
        data["id"] = ret[i].second;
        data["title"] = ret[i].first;
        result.append(data);
    }
    root["suggest"] = result;
    return SUB_OK;
}

int SuggestStrategy::_init() {
    // _dat = new DatImpl<std::vector<reverse_index> >();
    REFER_DICT(_suggest_dict);
    return SUB_OK;
}

}
