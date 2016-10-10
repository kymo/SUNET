 
/**
 * @file task_mgr.h
 * @author kymowind@gmail.com
 * @date 2016/10/10 11:08:48
 * @brief 
 *  
 **/




#ifndef  __TASK_MGR_H_
#define  __TASK_MGR_H_

#include <iostream>
#include <string>


namespace sub_framework {

typedef int (*CALL_BACK_PROC)(void*, void*);

class TaskMgr {
    
private:
    TaskMgr();
    static TaskMgr* _task_mgr_instance;
    std::map<std::string, CALL_BACK_PROC> _task_call_back_proc_dict;
    std::queue<Task*> _task_queue;
    int _max_task_cnt;
    int _task_query_size;
    pthread_mutex_t _add_mutex;
    pthread_mutex_t _get_mutex;
    pthread_cond_t _cond;
public:
    ~TaskMgr();
    static TaskMgr* _get_instance() {
        if (NULL == _task_mgr_instance) {
            _task_mgr_instance = new TaskMgr();
        }
        return _task_mgr_instance;
    }
    void _init();
    void _clear_task_queue();
    void _get_call_back_proc(const std::string& _task_name);
    void _add_task();
    Task* _get_task();
}
}













#endif  //__TASK_MGR_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
