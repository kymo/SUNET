 
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
INIT_SINGLE(EventQueue);

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
    int nRecvBuf= 128 * 1024; //设置为32K
    if (setsockopt(_svr_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)) == -1) {
        perror("set receive buf error!");
        exit(1);
    }
    
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
    
    struct sockaddr_in _client_addr;
    memset(&_client_addr, 0, sizeof(_client_addr));
    socklen_t clit_len = sizeof(struct sockaddr);
    int new_sock = -1;
    int mx_sock = -1;
    // new_sock = accept(svr_fd, (struct sockaddr*)&_client_addr, &clit_len);
    while ((new_sock = accept(svr_fd, (struct sockaddr*)&_client_addr, &clit_len)) > 0) {
        _set_nonblocking(new_sock);
        int nRecvBuf= 128 * 1024; //设置为32K
        if (setsockopt(new_sock, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)) == -1) {
            perror("set receive buf error!");
            exit(1);
        }
        std::cout << "New Client " << new_sock << std::endl;
        if (_event->_type == SELECT) {
            _event->_event_add(new_sock, EVT_READ);
        } else if (_event->_type == EPOLL) {
            _event->_event_add(new_sock, EPOLLIN | EPOLLET);
        }
        mx_sock = std::max(new_sock, mx_sock);
    }
    return mx_sock;
}

int SubServer::_on_http_read(int clt_fd) {
    
    int ret = 0;
    int ret_tot = 0;
    int buf_index = 0;
    int buf_left = 1024;
    int read_out = 0;
    if (_read_buf_map.find(clt_fd) == _read_buf_map.end()) {
        _read_buf_map[clt_fd] = new RECV_DATA();
    }
    RECV_DATA* recv_data = _read_buf_map[clt_fd];
    std::cout << "FIRST" << std::endl;
    do {
        
        std::cout << "CLIENT:" << clt_fd << "-" << strlen(recv_data->buf) << "[" << recv_data->buf << "]" << std::endl;
        
        ret = recv(clt_fd, recv_data->buf + recv_data->buf_len, buf_left, 0);

        if (recv_data->buf_len > 2 && recv_data->buf[recv_data->buf_len - 1] == '\n' && 
            recv_data->buf[recv_data->buf_len - 2] == '\r') {
            read_out = 1;
            break;
        }
        if (-1 == ret) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            } else {
                read_out = 1;
                break;
            }
        } else if (ret == 0) {
            return 0;
        } else {
            recv_data->buf_len += ret;
            if (recv_data->buf_len + buf_left >= recv_data->buf_cap) {
                recv_data->resize();
            }
        }
    } while(true);
    
    // add into task query
    if (read_out) {
        std::cout << "Add into task queue!" << std::endl;
        std::cout << "-----begin------" << std::endl;
        std::cout << strlen(recv_data->buf) << std::endl;
        std::cout << recv_data->buf << std::endl;
        std::cout << "-----end------" << std::endl;
        recv_data->buf[recv_data->buf_len] = '\0';
        SubTask* task = new ReqTask("req_task");
        
        REQ_TASK_DATA* req_task_data = new REQ_TASK_DATA(clt_fd, recv_data, _event);
        task->_set_task_data((void*)(req_task_data));
        SubTaskMgr::_get_instance()->_add_task(task);
        std::map<int, RECV_DATA*>::iterator it = _read_buf_map.find(clt_fd);
        if (it != _read_buf_map.end()) {
            std::cout << "Erase IT" << std::endl;
            _read_buf_map.erase(it);
        }    
    }
    return 1;
}

void SubServer::_run(int port) {
    // 初始化socket
    _init_sock(port);
    // init io model
    // _init_evt(SELECT);
    _init_evt(EPOLL);
    _event->_event_init(_svr_fd);
    _event->_set_read_callback_proc(&SubServer::_on_http_read);
    _event->_set_accept_callback_proc(&SubServer::_on_accept);
    // event loop
    _event->_event_loop();
}

}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
