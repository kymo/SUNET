/**
 * @file task_handler.h
 * @author kymowind@gmail.com
 * @date 2016/10/11 09:51:37
 * @brief 
 *  
 **/




#ifndef  __TASK_HANDLER_H_
#define  __TASK_HANDLER_H_
#include "thread.h"
#include "task_mgr.h"
#include <time.h>

namespace sub_framework {

class SubTaskHandler : public SubThreadHandler {
public:
    virtual int _thread_proc_handler(void *args) {
        while (! _stoped) {
            SubTask* task = SubTaskMgr::_get_instance()->_get_task();
            if (NULL != task) {
                std::cout << "deal with task " << task->_task_name << std::endl;
                task->_run();
                task->_call_back();
            }
        }
    }
};

}













#endif  //__TASK_HANDLER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
