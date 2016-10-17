 
/**
 * @file test_select.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/15 21:32:27
 * @brief 
 *  
 **/


#include  <stdio.h>
#include  <iostream>
#include  <unistd.h>
#include  <fcntl.h>
#include  <errno.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <sys/epoll.h>
#include  <netinet/in.h>
#include  <arpa/inet.h>
#include "select.h"
#include "event.h"
#include <fcntl.h>

using namespace sub_framework;
enum {
    SELECT = 0,
    EPOLL
};

#define SINGLETN(_class) \
    private: \
        static Sub##_class* _sub_##_class##_instance;\
        Sub##_class() {} \
    public: \
        static Sub##_class* _get_instance() { \
            if (NULL == _sub_##_class##_instance) { \
                _sub_##_class##_instance = new Sub##_class();\
            } \
            return _sub_##_class##_instance; \
        } 

#define INIT_SINGLE(_class) \
    Sub##_class* Sub##_class::_sub_##_class##_instance = NULL; 

class SubServer {
   
SINGLETN(Server);

private:
    struct sockaddr_in _svr_addr;
    int _svr_fd;
    std::vector<int> _clt_sock_vec;
    SubEvent* _event;

public:
    void _run();
    void _init_sock(int port);
    void _init_evt(int evt_type);

    static int _on_read(int clt_fd);
    static int _on_accept(int svr_fd);
    static void _set_nonblocking(int sock_fd);

};

void SubServer::_init_sock(int port) {
    memset(&_svr_addr, 0, sizeof(&_svr_addr));
    _svr_addr.sin_family = AF_INET;
    _svr_addr.sin_port = htons(port);
    _svr_addr.sin_addr.s_addr = htons(INADDR_ANY);

    _svr_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == _svr_fd) {
        std::cout << "Create Socket FD Error!" << std::endl;
        exit(1);
    }
    if (bind(_svr_fd, (struct sockaddr*)&_svr_addr, sizeof(_svr_addr)) == -1) {
        std::cout << "Bind Error!" << std::endl;
        exit(1);
    }
    if (listen(_svr_fd, MAX_CLT_CNT) == -1) {
        std::cout << "Listen Error!" << std::endl;
        exit(1);
    }
    int yes = 1;
    if (setsockopt(_svr_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    _set_nonblocking(_svr_fd);

    // 设置为非阻塞
}

void SubServer::_init_evt(int evt_type) {
    if (evt_type == SELECT) {
        _event = new SubSelectEvent();
    }
}


void SubServer::_set_nonblocking(int sock_fd) {
    int opts = fcntl(sock_fd, F_GETFL);  
    opts = (opts | O_NONBLOCK); 
    int ret = fcntl(sock_fd, F_SETFL, opts) ;
    std::cout << "set nonblocking:" << ret << std::endl;
}

int SubServer::_on_accept(int svr_fd) {
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t clit_len = sizeof(struct sockaddr);
    int new_sock = accept(svr_fd, (struct sockaddr*)&client_addr, &clit_len);
    // TODO set nonblocking
    _set_nonblocking(new_sock);
    return new_sock;
}

int SubServer::_on_read(int clt_fd) {
    std::cout << "on read callback proc!" << std::endl;
    char buf[65536];
    buf[0] = '\0';
    char recv_buf[65536];
    int ret = 0;
    int ret_tot = 0;
    std::cout << "Begin to recv:" << std::endl;
    int buf_index = 0;
    int buf_left = 65536;
    do {
        while ((ret = recv(clt_fd, recv_buf + buf_index, buf_left, 0)) > 0) {
            buf_index += ret;
            buf_left -= ret;
            if (0 == buf_left) {
                break;
            }
        }
        if (0 == ret) {
            // client close socket
            close(clt_fd);
            return 0;
        }
        if (-1 == ret && errno != EAGAIN) {
            // read error!
            std::cout << "read erro!" << std::endl;
            return 0;
        }
        if (errno == EAGAIN) {
            break;
        }
    } while(true);


    /*
    while (ret >= 0) {
        ret = recv(clt_fd, recv_buf, sizeof(recv_buf), 0);
        std::cout << "Recv " << ret<< std::endl;
        if (ret <= 0) {
            return ret;
        } else {
            memcpy(buf, recv_buf, sizeof(recv_buf));
            ret_tot += ret;
            if (ret < 1024) {
                break;
            }
        }
    }   
    */
    std::cout << "Get it from client:%s" << recv_buf << std::endl;
    // read wanle
    return 1;
}

void SubServer::_run() {
    _init_sock(8888);
    _init_evt(SELECT);
    _event->_event_init(_svr_fd);
    _event->_event_add(_svr_fd, 0); 
    // set call_back func
    _event->_set_read_callback_proc(&SubServer::_on_read);
    _event->_set_accept_callback_proc(&SubServer::_on_accept);
    // event loop
    _event->_event_loop();
}





INIT_SINGLE(Server);

int main() {
    SubServer*svr = SubServer::_get_instance();
    svr->_run();
    return 0;
}









/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */