 
/**
 * @file task_mgr.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/10 20:48:26
 * @brief 
 *  
 **/

#include "task_mgr.h"


namespace sub_framework {

SubTaskMgr* SubTaskMgr::_task_mgr_instance = NULL;

SubTaskMgr::SubTaskMgr() {

}

SubTaskMgr::~SubTaskMgr() {
    // empty the task query
    _clear_task_queue();
    pthread_mutex_destroy(&_add_mutex);
    pthread_mutex_destroy(&_get_mutex);
}

void SubTaskMgr::_init() {
    std::cout << "init mutex" << std::endl;
    pthread_mutex_init(&_add_mutex, NULL);
    pthread_mutex_init(&_get_mutex, NULL);
    std::cout << "~" << std::endl;
}

void SubTaskMgr::_set_call_back_proc(const std::string& _task_name, CALL_BACK_PROC call_back_proc) {
    _task_call_back_proc_dict[_task_name] = call_back_proc;
}

CALL_BACK_PROC SubTaskMgr::_get_call_back_proc(const std::string& _task_name) {
    if (_task_call_back_proc_dict.find(_task_name) == _task_call_back_proc_dict.end()) {
        return NULL;
    }
    return _task_call_back_proc_dict[_task_name];
}

void SubTaskMgr::_clear_task_queue() {
    while (! _task_queue.empty()) {
        SubTask* task = _task_queue.front();
        _task_queue.pop();
        if (task != NULL) {
            delete task;
            task = NULL;
        }
    }
}

void SubTaskMgr::_add_task(SubTask* task) {    
    if (task == NULL) {
        return ;
    }
    pthread_mutex_lock(&_add_mutex);
    _task_queue.push(task);
    pthread_mutex_unlock(&_add_mutex);
}

SubTask* SubTaskMgr::_get_task() {

    if (_task_queue.empty()) {
        return NULL;
    }

    SubTask *ret_task = NULL;
    pthread_mutex_lock(&_get_mutex);
    if (! _task_queue.empty()) {
        ret_task = _task_queue.front();
        _task_queue.pop();
    }
    pthread_mutex_unlock(&_get_mutex);
    return ret_task;
}

}






















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
