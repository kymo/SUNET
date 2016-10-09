 
/**
 * @file config.h
 * @author kymowind@gmail.com
 * @date 2016/10/09 19:49:56
 * @brief 
 *  
 **/



#ifndef  __CONFIG_H_
#define  __CONFIG_H_
#include <iostream>
#include <string>
#include <map>


namespace sub_framework {

class SubConfig {

private:
    std::map<std::string, std::string> _conf_dict;
    static SubConfig* _sub_config_instance;
    SubConfig() {}

public:
    ~SubConfig() {}
    
    static SubConfig* _get_instance() {
        if (NULL == _sub_config_instance) {
            _sub_config_instance = new SubConfig();
        }
        return _sub_config_instance;
    }
    void _get_conf_int_val(const std::string& key, int &default_val);
    void _get_conf_val(const std::string& key, std::string& conf_val);
    int _read_conf_file(const std::string& file_name);
    int _get_thread_cnt();
};

}














#endif  //__CONFIG_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
