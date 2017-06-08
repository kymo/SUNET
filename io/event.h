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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct recv_buf {
	char *buf;
	int buf_len;
	int buf_cap;
	recv_buf() {
		buf = new char[2048];
        memset(buf, '\0', sizeof(buf));
		buf_len = 0;
		buf_cap = 2048;
	}
	~recv_buf() {
		//std::cout << "delte recv buf !" << std::endl;
		if (NULL != buf) {
			delete buf;
			buf = NULL;
		}
	}
	void resize() {
        std::cout << "resize now!" << std::endl;
		char* new_buf = new char[buf_cap * 2];
		memcpy(new_buf, buf, buf_len);
		buf_cap *= 2;
		delete buf;
		buf = new_buf;
	}
} RECV_DATA;

class SubEventQueue {
    
SINGLETN(EventQueue);    

private:
    pthread_mutex_t _set_mutex;
    pthread_mutex_t _get_mutex;
    std::map<int, std::vector<char*> > _data_buf;

public:
    void _init() {
        
        pthread_mutex_init(&_set_mutex, NULL);
        pthread_mutex_init(&_get_mutex, NULL);
    }

    void _set_evt_data(int fd, char* buf_ptr) {
        std::cout << "----------Set fd " << fd << "; Data:" << std::endl;
        std::cout << buf_ptr << std::endl;
        pthread_mutex_lock(&_set_mutex);
        if (_data_buf.find(fd) == _data_buf.end()) {
            _data_buf[fd] = std::vector<char*>();
        }
        char *write_buf = new char[64];
	    char *json = "hello";
        sprintf(write_buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", strlen(json), json); 
        _data_buf[fd].push_back(write_buf);
        pthread_mutex_unlock(&_set_mutex);
        std::cout << "----------End" << std::endl;
    }
    
    void _get_evt_data(int fd, std::vector<char*>& ret) {
        std::cout << "Get fd " << fd << "; Data" << std::endl;
        if (_data_buf.find(fd) == _data_buf.end()) {
            return ;
        }
        ret = _data_buf[fd];
        for (int i = 0; i < ret.size(); i++) {
            std::cout << ret[i] << std::endl;
        }
        std::cout << "Gete End" << std::endl;
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
