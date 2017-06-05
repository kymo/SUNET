 
/**
 * @file config.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/09 19:54:42
 * @brief 
 *  
 **/

#include "config.h"

namespace sub_framework {
    
SubConfig* SubConfig::_sub_config_instance = NULL;

void SubConfig::_get_conf_int_val(const std::string& key, int& default_val) {
    std::string conf_val = "";
    _get_conf_val(key, conf_val);
    if ("" == conf_val) {
        return ;
    }
    default_val = atoi(conf_val.c_str());
}


void SubConfig::_get_conf_val(const std::string& key, std::string& conf_val) {
    conf_val = "";
    if (_conf_dict.find(key) != _conf_dict.end()) {
        conf_val = _conf_dict[key];
    } 
}

int SubConfig::_read_conf_file(const std::string& file_name) {
	std::ifstream fis(file_name.c_str());
	if (! fis) {
		return SUB_FAIL;
	}
	std::string line;
	std::string key = "";
	std::string val = "";
	while (getline(fis, line)) {
		key = "";
		val = "";
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
		_conf_dict[key] = val;
		std::cout << key << "-" << val << std::endl;
	}
	return SUB_OK;
}

int SubConfig::_get_thread_cnt() {
    int default_val = 20;
    _get_conf_int_val("thread_cnt", default_val);
    return default_val;
}

};



















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
