/**
 * @file http_parse.cpp
 * @date 2017/04/13 12:39:25
 * @brief 
 *  
 **/

#include "http_parse.h"

namespace sub_framework {

void HttpParser::_parse(char* recv_buf, Request& request) {
    int len = _parse_desc(recv_buf, request);
    if (-1 == len) {
		return ;
	}
	//_parse_headers(recv_buf + len, request);
    _parse_headers(recv_buf + len, request);
}

int HttpParser::_parse_desc(char* recv_buf, Request& request) {
    
    char*p = recv_buf;
    int i = 0;
    enum {METHOD, URL, VER} req_part = METHOD;
    int parse_len = 0;
    while (*p != '\r') {
        if (*p != ' ') {
            if (req_part == METHOD && (*p < 'A' || *p > 'Z')) {
                return -1;
            }
            parse_len ++;
        } else if (req_part == METHOD) {
            
            request.method = (char*) malloc ((parse_len + 1) * sizeof(char));
            memset(request.method, 0, strlen(request.method));
            strncpy(request.method, recv_buf, parse_len);
            request.method[parse_len] = '\0';
            parse_len = 0;
            req_part = URL;
        } else if (req_part == URL) {
            request.url = (char*) malloc ((parse_len + 1) * sizeof(char));
            char* url = (char*) malloc ((parse_len + 1) * sizeof(char));
            memset(request.url, 0, strlen(request.url));
            memset(url, 0, sizeof(url));
            strncpy(url, recv_buf + strlen(request.method) + 1, parse_len);
            strncpy(request.url, recv_buf + strlen(request.method) + 1, parse_len);
            request.url[parse_len] = '\0';
            parse_len = 0;
            req_part = VER;
        }
        p++;
        i++;
    }
    if (req_part == URL) {
        if (parse_len != 0) {
            request.url = (char*) malloc ((1 + parse_len) * sizeof(char));
            memset(request.url, 0, sizeof(request.url));
            strncpy(request.url, recv_buf + strlen(request.method) + 1, parse_len);
            request.url[parse_len] = '\0';
            parse_len = 0;
        } else {
            return -1;
        }
    }
    if (parse_len == 0) {
        request.version = (char *)malloc(8 * sizeof(char));
        memset(request.version, 0, 8 * sizeof(char));
        strcpy(request.version, "HTTP/1.1");
    } else {
        request.version = (char*) malloc ((1 + parse_len) * sizeof(char));
        memset(request.version, 0, sizeof(request.version));
        strncpy(request.version, recv_buf + strlen(request.method) + strlen(request.url) + 2, parse_len);
        request.version[parse_len] = '\0';
    }
    return i + 2;
}

int HttpParser::_parse_headers(char* recv_buf, Request& request) {
    
    char *p = recv_buf;
    char *key, *value;
    int i = 0;
    int parse_len = 0;
    while (1) {
        if (*p == '\r' && *(p + 1) == '\n') {
            break;
        }
        while (*p != ':') {
            p++;
            i++;
            parse_len++;
        }
        if (*p == ':') {
            key = (char*) malloc (sizeof(char) * (1 + parse_len));
            memset(key, 0, sizeof(key));
            strncpy(key, recv_buf + i - parse_len, parse_len);
            key[parse_len] = '\0';
            parse_len = 0;
            p++;
            i++;
        }
        while (*p != ' ') {
            p ++;
            i ++;
            parse_len ++;
        }
        while (*p != '\r') {
            p++;
            i++;
            parse_len++;
        }
        if (*p == '\r') {
            value = (char*) malloc (sizeof(char) * (parse_len + 1));
            memset(value, 0, sizeof(value));
            strncpy(value, recv_buf + i - parse_len, parse_len);
            value[parse_len] = '\0';
            parse_len = 0;
            p++;
            i++;
        }
        request.headers[std::string(key)] = std::string(value);
        i++;
        p++;
    }
    return i + 2;

}

}



















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
