/**
 * @file epoll.cpp
 * @author kymowind@gmail.com
 * @date 2017/03/22 20:53:38
 * @brief 
 *  
 **/

#include "epoll.h"

namespace sub_framework {

void SubEpollEvent::_event_init(int srv_fd) {

    _max_evt_cnt = 1024;
    _epl_fd = epoll_create(_max_evt_cnt);
    if (-1 == _epl_fd) {
        FATAL_LOG("Epoll Create Error!");
        return ;
    }
    _svr_fd = srv_fd;
    // add server fd into the fd_set
    _event_add(_svr_fd, EPOLLIN | EPOLLET);
    _epl_evt_set = (struct epoll_event*) malloc (sizeof(struct epoll_event) * _max_evt_cnt);
}

void SubEpollEvent::_event_loop() {
    while (true) {
		DEBUG_LOG("Epoll Wait!");
        int max_fd = epoll_wait(_epl_fd, _epl_evt_set, _max_evt_cnt, 1000);
        if (-1 == max_fd) {
            WARN_LOG("Epoll Loop Error!");
            continue;
        }
        for (int fd = 0; fd < max_fd; ++fd) {
            int handler_fd = _epl_evt_set[fd].data.fd;
            if (handler_fd == _svr_fd) {
                _event_accept_callback_proc(_svr_fd);
            	continue;
			} 
			
            if (_epl_evt_set[fd].events & EPOLLIN) {
				int recv_ret = _event_read_callback_proc(handler_fd);
                if (0 == recv_ret) {
					DEBUG_LOG("client close socket!");
                    close(handler_fd);
                    _event_del(handler_fd, EPOLLIN);
                } 
            } else if (_epl_evt_set[fd].events & EPOLLOUT) {
                std::vector<char*> write_buf_vec;
                SubEventQueue::_get_instance()->_get_evt_data(handler_fd, write_buf_vec);
                if (write_buf_vec.size() == 0) {
                    WARN_LOG("Get Write Buffer Empty!");
					close(handler_fd);
                    _event_del(handler_fd, EPOLLIN);
                	_event_del(handler_fd, EPOLLOUT);
                    continue;
                }
                bool tag = false;
                for (int i = 0; i < write_buf_vec.size(); i++) {
                    char* write_buf = write_buf_vec[0];
                    if (send(handler_fd, write_buf, strlen(write_buf), 0) <= 0) {
                        tag = true;
                        break;
                    }
                }
                if (tag) {
                    close(handler_fd);
                    _event_del(handler_fd, EPOLLIN);
                }
                _event_del(handler_fd, EPOLLOUT);
			}
        }
    }
}

void SubEpollEvent::_event_add(int evt_fd, int evt_type) {
    _epl_evt.events = evt_type;
    _epl_evt.data.fd = evt_fd;
    epoll_ctl(_epl_fd, EPOLL_CTL_ADD, evt_fd, &_epl_evt);
}

int SubEpollEvent::_event_mod(int evt_fd, int evt_type) {
    _epl_evt.events = evt_type;
    _epl_evt.data.fd = evt_fd;
    epoll_ctl(_epl_fd, EPOLL_CTL_MOD, evt_fd, &_epl_evt);
}

int SubEpollEvent::_event_del(int evt_fd, int evt_type) {
    _epl_evt.events = evt_type;
    _epl_evt.data.fd = evt_fd;
    epoll_ctl(_epl_fd, EPOLL_CTL_DEL, evt_fd, &_epl_evt);
}

}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
