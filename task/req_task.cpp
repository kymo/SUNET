    
/**
 * @file req_task.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/10 09:47:38
 * @brief 
 *  
 **/

#include "req_task.h"
#include "http_parse.h"
#include "json/json.h"
namespace sub_framework {

ReqTask::ReqTask(const std::string& task_name) {
    _task_name = task_name;
}

ReqTask::~ReqTask() {
	if (NULL != _task_ret) {
		free(_task_ret);
		_task_ret = NULL;
	}
	if (NULL != _task_data) {
        delete (REQ_TASK_DATA*) _task_data;
		_task_data = NULL;
	}
}

void ReqTask::_set_task_data(void *task_data) {
    _task_data = task_data;
    _task_ret = (void*)(malloc(sizeof(char) * 65536));
}

int ReqTask::_run() {
	CALL_BACK_PROC call_back_proc = SubTaskMgr::_get_instance()->_get_call_back_proc(_task_name);
    if (NULL == call_back_proc) {
		WARN_LOG("Call Back Proc NULL!");
        return 0;
    }
	// 解析http
	Request request;
    HttpParser http;
    //char* buf = (char*) _task_data;
    REQ_TASK_DATA *req_task_data = (REQ_TASK_DATA *)_task_data;
    if (req_task_data == NULL) {
        WARN_LOG("Req Task Data is NULL!");
        return 0;
    }
    char *buf = req_task_data->_data->buf;
    int fd = req_task_data->_fd;
    DEBUG_LOG("Begin To Parse Data[%s]", buf);
    http._parse(buf, request);
	Json::Value root;
	SubStrategyMgr::_get_instance()->_run_uri(request.url, request, root);
	DEBUG_LOG("Strategy Return: %s", root.toStyledString().c_str());
    int ret = (*call_back_proc)(_task_data, _task_ret);
    char *write_buf = new char[1024];
    sprintf(write_buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", 
			root.toStyledString().length(), root.toStyledString().c_str()); 
    std::cout << "begin to set evt data" << std::endl;
    std::cout << write_buf << " " << strlen(write_buf) << std::endl;
    // SubEventQueue::_get_instance()->_set_evt_data(fd, write_buf);
    if (req_task_data->_evt->_type == SELECT) {
        req_task_data->_evt->_event_add(fd, EVT_WRITE);
    } else if (req_task_data->_evt->_type == EPOLL) {
        DEBUG_LOG("Change To Read!");
        req_task_data->_evt->_event_mod(fd, EPOLLOUT | EPOLLET, \
                (void*)(new SUB_EPOLL_OUT_ENV(fd, write_buf)));
    }
    return ret;
}

int ReqTask::_call_back() {
    if (NULL == _task_ret) {
        return -1;
    } 
    // char* buf = (char*) _task_data;
    //int v = *((int*)_task_ret);
    char *ret = (char*) _task_ret; 
    /*解析html请求*/
	/*Request request;
    HttpParser http;
    http._parse(buf, request);
	*/
	return 0;
}



}
















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
