    
/**
 * @file req_task.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/10 09:47:38
 * @brief 
 *  
 **/

#include "req_task.h"
#include "http_parse.h"

namespace sub_framework {

ReqTask::ReqTask(const std::string& task_name) {
    _task_name = task_name;
}

ReqTask::~ReqTask() {

}

void ReqTask::_set_task_data(void *task_data) {
    _task_data = task_data;
}

int ReqTask::_run() {
    CALL_BACK_PROC call_back_proc = SubTaskMgr::_get_instance()->_get_call_back_proc(_task_name);
    if (NULL == call_back_proc) {
        return 0;
    }
    _task_ret = (void*)(malloc(sizeof(char) * 65536));
    return (*call_back_proc)(_task_data, _task_ret);
}

int ReqTask::_call_back() {
    if (NULL == _task_ret) {
        return -1;
    } 
    char* buf = (char*) _task_data;
    //int v = *((int*)_task_ret);
    char *ret = (char*) _task_ret; 
    /*解析html请求*/
	Request request;
    HttpParser httpp;
    httpp._parse(buf, request);
	return 0;
}

}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
