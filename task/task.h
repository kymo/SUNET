 
/**
 * @file task.h
 * @author kymowind@gmail.com
 * @date 2016/10/09 20:46:07
 * @brief 
 *  
 **/




#ifndef  __TASK_H_
#define  __TASK_H_

#include <iostream>
#include <map>
#include <string>
#include "sub_log.h"

namespace sub_framework {

class SubTask {

public:
    SubTask() {}
    virtual ~SubTask() {
	}
    void* _task_data;
    void* _task_ret;
    std::string _task_name;
    virtual int _run() = 0;
    virtual int _call_back() = 0;
    virtual void _set_task_data(void *task_data) = 0;
};

}










#endif  //__TASK_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
