 /*
 * @file epoll.h
 * @author kymowind@gmail.com
 * @date 2017/03/22 20:50:59
 * @brief 
 *  
 **/

#ifndef  __EPOLL_H_
#define  __EPOLL_H_

#include <iostream>
#include <vector>
#include "event.h"
#include "errno.h"
#include <stdio.h>
#include <stdlib.h>

namespace sub_framework {

#define MAX_CLT_CNT 1024

class SubEpollEvent : public SubEvent {
    
private:
    std::vector<int> _clt_soct_vec;
    int _svr_fd;
    int _max_sock_fd;
    int _epl_fd;
    epoll_event _epl_evt;
    int _max_evt_cnt;
    struct epoll_event* _epl_evt_set;
public:
	SubEpollEvent() {
        _type = EPOLL;
    }
	~SubEpollEvent() {
		if (_epl_evt_set != NULL) {
			free(_epl_evt_set);
			_epl_evt_set = NULL;
		}
	}
    virtual void _event_init(int srv_fd);
    virtual void _event_loop();
    virtual void _event_add(int evt_fd, int evt_type);
    virtual int _event_mod(int evt_fd, int evt_type);
    int _event_del(int evt_fd, int evt_type);
};

}










#endif  //__EPOLL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
