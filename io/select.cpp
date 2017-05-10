/**
 * @file select.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/15 16:37:38
 * @brief 
 *  
 **/

#include "select.h"

namespace sub_framework {

void SubSelectEvent::_event_add(int evt_fd, int evt_type)  {
    // FD_ZERO(&_read_set);
    if (evt_type == EVT_READ) {
        FD_SET(evt_fd, &_read_set);
        _clt_sock_vec.push_back(evt_fd);
    } else if (evt_type == EVT_WRITE) {
        std::cout << "Epoll Add it!" << std::endl;
        FD_SET(evt_fd, &_write_set);
    }
}

void SubSelectEvent::_event_init(int srv_fd) {
    FD_ZERO(&_read_set);
    FD_ZERO(&_write_set);
    _svr_fd = srv_fd;
    _max_sock_fd = srv_fd;
    _event_add(srv_fd, EVT_READ);
}

int SubSelectEvent::_event_mod(int evt_fd, int evt_type) {

}

void SubSelectEvent::_event_loop() {
    
    while (true) {
        struct timeval tv = {1, 0};
        FD_ZERO(&_copy_read_set);
        FD_ZERO(&_copy_write_set);

        _copy_write_set = _write_set;
        _copy_read_set = _read_set;
        int ret = select(_max_sock_fd + 1, &_copy_read_set, &_copy_write_set, NULL, &tv);
        if (-1 == ret) {
            std::cout << "Select Error!" << std::endl;
        } else if (0 == ret) {
            std::cout << "Select Time Out!" << std::endl;
        } else {
            // ACCEPT
            if (FD_ISSET(_svr_fd, &_copy_read_set)) { // accpet {
                int new_fd = _event_accept_callback_proc(_svr_fd);
                if (-1 != new_fd) {
                    // _event_add(new_fd, EVT_READ);
                    _max_sock_fd = std::max(_max_sock_fd, new_fd); 
                    // _clt_sock_vec.push_back(new_fd);
                }
                // FD_CLR(_svr_fd, &_read_set);
            }
            //
            std::cout << "begin" << std::endl;
            for (int i = 0; i < _clt_sock_vec.size(); i++) {
                std::cout << "click socket fd " << _clt_sock_vec[i] << std::endl;
                if (-1 == _clt_sock_vec[i] || _clt_sock_vec[i] == _svr_fd) {
                    continue;
                }
                // 当前client fd 是否可读
                if (FD_ISSET(_clt_sock_vec[i], &_copy_read_set)) {
                    // 读函数
                    // std::cout << "sock " << _clt_sock_vec[i] << " read" << std::endl;
                    int recv_ret = _event_read_callback_proc(_clt_sock_vec[i]);
                    FD_CLR(_clt_sock_vec[i], &_read_set);
                    if (recv_ret == 0) {
                        // 从读写集合中剔除该client socket
                        FD_CLR(_clt_sock_vec[i], &_write_set);
                        close(_clt_sock_vec[i]);
                        _clt_sock_vec.erase(_clt_sock_vec.begin() + i);
                        continue;
                    }
                    // _event_add(_clt_sock_vec[i], EVT_WRITE);
                // 当前client socket fd 是否可写
                } else if (FD_ISSET(_clt_sock_vec[i], &_copy_write_set)) {
                    // std::cout << "sock " << _clt_sock_vec[i] << " read" << std::endl;
                    //char write_buf[128] = "HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nHello World";
                    /*std::vector<char*> write_buf_vec;
                    SubEventQueue::_get_instance()->_get_evt_data(_clt_sock_vec[i], write_buf_vec);
                    std::cout << "WRITE CLIENT " << _clt_sock_vec[i] << std::endl;
                    //std::cout << "end it !" << std::endl;
                    if (write_buf_vec.size() == 0) {
                        FD_CLR(_clt_sock_vec[i], &_write_set);
                        FD_CLR(_clt_sock_vec[i], &_read_set);
                        close(_clt_sock_vec[i]);
                        _clt_sock_vec.erase(_clt_sock_vec.begin() + i);
                        continue;
                    }
                    char*write_buf = write_buf_vec[0];
                    */
                    FD_CLR(_clt_sock_vec[i], &_write_set);
                    char write_bufs[128] = "HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nHello World";
                    if (send(_clt_sock_vec[i], write_bufs, sizeof(write_bufs), 0) <= 0) {
                        FD_CLR(_clt_sock_vec[i], &_read_set);
                        close(_clt_sock_vec[i]);
                        _clt_sock_vec.erase(_clt_sock_vec.begin() + i);
                        continue;
                    }
                    _event_add(_clt_sock_vec[i], EVT_READ);

                }
            }

            std::cout << "end" << std::endl;
        }
    }
}

}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
