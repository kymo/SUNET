 
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
        FATAL_LOG("Create Socket FD Error!");
        exit(1);
    }
    if (bind(_svr_fd, (struct sockaddr*)&_svr_addr, sizeof(_svr_addr)) == -1) {
        FATAL_LOG("Bind Error!");
        exit(1);
    }
    if (listen(_svr_fd, MAX_CLT_CNT) == -1) {
        FATAL_LOG("Listen Error!");
        exit(1);
    }
    int yes = 1;
    int nRecvBuf= 128 * 1024; //设置为32K
    if (setsockopt(_svr_fd, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)) == -1) {
        FATAL_LOG("Set Receive Buffer Error!");
        exit(1);
    }
    
    if (setsockopt(_svr_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        FATAL_LOG("Set Reuseraddr Error");
        exit(1);
    }
    // 设置为非阻塞
    _set_nonblocking(_svr_fd);
    
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;//设定接受到指定信号后的动作为忽略
    sa.sa_flags = 0;
    if (sigemptyset(&sa.sa_mask) == -1 ||   //初始化信号集为空
            sigaction(SIGPIPE, &sa, 0) == -1) {   //屏蔽SIGPIPE信号
        FATAL_LOG("Failed to ignore SIGPIPE!");
        exit(EXIT_FAILURE);
    }

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
}

int SubServer::_on_accept(int svr_fd) {
    
    struct sockaddr_in _client_addr;
    memset(&_client_addr, 0, sizeof(_client_addr));
    socklen_t clit_len = sizeof(struct sockaddr);
    int new_sock = -1;
    int mx_sock = -1;
    while ((new_sock = accept(svr_fd, (struct sockaddr*)&_client_addr, &clit_len)) > 0) {
        _set_nonblocking(new_sock);
        int nRecvBuf= 128 * 1024; //设置为32K
        if (setsockopt(new_sock, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)) == -1) {
            WARN_LOG("Set Receive Buf Erro!");
            exit(1);
        }
        DEBUG_LOG("New Client Comes[%d]", new_sock);
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
    std::map<int, RECV_DATA*>::iterator it = _read_buf_map.find(clt_fd);
    RECV_DATA* recv_data = it->second;
    do {
        buf_index = 0;
        ret = 0;
        buf_left = 1024;
        ret = recv(clt_fd, recv_data->buf + recv_data->buf_len, buf_left, 0);
        DEBUG_LOG("Receive Client %d data[%d]%d:[%s]", clt_fd, strlen(recv_data->buf), recv_data->buf_len, recv_data->buf);
        if (-1 == ret) {
            if (errno != EAGAIN) {
                DEBUG_LOG("Read Error !");
                break;
            } else {
                break;
            }
        } else if (ret == 0) {
            DEBUG_LOG("Client close socket!");
            return READ_FAIL;
        } else {
            recv_data->buf_len += ret;
            if (recv_data->buf_len + buf_left >= recv_data->buf_cap) {
                DEBUG_LOG("Receive read out !");
                recv_data->resize();
            }
            if (ret == buf_left) {
                continue;
            } else {
                break;    
            }
        }

    } while(true);
    
    // add into task query
    if (recv_data->buf_len > 0) {
        DEBUG_LOG("Add into task queue!");
        recv_data->buf[recv_data->buf_len] = '\0';
        DEBUG_LOG("Recv Buf[%d][%d] :%s", recv_data->buf_len, strlen(recv_data->buf), recv_data->buf);
        SubTask* task = new ReqTask("req_task");
        REQ_TASK_DATA* req_task_data = new REQ_TASK_DATA(clt_fd, recv_data, _event);
        task->_set_task_data((void*)(req_task_data));
        SubTaskMgr::_get_instance()->_add_task(task);
        _read_buf_map.erase(it);
        return READ_OK;
    }
    if (read_out) {
        return READ_ERROR;
    }
    DEBUG_LOG("Read Okay!");
    return READ_OK;
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
