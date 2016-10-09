 
/**
 * @file src/thread_pool.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/08 17:13:22
 * @brief 
 *  
 **/

#include "thread_pool.h"

namespace sub_framework {
    

SubThreadPool::SubThreadPool(int threads_cnt) {
    _threads_cnt = threads_cnt;
    _stoped = false;
}


SubThreadPool::~SubThreadPool() {
    
}

void SubThreadPool::_set_thread_cnt(int thread_cnt) {
    
    if (_threads.size() != 0) {
        std::cout << "Thread Pool is running!Set thread count is not allowed!" << std::endl;
        return ;
    }
    _threads_cnt = thread_cnt;
}

int SubThreadPool::_start() {
    if (_threads.size() != 0 || _threads_cnt < 1) {
        std::cout << "start failed!" << _threads.size() << "," << _threads_cnt << std::endl;
        return 0;
    }
    for (int i = 0; i < _threads_cnt; i++) {
         
    }

    return 1;
}

}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
