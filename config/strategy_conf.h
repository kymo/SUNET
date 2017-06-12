/**
 * @file strategy_config.h
 * @author kymowind@gmail.com
 * @date 2016/10/09 19:49:56
 * @brief 
 *  
 **/

#ifndef  __STRATEGY_CONFIG_H_
#define  __STRATEGY_CONFIG_H_

#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include "define.h"
#include "utils.h"
namespace sub_framework {

typedef struct strategy_conf {
	std::string _strategy_name;
	int _strategy_level;

	strategy_conf();
	strategy_conf(const std::string& strategy_name,
		const int& strategy_level) {
		_strategy_name = strategy_name;
		_strategy_level = strategy_level;
	}

} STRATEGYTYPE;

class SubStrategyConfig {

private:
	SubStrategyConfig() {}
    
	static SubStrategyConfig* _sub_strategy_config_instance;
	
	std::map<std::string, std::vector<STRATEGYTYPE> > _strategy_conf_dict;

public:
    ~SubStrategyConfig() {}

	const std::map<std::string, std::vector<STRATEGYTYPE> >& _get_strategytype_map() const {
		return _strategy_conf_dict;
	}

    static SubStrategyConfig* _get_instance() {
        
		if (NULL == _sub_strategy_config_instance) {
            _sub_strategy_config_instance = new SubStrategyConfig();
        }
        
		return _sub_strategy_config_instance;
    
	}
	/*
	 * @biref read conf file
	 * @return 
	 *		SUB_OK
	 *		SUB_FAIL
	 */
    int _read_conf_file(const std::string& file_name);

	
	int _get_uri_strategies(const std::string& uri, 
		std::vector<STRATEGYTYPE>& uri_strategy_vec);

};

}


#endif 

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
