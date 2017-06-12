#include "sub_log.h"

namespace sub_framework {

SubLogger* SubLogger::_instance = NULL;

void SubLogger::_init_logger() {
    _log_level = 15;
    SubConfig::_get_instance()->_get_conf_int_val("log_level", _log_level);
    _log_path = "./log";
    SubConfig::_get_instance()->_get_conf_val("log_path", _log_path);
    // init lock mutex
    _log_file_name = "";
    _log_file_p = NULL;
    _update_log_file_name(NULL);
    pthread_mutex_init(&_log_mutex, NULL);
}

int SubLogger::_update_log_file_name(char *log_line) {
    time_t cur_time;
    struct tm* time_info = NULL;
    time(&cur_time);
    time_info = localtime(&cur_time);
    char line[64];
    sprintf(line, "sub.log.%04d%02d%02d%02d00", time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, time_info->tm_hour);
    if (NULL != log_line) {
        sprintf(log_line, "%04d-%02d-%02d %02d:%02d:%02d", time_info->tm_year + 1900, time_info->tm_mon + 1, 
           time_info->tm_mday, time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    }
    if (strcmp(_log_file_name.c_str(), line) != 0) {
        if (_log_file_p) {
            fclose(_log_file_p);
        }
        _log_file_name = line;
        _log_file_p = fopen((_log_path + _log_file_name).c_str(), "a+");
        if (!_log_file_p) {
            return LOG_FAIL;
        }
    }
    return LOG_OK;
}


void SubLogger::_write_log(int log_type, const char* format, ...) {
    if (! (1 << log_type & _log_level)) {
        return;
    }

    pthread_mutex_lock(&_log_mutex);
    char log_line[MAX_LOG_LEN];
    if (LOG_FAIL == _update_log_file_name(log_line)) {
        std::cerr << "[ERROR] error when write log!" << std::endl;
        return ;
    }
    switch (log_type) {
    case DEBUG:
        strcat(log_line, " [DEBUG] ");
        break;
    case WARN:
        strcat(log_line, " [WARNING] ");
        break;
    case NOTICE:
        strcat(log_line, " [NOTICE] ");
        break;
    case FATAL:
        strcat(log_line, " [FATAL] ");
    default:
        pthread_mutex_unlock(&_log_mutex);
        return;
    }
    va_list arg_ptr;
    va_start(arg_ptr, format);
    vsnprintf(log_line + strlen(log_line), MAX_LOG_LEN - strlen(log_line), format, arg_ptr);
    va_end(arg_ptr);
    if (NULL == _log_file_p) {
        _log_file_p = fopen((_log_path + _log_file_name).c_str(), "a+");
        if (! _log_file_p) {
            std::cerr << "[ERROR] error when create log file!" << std::endl;
            return ;
        }
    }
    fprintf(_log_file_p, "%s\n", log_line);
    fflush(_log_file_p);
    pthread_mutex_unlock(&_log_mutex);
}

}


