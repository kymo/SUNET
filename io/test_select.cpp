 
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
    int _on_read(int clt_fd);
    int _on_accept(int svr_fd);

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
}

void SubServer::_init_evt(int evt_type) {
    if (evt_type == SELECT) {
        _event = new SubSelectEvent();
    }
}

int on_accept(int svr_fd) {
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t clit_len = sizeof(struct sockaddr);
    int new_sock = accept(svr_fd, (struct sockaddr*)&client_addr, &clit_len);
    // TODO set nonblocking
    return new_sock;
}


int on_read(int clt_fd) {

    char buf[65536];
    char recv_buf[1024];
    int ret = 0;
    int ret_tot = 0;
    while (ret >= 0) {
        ret = recv(clt_fd, recv_buf, sizeof(recv_buf), 0);
        if (ret <= 0) {
            break;
        } {
            memcpy(buf, recv_buf, sizeof(recv_buf));
            ret_tot += ret;
        }
    }
    std::cout << "Get it from client:%s" << buf << std::endl;
    // read wanle
    return ret_tot;
}

void SubServer::_run() {
    _init_sock(8888);
    _init_evt(SELECT);
    
    std::cout << _svr_fd << std::endl;
    _event->_event_init(_svr_fd);
     // set call_back func
    _event->_set_read_callback_proc(on_read);
    _event->_set_accept_callback_proc(on_accept);
    _event->_event_loop();
}





INIT_SINGLE(Server);

int main() {
    SubServer*svr = SubServer::_get_instance();
    svr->_run();
    return 0;
}









/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
