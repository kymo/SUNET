 
/**
 * @file event.h
 * @author kymowind@gmail.com
 * @date 2016/10/14 20:17:42
 * @brief 
 *  
 **/




#ifndef  __EVENT_H_
#define  __EVENT_H_

namespace sub_framework {

typedef int (*EVENT_CALL_BACK_PROC)(int);
class SubEvent {
    
private:
    int _ev_fd;
public:
    EVENT_CALL_BACK_PROC _event_read_callback_proc;
    EVENT_CALL_BACK_PROC _event_accept_callback_proc;
    
    void _set_read_callback_proc(EVENT_CALL_BACK_PROC event_read_callback_proc) {
        _event_read_callback_proc = event_read_callback_proc;
    }
    void _set_accept_callback_proc(EVENT_CALL_BACK_PROC event_accept_callback_proc) {
        _event_accept_callback_proc = event_accept_callback_proc;
    }
    
    virtual void _event_init(int src_fd) = 0;
    virtual void _event_loop() = 0;
    virtual void _event_add(int evt_fd, int evt_type) = 0;
};

}















#endif  //__EVENT_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
