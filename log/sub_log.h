 /*
 * @file sub_log.h
 * @date 2017/06/12 14:49:27
 * @version $Revision$ 
 * @brief 
 *  
 **/
#ifndef SUB_LOG_H
#define SUB_LOG_H

#include <iostream>
#include <vector>
#include "config.h"
#include <time.h>
#include <pthread.h>
#include <stdarg.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

namespace sub_framework {

enum {
    DEBUG = 0,
    WARN,   // 1
    NOTICE, // 2
    FATAL,  // 3
};


#define DEBUG_LOG(fmt, ...) SubLogger::_get_instance()->_write_log(DEBUG, fmt, ##__VA_ARGS__)
#define WARN_LOG(fmt, ...) SubLogger::_get_instance()->_write_log(WARN, fmt, ##__VA_ARGS__)
#define NOTICE_LOG(fmt, ...) SubLogger::_get_instance()->_write_log(NOTICE, fmt, ##__VA_ARGS__)
#define FATAL_LOG(fmt, ...) SubLogger::_get_instance()->_write_log(FATAL, fmt, ##__VA_ARGS__)


class SubLogger {
    
public:
    static SubLogger* _get_instance() {
        if (NULL == _instance) {
            _instance = new SubLogger();
        }
        return _instance;
    }

    int _update_log_file_name(char* log_line);
    
    void _init_logger();     
    
    void _write_log(int log_level, const char* format, ...);

    ~SubLogger() {
        std::cout << "end of logg" << std::endl;
        if (_log_file_p) {
            fclose(_log_file_p);
        }
    }
private:
    static SubLogger* _instance;
    SubLogger() {}
    
    FILE* _log_file_p;
    const static int MAX_LOG_LEN = 1024;
    const static int LOG_OK = 1;
    const static int LOG_FAIL = 0;
    pthread_mutex_t _log_mutex;
    int _log_level;
    std::string _log_file_name;
    std::string _log_path;

};
}


#endif  // SUB_LOG_H



/* vim: set ts=4 sw=4 sts=4 tw=100 */
