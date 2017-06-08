/**
 * @file http_parse.cpp
 * @date 2017/04/13 12:39:25
 * @brief 
 *  
 **/

#include "http_parse.h"

namespace sub_framework {

void HttpParser::_parse(char* recv_buf, Request& request) {
    std::cout << "recv buf: " << std::endl;
    std::cout << recv_buf << std::endl;

    int len = _parse_desc(recv_buf, request);
    if (-1 == len) {
		return ;
	}
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
            strncpy(request.method, recv_buf, parse_len);
            request.method[parse_len] = '\0';
            parse_len = 0;
            req_part = URL;
        } else if (req_part == URL) {
            char* url = (char*) malloc ((parse_len + 1) * sizeof(char));
            strncpy(url, recv_buf + strlen(request.method) + 1, parse_len);
			char*p = strtok(url, "?");
			char*s = strtok(NULL, "?");
			strncpy(request.url, p, strlen(p));
            request.url[strlen(p)] = '\0';
			char* val = NULL;
			char* ky = NULL;

            if (s != NULL) {
				char *outer_ptr=NULL;  
				char *inner_ptr=NULL;
				while ((val = strtok_r(s, "&", &outer_ptr)) != NULL) {
					s = val;
                    std::cout << val << std::endl;
					ky = strtok_r(s, "=", &inner_ptr);
					val = strtok_r(NULL, "=", &inner_ptr);
					if (ky != NULL && val != NULL) {
						request.params[std::string(ky)] = std::string(val);
                        std::cout << "Key[" << ky << "][" << val << "]" << std::endl;
					}
					s = NULL;
				}
			}
            //std::cout << ky << "." << val << std::endl;
			//strncpy(request.url, recv_buf + strlen(request.method) + 1, parse_len);
            parse_len = 0;
            req_part = VER;
        }
        p++;
        i++;
    }
    if (req_part == URL) {
        if (parse_len != 0) {
            strncpy(request.url, recv_buf + strlen(request.method) + 1, parse_len);
            request.url[parse_len] = '\0';
            parse_len = 0;
        } else {
            return -1;
        }
    }
    if (parse_len == 0) {
        strcpy(request.version, "HTTP/1.1");
    } else {
        strncpy(request.version, recv_buf + strlen(request.method) + strlen(request.url) + 2, parse_len);
        request.version[parse_len] = '\0';
    }
    return i + 2;
}

int HttpParser::_parse_headers(char* recv_buf, Request& request) {
    char *p = recv_buf;
    char key[1024];
    char value[10 * 1024];
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
            memset(key, 0, sizeof(key));
            strncpy(key, recv_buf + i - parse_len, parse_len);
            key[parse_len] = '\0';
            parse_len = 0;
            p++;
            i++;
        }
        while (*p == ' ') {
            p ++;
            i ++;
        }
        while (*p != '\r') {
            p++;
            i++;
            parse_len++;
        }
        if (*p == '\r') {
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
