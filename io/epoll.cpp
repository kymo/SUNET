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

    _max_evt_cnt = 10240;
    _epl_fd = epoll_create(_max_evt_cnt);
    if (-1 == _epl_fd) {
        FATAL_LOG("Epoll Create Error!");
        return ;
    }
    _svr_fd = srv_fd;
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
                if (READ_OK != recv_ret) {
                    DEBUG_LOG("Recv Info %d", recv_ret);
                    _event_del(handler_fd, EPOLLIN | EPOLLET);
                    _event_del(handler_fd, EPOLLOUT | EPOLLET);
                    close(handler_fd);
                } 
            } else if (_epl_evt_set[fd].events & EPOLLOUT) {
                std::vector<char*> write_buf_vec;
                //SubEventQueue::_get_instance()->_get_evt_data(handler_fd, write_buf_vec);
                SUB_EPOLL_OUT_ENV* out_env = (SUB_EPOLL_OUT_ENV*) _epl_evt_set[fd].data.ptr;
                DEBUG_LOG("Write Back [%d]", out_env->_fd);
                write_buf_vec.push_back(out_env->_buf);
                // write_buf_vec.push_back((char*)_epl_evt_set[fd].data.ptr);
                if (write_buf_vec.size() == 0) {
                    WARN_LOG("Get Write Buffer Empty!");
                    _event_mod(handler_fd, EPOLLIN | EPOLLET);
                    continue;
                }
                bool tag = false;
                for (int i = 0; i < write_buf_vec.size(); i++) {
                    char* write_buf = write_buf_vec[i];
                    if (write_buf == NULL || strlen(write_buf) == 0) {
                        continue;
                    }

                    int left_buf_size = strlen(write_buf);
                    int n = left_buf_size;
                    while (n > 0) {
                        int nwrite = send(out_env->_fd, write_buf + left_buf_size - n, n, 0);
                        if (nwrite < n) {
                            if (nwrite == -1 && (errno != EAGAIN)) {
                                FATAL_LOG("Write Error!");
                            }
                            break;
                        }
                        n -= nwrite;
                    }
                }
                _event_mod(out_env->_fd, EPOLLIN | EPOLLET);
                if (out_env != NULL) {
                    delete out_env;
                    out_env = NULL;
                }
                DEBUG_LOG("[%d] Write Back Okay!", handler_fd);
            }
        }
    }
}

void SubEpollEvent::_event_add(int evt_fd, int evt_type) {
    epoll_event _epl_evt;
    _epl_evt.events = evt_type;
    _epl_evt.data.fd = evt_fd;
    epoll_ctl(_epl_fd, EPOLL_CTL_ADD, evt_fd, &_epl_evt);
}

int SubEpollEvent::_event_mod(int evt_fd, int evt_type, void* write_buf) {
    epoll_event _epl_evt;
    _epl_evt.events = evt_type;
    _epl_evt.data.fd = evt_fd;
    _epl_evt.data.ptr = write_buf;
    epoll_ctl(_epl_fd, EPOLL_CTL_MOD, evt_fd, &_epl_evt);
}

int SubEpollEvent::_event_mod(int evt_fd, int evt_type) {
    epoll_event _epl_evt;
    _epl_evt.events = evt_type;
    _epl_evt.data.fd = evt_fd;
    epoll_ctl(_epl_fd, EPOLL_CTL_MOD, evt_fd, &_epl_evt);
}

int SubEpollEvent::_event_del(int evt_fd, int evt_type) {
    epoll_event _epl_evt;
    _epl_evt.events = evt_type;
    _epl_evt.data.fd = evt_fd;
    epoll_ctl(_epl_fd, EPOLL_CTL_DEL, evt_fd, &_epl_evt);
}

}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
