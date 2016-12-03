 
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
    } else if (evt_type == EVT_WRITE) {
        FD_SET(evt_fd, &_write_set);
    }
}

void SubSelectEvent::_event_init(int srv_fd) {
    FD_ZERO(&_read_set);
    _svr_fd = srv_fd;
    _max_sock_fd = srv_fd;
}

void SubSelectEvent::_event_loop() {
    
    while (true) {
        struct timeval tv = {5, 0};
        FD_ZERO(&_write_set);
        _write_set = _read_set;
        int ret = select(_max_sock_fd + 1, &_write_set, NULL, NULL, &tv);
        if (-1 == ret) {
            std::cout << "Select Error!" << std::endl;
        } else if (0 == ret) {
            std::cout << "Select Time Out!" << std::endl;
        } else {
            // ACCEPT
            if (FD_ISSET(_svr_fd, &_write_set)) { // accpet {
                int new_fd = _event_accept_callback_proc(_svr_fd);
                if (-1 == new_fd) {
                    std::cout << "Select Accpet Error!" << std::endl;
                } else {
                    std::cout << "Accept from client, sock fd:" << new_fd << std::endl;
                    _event_add(new_fd, EVT_READ);
                    _max_sock_fd = std::max(_max_sock_fd, new_fd); 
                    _clt_sock_vec.push_back(new_fd);
                }
            }

            // 
            for (int i = 0; i < _clt_sock_vec.size(); i++) {
                std::cout << _clt_sock_vec[i] << " ..." << std::endl;
                if (_clt_sock_vec[i] != -1 && FD_ISSET(_clt_sock_vec[i], &_write_set)) {
                    std::cout << "RECV ONE!" << std::endl;
                    int recv_ret = _event_read_callback_proc(_clt_sock_vec[i]);
                    if (recv_ret == 0) {
                        FD_CLR(_clt_sock_vec[i], &_read_set);
                        close(_clt_sock_vec[i]);
                        _clt_sock_vec.erase(_clt_sock_vec.begin() + i);
                        continue;
                    }

                    // write back 
                    // TODO add write _event into another fd set
                    //
                    char write_buf[128] = "HTTP/1.1 200 OK\r\nContent-Length: 11\r\n\r\nHello World";
                    if (send(_clt_sock_vec[i], write_buf, sizeof(write_buf), 0) <= 0) {
                        FD_CLR(_clt_sock_vec[i], &_read_set);
                        close(_clt_sock_vec[i]);
                        _clt_sock_vec.erase(_clt_sock_vec.begin() + i);
                        continue;
                    }
                }
            }
        }
    }
}


}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
