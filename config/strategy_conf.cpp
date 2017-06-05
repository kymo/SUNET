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
	std::vector<std::pair<std::string, int> > uri_strategy_vec;
	while (getline(fis, line)) {
		if (line.find("[") == 0) {
			if (uri_strategy_vec.size() != 0) {
				_conf_dict[cur_uri] = uri_strategy_vec;
			}
			cur_uri = line.substr(1, line.length() - 2);	
			uri_strategy_vec.clear();
		} else {
			std::string key = "";
			std::string val = "";
			std::string str = "";
			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ' ') continue;
				if (line[i] == ':') {
					key = str;
					str = "";
				} else {
					str += line[i];
				}
			}
			val = str;
			uri_strategy_vec.push_back(std::make_pair<std::string, int>(key, atoi(val.c_str())));
		}
	}
	if (uri_strategy_vec.size() != 0) {
		_conf_dict[cur_uri] = uri_strategy_vec;
	}
	for (std::map<std::string, std::vector<std::pair<std::string, int> > >::iterator it = _conf_dict.begin();
			it != _conf_dict.end(); it++) {
		std::cout << it->first << ":";
		for (int i = 0; i < it->second.size(); i++) {
			std::cout << "|" << it->second[i].first << ":" << it->second[i].second << "|" << std::endl;
		}
	}
}

int SubStrategyConfig::_get_uri_strategies(const std::string& uri,
	std::vector<std::pair<std::string, int> >& uri_strategy_vec) {
	if (_conf_dict.find(uri) == _conf_dict.end()) {
		return SUB_FAIL;
	}
	uri_strategy_vec = _conf_dict[uri];
	return SUB_OK;
}

}
