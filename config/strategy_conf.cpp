/**
 * @file strategy_config.cpp
 * @author kymowind@gmail.com
 * @date 2017/02/14 10:20:32
 * @brief 
 *  
 **/

#include "strategy_conf.h"

namespace sub_framework {

SubStrategyConfig* SubStrategyConfig::_sub_strategy_config_instance= NULL;

int SubStrategyConfig::_read_conf_file(const std::string& file_name) {

	std::ifstream fis(file_name.c_str());

	if (!fis) {
		return SUB_FAIL;
	}
	std::string line = "";
	std::string cur_uri = "";
	std::vector<STRATEGYTYPE> uri_strategy_vec;
	uri_strategy_vec.clear();
	std::map<std::string, std::string> _map;
	while (getline(fis, line)) {
		if (line.find("[") == 0) {
			if (uri_strategy_vec.size() != 0) {
				_strategy_conf_dict.insert(std::pair<std::string, std::vector<STRATEGYTYPE> >(cur_uri, uri_strategy_vec));
			}
			cur_uri = line.substr(1, line.length() - 2);	
			uri_strategy_vec.clear();
		} else {
            std::vector<std::string> split_res;
            StringUtil::split(line, ":", split_res);
            uri_strategy_vec.push_back(STRATEGYTYPE(split_res[0], atoi(split_res[1].c_str())));
		}
	}
	if (uri_strategy_vec.size() != 0) {
		_strategy_conf_dict[cur_uri] = uri_strategy_vec;
	}
}

int SubStrategyConfig::_get_uri_strategies(const std::string& uri,
	std::vector<STRATEGYTYPE>& uri_strategy_vec) {
	if (_strategy_conf_dict.find(uri) == _strategy_conf_dict.end()) {
		return SUB_FAIL;
	}
	uri_strategy_vec = _strategy_conf_dict[uri];
	return SUB_OK;
}

}
