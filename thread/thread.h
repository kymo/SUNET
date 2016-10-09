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

namespace sub_framework {

enum {
    THREAD_NEW = 0,
    THREAD_RUN = 1,
    THREAD_EXIT = -1
};

// thread handler
class SubThreadHandler {
public:
    SubThreadHandler () {
        _stoped = false;
    }
    ~SubThreadHandler() {}
    virtual int _thread_proc_handler(void* args) = 0;
    bool _is_stop() {
        return _stoped;
    }
    void _stop() {
        _stoped = true;
    }
private:
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
