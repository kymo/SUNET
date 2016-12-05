 
/**
 * @file select.h
 * @author kymowind@gmail.com
 * @date 2016/10/15 16:40:42
 * @brief 
 *  
 **/


#ifndef  __SELECT_H_
#define  __SELECT_H_

#include <iostream>
#include <vector>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include  <netinet/in.h>
#include  <arpa/inet.h>
#include "event.h"
#include "errno.h"

namespace sub_framework {

#define MAX_CLT_CNT 1024

class SubSelectEvent : public SubEvent {

private:
    fd_set _read_set;
    fd_set _write_set;
    fd_set _copy_read_set;
    fd_set _copy_write_set;
    int _svr_fd;
    std::vector<int> _clt_sock_vec;
    int _max_sock_fd;

public:
    
    virtual void _event_init(int srv_fd);
    virtual void _event_loop();
    virtual void _event_add(int evt_fd, int evt_type);

};


}














#endif  //__SELECT_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
