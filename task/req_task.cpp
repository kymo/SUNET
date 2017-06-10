    
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
	// std::cout << "delete req task!" << std::endl;
	if (NULL != _task_ret) {
		free(_task_ret);
		_task_ret = NULL;
	}
	if (NULL != _task_data) {
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
        std::cout << "call back proc NULL!" << std::endl;
		return 0;
    }
	// 解析http
	Request request;
    HttpParser http;
    //char* buf = (char*) _task_data;
    REQ_TASK_DATA *req_task_data = (REQ_TASK_DATA *)_task_data;
    if (req_task_data == NULL) {
        return 0;
    }
    char *buf = req_task_data->_data->buf;
    int fd = req_task_data->_fd;
    http._parse(buf, request);
    std::cout << request.to_str() << std::endl;
	Json::Value root;
	SubStrategyMgr::_get_instance()->_run_uri(request.url, request, root);
    std::cout << root.toStyledString() << std::endl;
	int ret = (*call_back_proc)(_task_data, _task_ret);
    std::cout << "finish call back proc" << std::endl;
    std::cout << (char*) _task_ret << std::endl;
    char *write_buf = new char[1024];
    // char *json = "hello";
    sprintf(write_buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s", strlen(root.toStyledString().c_str()), 
        root.toStyledString().c_str()); 
    SubEventQueue::_get_instance()->_set_evt_data(fd, write_buf);
    std::cout << "finish call back proc!" << std::endl;
    if (req_task_data->_evt->_type == SELECT) {
        req_task_data->_evt->_event_add(fd, EVT_WRITE);
    } else if (req_task_data->_evt->_type == EPOLL) {
        req_task_data->_evt->_event_mod(fd, EPOLLOUT | EPOLLET);
    }
	if (NULL != req_task_data) {
		delete req_task_data;
		req_task_data = NULL;
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
