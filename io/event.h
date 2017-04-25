/**
 * @file event.h
 * @author kymowind@gmail.com
 * @date 2016/10/14 20:17:42
 * @brief 
 *  
 **/

#ifndef  __EVENT_H_
#define  __EVENT_H_

#include <map>
#include <vector>
#include <iostream>
#include <sys/time.h>
#include "errno.h"
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "define.h"

namespace sub_framework {

typedef int (*EVENT_CALL_BACK_PROC)(int);

enum EVENT_TYPE {
    EVT_READ = 0,
    EVT_WRITE = 1
};

enum IOTYPE {
    SELECT = 0,
    EPOLL
};

class SubEventQueue {
    
SINGLETN(EventQueue);    

private:
    std::map<int, std::vector<char*> > _data_buf;

public:
    
    void _set_evt_data(int fd, char* buf_ptr) {
        std::cout << "set " << fd << buf_ptr << std::endl;
        if (_data_buf.find(fd) == _data_buf.end()) {
            _data_buf[fd] = std::vector<char*>();
        }
        std::cout << "set okay!" << std::endl;
        _data_buf[fd].push_back(buf_ptr);
    }
    
    void _get_evt_data(int fd, std::vector<char*>& ret) {
        std::cout << fd << " " << _data_buf.size() << std::endl;
        if (_data_buf.find(fd) == _data_buf.end()) {
            return ;
        }
        std::cout << "gete" << std::endl;
        ret = _data_buf[fd];
    }

};

class SubEvent {
    
private:
    int _ev_fd;

public:
    int _type;
    EVENT_CALL_BACK_PROC _event_read_callback_proc;
    EVENT_CALL_BACK_PROC _event_accept_callback_proc;
    
    // read 回调函数
    void _set_read_callback_proc(EVENT_CALL_BACK_PROC event_read_callback_proc) {
        _event_read_callback_proc = event_read_callback_proc;
    }

    // accept 回调函数
    void _set_accept_callback_proc(EVENT_CALL_BACK_PROC event_accept_callback_proc) {
        _event_accept_callback_proc = event_accept_callback_proc;
    }
    
    virtual void _event_init(int src_fd) = 0;
    virtual void _event_loop() = 0;
    virtual void _event_add(int evt_fd, int evt_type) = 0;
    virtual int _event_mod(int evt_fd, int evt_type) = 0;
};

}

#endif  //__EVENT_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
