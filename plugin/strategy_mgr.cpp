/**
 * @file strategy_mgr.cpp
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief 
 *  
 **/

#include "strategy_mgr.h"
#include "text_strategy.h"
#include "rewrite_strategy.h"
#include "search_strategy.h"
#include "suggest_strategy.h"
#include "dict_mgr.h"
#include "trie.h"

namespace sub_framework {

SubStrategyMgr* SubStrategyMgr::_strategy_mgr_instance = NULL;
DictMgr* DictMgr::_instance = NULL;

IStrategy* SubStrategyMgr::_produce(const std::string& strategy_name) {
    IStrategy* sub_strategy = NULL;
    if (strategy_name == "TextStrategy") {
        sub_strategy = new TextStrategy();
    } else if (strategy_name == "RewriteStrategy") {
        sub_strategy = new RewriteStrategy();
    } else if (strategy_name == "SearchStrategy") {
		sub_strategy = new SearchStrategy();
	} else if (strategy_name == "SuggestStrategy") {
		sub_strategy = new SuggestStrategy();
	}
    return sub_strategy;
}

int SubStrategyMgr::_init_strategies() {
    // init all the strategies
    
    DictMgr::_get_instance()->_load_dict();

    const STRATEGY_MAP& strategy_type_map = 
        SubStrategyConfig::_get_instance()->_get_strategytype_map();

    std::map<std::string, IStrategy*> sub_strategy_map;
    for (STRATEGY_MAP::const_iterator it = strategy_type_map.begin();
        it != strategy_type_map.end(); it++) {
        std::string uri = it->first;
        const std::vector<STRATEGYTYPE>& strategy_type_vec = it->second;
        std::vector<STRATEGY_TYPE_MGR> strategy_vec;
        for (int i = 0; i < strategy_type_vec.size(); i++) {
            if (sub_strategy_map.find(strategy_type_vec[i]._strategy_name) == 
				sub_strategy_map.end()) {
                IStrategy* strategy = _produce(strategy_type_vec[i]._strategy_name);
                if (NULL == strategy) {
                    return SUB_FAIL;
                }
                sub_strategy_map[strategy_type_vec[i]._strategy_name] = strategy;
            }
            strategy_vec.push_back(
				STRATEGY_TYPE_MGR(sub_strategy_map[strategy_type_vec[i]._strategy_name], 
                strategy_type_vec[i]._strategy_level)); 
        }
        _uri_strategies_map[uri] = strategy_vec;
    }
    return SUB_OK;
}

int SubStrategyMgr::_run_uri(const std::string& uri, const Request& req, Json::Value& root) {

    if (_uri_strategies_map.find(uri) == _uri_strategies_map.end()) {
        return SUB_FAIL;
    }
    const std::vector<STRATEGY_TYPE_MGR>& strategy_vec = _uri_strategies_map[uri];
    for (int i = 0; i < strategy_vec.size(); i++) {
        IStrategy* strategy = strategy_vec[i]._sub_strategy;
        strategy->_process(req, root, strategy_vec[i]._strategy_level);
    }
    return SUB_OK;
}

}
