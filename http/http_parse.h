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
#include <vector>
#include "utils.h"

#define HTTP_PARSE_FAIL -1

namespace sub_framework {

class Request {

public:
    std::string url;
    std::string method;
    std::string version;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> params;
    
    Request() {
        params["q"] = "";
		params["p"] = "1";
    }

    std::string get_query() const {   
        std::map<std::string, std::string>::const_iterator it = params.find("q");
        return it->second;
    }

	int get_page() const {
		std::map<std::string, std::string>::const_iterator it = 
			params.find("p");
        if (it == params.end()) {
            return 1;
        }
		return atoi(it->second.c_str());
	}

    std::string to_str() {
        std::string ret = "";
        ret += "url: ";
        ret += url;
        ret += ".\n";
        ret += "method: ";
        ret += method;
        ret += ".\n";
        ret += "version: ";
        ret += version;
        ret += ".\n";
        return ret;
    }

    ~Request() {
    }
};

class Response {
};

class HttpParser {

public:
    void _parse(char* recv_buf, Request& request);
    int _parse_desc(char* recv_buf, Request& request);
    int _parse_headers(char* recv_buf, Request& request);
    short int hexChar2dec(char c);
    char dec2hexChar(short int n);
    void encode(const std::string&url, std::string& result);
    void decode(const std::string&url, std::string& result);

};

}

#endif  //__HTTP_PARSE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
