 
/**
 * @file req_task.h
 * @author kymowind@gmail.com
 * @date 2016/10/10 09:24:23
 * @brief 
 *  
 **/




#ifndef  __REQ_TASK_H_
#define  __REQ_TASK_H_

#include "task.h"

namespace sub_framework {

class ReqTask : public Task {
    
public:
    ReqTask(const std::string& task_name);
    ~ReqTask();
    virtual void _run();
    virtual void _call_back();
  
}

}










#endif  //__REQ_TASK_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
