 
/**
 * @file thread_pool.h
 * @author kymowind@gmail.com
 * @date 2016/10/08 17:09:01
 * @brief 
 *  
 **/




#ifndef  __THREAD_POOL_H_
#define  __THREAD_POOL_H_


#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include "thread.h"
#include "task_mgr.h"
#include "config.h"

namespace sub_framework {
class SubThreadPool {

public:

    /**
     * 析构函数
     */
    virtual ~SubThreadPool();

    /**
     * 设置线程函数
     */
    void _set_thread_cnt(int thread_cnt);

    /**
     * 创建_thread_cnt个线程
     */
    int _start();

    /**
     * stop
     */
    void _stop();

    /**
     * wait
     */
    void _wait();

    /**
     * get instalce
     */
    static SubThreadPool* _get_instance() {
        if (NULL == _sub_thread_pool_instance) {
            _sub_thread_pool_instance = new SubThreadPool();
        }
        return _sub_thread_pool_instance;
    }

    /**
     * init the data
     */
    void _init();

private:
    SubThreadPool();
    std::vector<SubThread*> _threads;
    static SubThreadPool* _sub_thread_pool_instance;
    int _threads_cnt;
    bool _stoped;

};

}












#endif  //__THREAD_POOL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
