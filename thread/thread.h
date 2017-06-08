/**
 * @file thread.h
 * @author kymowind@gmail.com
 * @date 2016/10/08 13:45:00
 * @brief 
 *  
 **/



#ifndef  __THREAD_H_
#define  __THREAD_H_

#include <iostream>
#include <vector>
#include <map>
#include <pthread.h>
#include "task.h"
#include "task_mgr.h"

namespace sub_framework {

enum {
    THREAD_NEW = 0,
    THREAD_RUN = 1,
    THREAD_EXIT = -1
};

// SubThreadHandler -- 每一个线程的处理单元
class SubThreadHandler {
public:
    SubThreadHandler () {
        _stoped = false;
    }
    ~SubThreadHandler() {}
    
    int _thread_proc_handler(void* args) {
        // 线程从task 队列中获取一个task,如果获取成功，则执行该task
        // 每一个task都有对应的回调函数
        while (! _stoped) {
            SubTask* task = SubTaskMgr::_get_instance()->_get_task();
            if (NULL != task) {
                // std::cout << "deal with task[ " << task->_task_name << " ]" << std::endl;
                task->_run();
                task->_call_back();
				// std::cout << "delete task 2" << std::endl;
            	delete task;
				task = NULL;
				// std::cout << "delete task 21" << std::endl;
			}
        }
    }
    
    bool _is_stop() {
        return _stoped;
    }
    void _stop() {
        _stoped = true;
    }
public:
    bool _stoped;
};

class SubThread {
private:
    int _thread_stat;
    pthread_t _tid;
    void* _args;
    SubThreadHandler* _handler;
    friend void* thread_proc(void *args);

public:
    SubThread();
    SubThread(SubThreadHandler* handler, void* args);
    ~SubThread();
    int _get_stat();
    void _wait();
    void _stop();
    bool _is_stop();
    int _start();
    void _join();
    void _run();
};

};














#endif  //__THREAD_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
