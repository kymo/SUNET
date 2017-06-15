 
/**
 * @file server.h
 * @author kymowind@gmail.com
 * @date 2016/10/17 10:08:00
 * @brief 
 *  
 **/

#ifndef  __SERVER_H_
#define  __SERVER_H_
#include <stdio.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "select.h"
#include "event.h"
#include "epoll.h"
#include <fcntl.h>
#include "define.h"
#include "task.h"
#include "req_task.h"
#include "task_mgr.h"
#include "thread_pool.h"
#include "sub_log.h"
#include <signal.h>

namespace sub_framework {

enum TASKTYPE {
};

#define REQ_TASK "req_task"
#define PARSER_TASK "parser_task"

static SubEvent* _event;
static std::map<int, RECV_DATA*> _read_buf_map;

class SubServer {
   
SINGLETN(Server);

private:
    struct sockaddr_in _svr_addr;
    int _svr_fd;
    std::vector<int> _clt_sock_vec;

public:
    void _run(int port);
    void _init_sock(int port);
    void _init_evt(int evt_type);
    static int _on_http_read(int clt_fd);    // http protocal
    static int _on_stp_read(int clt_fd); // search transfer protocal
    static int _on_accept(int svr_fd);
    static void _set_nonblocking(int sock_fd);

};

}

#endif 
