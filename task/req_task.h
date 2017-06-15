 
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
#include "task_mgr.h"
#include "event.h"
#include "strategy_mgr.h"

namespace sub_framework {

typedef struct req_task_data {
    int _fd;
    RECV_DATA *_data;
    SubEvent* _evt;
    req_task_data() {}
    req_task_data(int fd, RECV_DATA* recv_data, SubEvent* evt) {
        _fd = fd;
        _data = recv_data;
        _evt = evt;
    }
    ~req_task_data() {
        // std::cout << "delete req task data!" << std::endl;
        if (NULL != _data) {
            delete _data;
            _data = NULL;
        }
    }

} REQ_TASK_DATA;

class ReqTask : public SubTask {

public:
    ReqTask(const std::string& task_name);
    virtual ~ReqTask();
    virtual int _run();
    virtual int _call_back();
    virtual void _set_task_data(void* task_data);
};

}










#endif  //__REQ_TASK_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
