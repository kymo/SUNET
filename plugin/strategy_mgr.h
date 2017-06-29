/**
 * @file strategy_mgr.h
 * @author kymowind@gmail.com
 * @date 2017/02/14 20:17:42
 * @brief 
 *  
 **/

#ifndef  __STRATEGYMGR_H_
#define  __STRATEGYMGR_H_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "define.h"
#include "strategy_conf.h"
#include "istrategy.h"
#include "dict_mgr.h"

namespace sub_framework {

#define STRATEGY_MAP std::map<std::string, std::vector<STRATEGYTYPE> >


typedef struct strategy_type_mgr {
    IStrategy* _sub_strategy;
    int _strategy_level;
    strategy_type_mgr() {}
    strategy_type_mgr(IStrategy* sub_strategy, int strategy_level) {
        _sub_strategy = sub_strategy;
        _strategy_level = strategy_level;
    }
} STRATEGY_TYPE_MGR;

class SubStrategyMgr {

private:
    SubStrategyMgr() {}
    static SubStrategyMgr* _strategy_mgr_instance;
    std::map<std::string, std::vector<STRATEGY_TYPE_MGR> > _uri_strategies_map;

public:
    ~SubStrategyMgr();
    static SubStrategyMgr* _get_instance() {
        if (NULL == _strategy_mgr_instance) {
            _strategy_mgr_instance = new SubStrategyMgr();
        }
        return _strategy_mgr_instance;
    }

    /*
     * @brief produce the strategy according to name
     * @return
     */
    IStrategy* _produce(const std::string& strategy_name);

    /*
     * @brief init the strategies accroding to the strategy conf
     * @return
     *         SUB_OK init ok
     *         SUB_FAIL init failure
     *
     */
    int _init_strategies();
    
    // SubStrategy* _get_strategy_instance(const std::string stragey_name);
    
    /*
     * @brief run the strategy binded to the uri
     * @return
     *         SUB_OK run ok
     *         SUB_FAIL run failue
     */
    int _run_uri(const std::string& uri, const Request& req, Json::Value& root);    
    
};


}
#endif
