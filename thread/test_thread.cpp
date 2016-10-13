 
/**
 * @file main.cpp
 * @author wangxiaofeng04(com@baidu.com)
 * @date 2016/10/08 22:19:53
 * @brief 
 *  
 **/

#include <iostream>
#include "thread.h"
#include "thread_pool.h"
#include "task_mgr.h"
#include "req_task.h"
#include "task.h"

using namespace std;


class EchoTaskHandler : public sub_framework::SubThreadHandler {
public:
    int _thread_proc_handler(void *args) {
        int *v = (int*)args;
        std::cout << "this is the frame work of thread !, number:" << *v << std::endl;
    }
};

class ReqTaskHandler : public sub_framework::SubThreadHandler {
public:
    int _thread_proc_handler(void *args) {
        // add task
        for (int i = 0; i < 10; i++) {
            std::cout << "create req_task " << i << std::endl;
            sub_framework::SubTask* task = new sub_framework::ReqTask("req_task"); 
            task->_set_task_data(args);
            sub_framework::SubTaskMgr::_get_instance()->_add_task(task);
        }
    }
};

void test_sig_thread() {
    EchoTaskHandler* echo_handler = new EchoTaskHandler();
    int t = 0;
    sub_framework::SubThread* sub_thread = new sub_framework::SubThread(echo_handler, (void*)&t);
    sub_thread->_start();
    sub_thread->_join();
}

void test_mul_thread() {
    EchoTaskHandler* echo_handler = new EchoTaskHandler();
    for (int i = 0; i < 10; i ++) {
        sub_framework::SubThread* sub_thread = new sub_framework::SubThread(echo_handler, (void*)&i);
        sub_thread->_start();
        sub_thread->_join();
    }
}


int req_call_back(void *a, void *b) {
    int *aa = (int*)a;
    *aa += 10000;
    std::cout << "THIS IS " <<  *aa << std::endl;
    int*bb = (int*)b;
    *bb = *aa;
    std::cout << *(int*)b << std::endl;
    //std::cout << "i am callback!" << std::endl;
}

void test_thread_pool() {
    sub_framework::SubTaskMgr::_get_instance()->_init();
    sub_framework::SubThreadPool* thread_pool = sub_framework::SubThreadPool::_get_instance();
    thread_pool->_init();
    thread_pool->_set_thread_cnt(30);
    thread_pool->_start();

    // add req_call_back
    sub_framework::SubTaskMgr::_get_instance()->_set_call_back_proc("req_task", req_call_back);
    sub_framework::SubThreadHandler* req_handler = new ReqTaskHandler();
    int fucn = 1000;
    sub_framework::SubThread* sub_thread = new sub_framework::SubThread(req_handler, (void*)(&fucn));
    sub_thread->_start();
    sub_thread->_join();
    while (1);
}

int main() {
    //test_sig_thread();
    //test_mul_thread();
    test_thread_pool();
    return 0;
}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
