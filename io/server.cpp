 
/**
 * @file server.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/17 10:09:27
 * @brief 
 *  
 **/


#include "server.h"


namespace sub_framework {

INIT_SINGLE(Server);

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
    // 设置为非阻塞
    _set_nonblocking(_svr_fd);
}

void SubServer::_init_evt(int evt_type) {
    if (evt_type == SELECT) {
        _event = new SubSelectEvent();
    } else if (evt_type == EPOLL) {
        _event = new SubEpollEvent();
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
        
        if (errno == EWOULDBLOCK) {
            std::cout << "READ NONBLOCKING" << std::endl;
        }

        std::cout << "READ ret " << ret << std::endl;
        if (0 == ret) {
            // client close socket
            std::cout << "client close socket!" << std::endl;
            close(clt_fd);
            return 0;
        }
        if (-1 == ret && errno != EAGAIN) {
            // read error!
            std::cout << "read erro!" << std::endl;
            return 0;
        }
        if (errno == EAGAIN) {
            std::cout << "read again!" << std::endl;
            break;
        }
    } while(true);

    std::cout << "Get it from client:%s" << recv_buf << std::endl;
    // read wanle
    // addinto task query
    SubTask* task = new ReqTask("req_task");
    task->_set_task_data((void*)recv_buf);
    SubTaskMgr::_get_instance()->_add_task(task);
    return 1;
}

void SubServer::_run() {
    // 初始化socket
    _init_sock(9999);
    // init io model
    // _init_evt(SELECT);
    _init_evt(EPOLL);
    _event->_event_init(_svr_fd);
    _event->_set_read_callback_proc(&SubServer::_on_read);
    _event->_set_accept_callback_proc(&SubServer::_on_accept);
    // event loop
    _event->_event_loop();

}

}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
