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

namespace sub_framework {

class SubStrategyConfig {

private:
	SubStrategyConfig() {}
    
	std::map<std::string, std::vector<std::pair<std::string, int> > > _conf_dict;
    
	static SubStrategyConfig* _sub_strategy_config_instance;

public:
    ~SubStrategyConfig() {}
    
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
		std::vector<std::pair<std::string, int> >& uri_strategy_map);

};

}


#endif 

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
