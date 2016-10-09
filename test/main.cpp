 
/**
 * @file main.cpp
 * @author wangxiaofeng04(com@baidu.com)
 * @date 2016/10/08 22:19:53
 * @brief 
 *  
 **/

#include <iostream>
#include "thread.h"

using namespace std;


class EchoTaskHandler : public sub_framework::SubThreadHandler {

public:
    int _thread_proc_handler() {
        std::cout << "this is the frame work of thread!";
    }
};


void test_sig_thread() {

    EchoTaskHandler* echo_handler = new EchoTaskHandler();
    sub_framework::SubThread* sub_thread = new sub_framework::SubThread(echo_handler);
    sub_thread->_start();
    sub_thread->_join();

}

int main() {
    test_sig_thread();
    return 0;
}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
