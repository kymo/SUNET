 /*
* @file http_parse.h
 * @date 2017/04/13 12:30:35
 * @brief 
 *  
 **/



#ifndef  __HTTP_PARSE_H_
#define  __HTTP_PARSE_H_

#include <map>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

namespace sub_framework {

class Request {

public:
    char *url;
    char *method;
    char *version;
    std::map<std::string, std::string> headers;
	std::map<std::string, std::string> params;
	
	Request() {
		url = NULL;
		method = NULL;
		version = NULL;
	}

	~Request() {
		if (NULL != url) {
			free(url);
			url = NULL;
		}
		if (NULL != method) {
			free(method);
			method = NULL;
		}
		if (NULL != version) {
			free(version);
			version = NULL;
		}
	}
};

class Response {
};

class HttpParser {

public:
    void _parse(char* recv_buf, Request& request);
    int _parse_desc(char* recv_buf, Request& request);
    int _parse_headers(char* recv_buf, Request& request);
};

}

#endif  //__HTTP_PARSE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
