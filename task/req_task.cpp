 
/**
 * @file req_task.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/10 09:47:38
 * @brief 
 *  
 **/

#include "req_task.h"

namespace sub_framework {

ReqTask::ReqTask(const std::string& task_name) {
    _task_name = task_name;
}

ReqTask::~ReqTask() {

}

void ReqTask::_run() {
    CALL_BACK_PROC* call_back_proc = TaskMgr::_get_instance()->_get_call_back_proc(_task_name);
    if (NULL == call_back_proc) {
        return ;
    }
    return (*call_back_proc)(_task_data, _task_ret);
}

void ReqTask::_call_back() {

}

}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
