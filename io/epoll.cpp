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
        std::cout << "[Error] epoll create error!" << std::endl;
        return ;
    }
    _svr_fd = srv_fd;
    // add server fd into the fd_set
    _event_add(_svr_fd, EPOLLIN );
    _epl_evt_set = (struct epoll_event*) malloc (sizeof(struct epoll_event) * _max_evt_cnt);
    std::cout << "event init!" << std::endl;
}

void SubEpollEvent::_event_loop() {
    while (true) {
        int max_fd = epoll_wait(_epl_fd, _epl_evt_set, _max_evt_cnt, 1000);
        std::cout << "strat" << std::endl;
        if (-1 == max_fd) {
            std::cout << "[Error] epoll loop error!" << std::endl;
            return ;
        }
        std::cout << "mid" << std::endl;
        for (int fd = 0; fd < max_fd; ++fd) {
            int handler_fd = _epl_evt_set[fd].data.fd;
            std::cout << handler_fd << " " << _svr_fd << " " << (_epl_evt_set[fd].events & EPOLLIN) <<  std::endl;
            if (handler_fd == _svr_fd) {
                int new_fd = _event_accept_callback_proc(_svr_fd);
                _event_add(new_fd, EPOLLIN | EPOLLET);
            } else if (_epl_evt_set[fd].events & EPOLLIN) {
                std::cout << "read" << std::endl;
				int recv_ret = _event_read_callback_proc(handler_fd);
                if (0 == recv_ret) {
                    // client close socket
                    _event_del(handler_fd, EPOLLIN);
                    _event_del(handler_fd, EPOLLOUT);
                    close(handler_fd);
                } else {
                    std::cout << "get it " << std::endl;
                    _event_mod(handler_fd, EPOLLOUT | EPOLLET);
                }
            } else if (_epl_evt_set[fd].events & EPOLLOUT) {
				std::cout << "write" << std::endl;
                char write_buf[128] = "HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nHello World";
				std::cout << "writewrite" << std::endl;
                if (send(handler_fd, write_buf, sizeof(write_buf), 0) <= 0) {
					std::cout << "write not ok" << std::endl;
					close(handler_fd);
                	_event_del(handler_fd, EPOLLOUT | EPOLLET);
				}
                std::cout << "helo" << std::endl;
                _event_del(handler_fd, EPOLLOUT);
				//_event_mod(handler_fd, EPOLLIN | EPOLLET);
				close(handler_fd);
			}
        }
        std::cout << "end" << std::endl;
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
