    
/**
 * @file req_task.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/10 09:47:38
 * @brief 
 *  
 **/

#include "req_task.h"
#include "http_parse.h"

namespace sub_framework {

ReqTask::ReqTask(const std::string& task_name) {
    _task_name = task_name;
}

ReqTask::~ReqTask() {
	if (NULL != _task_ret) {
		free(_task_ret);
		_task_ret = NULL;
	}
}

void ReqTask::_set_task_data(void *task_data) {
    _task_data = task_data;
}

int ReqTask::_run() {
	CALL_BACK_PROC call_back_proc = SubTaskMgr::_get_instance()->_get_call_back_proc(_task_name);
    if (NULL == call_back_proc) {
        std::cout << "call back proc NULL!" << std::endl;
		return 0;
    }
	// 解析http
	Request request;
    HttpParser httpp;
    //char* buf = (char*) _task_data;
    REQ_TASK_DATA *req_task_data = (REQ_TASK_DATA *)_task_data;
    if (req_task_data == NULL) {
        return 0;
    }
    char *buf = req_task_data->_data;
    // std::cout << "task data" << buf << std::endl;
    int fd = req_task_data->_fd;
    // httpp._parse(buf, request);
	// std::cout << " ----------------------------------- " << std::endl;
	// std::cout << "Request result:" << std::endl;
	/*
	 * std::cout << request.url << std::endl;
	std::cout << request.method << std::endl;
	std::cout << request.version << std::endl;
	for (std::map<std::string, std::string>::iterator it = request.headers.begin(); it != request.headers.end(); it++) {
		std::cout << it->first << ":" << it->second << std::endl;
	}
	for (std::map<std::string, std::string>::iterator it = request.params.begin(); it != request.params.end(); it++) {
		std::cout << it->first << ":" << it->second << std::endl;
	}
	*/
	// std::cout << "fuck" << " " << fd << std::endl;
	// 判断http 的url
	// if (strcmp(http.url, "/get") == 0) {
		 
	// }
	
    _task_ret = (void*)(malloc(sizeof(char) * 65536));
    int ret = (*call_back_proc)(_task_data, _task_ret);
    SubEventQueue::_get_instance()->_set_evt_data(fd, (char*)_task_ret);
    if (req_task_data->_evt->_type == SELECT) {
        //req_task_data->_evt->_event_add(fd, EVT_WRITE);
    } else if (req_task_data->_evt->_type == EPOLL) {
        //std::cout << "mod event" << std::endl;
        req_task_data->_evt->_event_mod(fd, EPOLLOUT | EPOLLET);
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
    HttpParser httpp;
    httpp._parse(buf, request);
	*/
	return 0;
}



}
















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
