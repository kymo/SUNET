/**
 * @file http_parse.cpp
 * @date 2017/04/13 12:39:25
 * @brief 
 *  
 **/

#include "http_parse.h"

namespace sub_framework {

char HttpParser::dec2hexChar(short int n) {  
    if ( 0 <= n && n <= 9 ) {  
        return char( short('0') + n );  
    } else if ( 10 <= n && n <= 15 ) {  
        return char( short('A') + n - 10 );  
    } else {  
        return char(0);  
    }  
}  

short int HttpParser::hexChar2dec(char c) {  
    if ( '0'<=c && c<='9' ) {  
        return short(c-'0');  
    } else if ( 'a'<=c && c<='f' ) {  
        return ( short(c-'a') + 10 );  
    } else if ( 'A'<=c && c<='F' ) {  
        return ( short(c-'A') + 10 );  
    } else {  
        return -1;  
    }  
}  

void HttpParser::encode(const std::string &URL, std::string& result) {
    result = "";  
    for ( unsigned int i=0; i<URL.size(); i++ ) {  
        char c = URL[i];  
        if (  
                ( '0'<=c && c<='9' ) ||  
                ( 'a'<=c && c<='z' ) ||  
                ( 'A'<=c && c<='Z' ) ||  
                c=='/' || c=='.'  
           ) {  
            result += c;  
        } else {  
            int j = (short int)c;  
            if ( j < 0 ) {  
                j += 256;  
            }  
            int i1, i0;  
            i1 = j / 16;  
            i0 = j - i1*16;  
            result += '%';  
            result += dec2hexChar(i1);  
            result += dec2hexChar(i0);  
        }  
    }  
}  

void HttpParser::decode(const std::string &URL, std::string& result) {  
    result = "";  
    for ( unsigned int i=0; i<URL.size(); i++ ) {  
        char c = URL[i];  
        if ( c != '%' ) {  
            result += c;  
        } else {  
            char c1 = URL[++i];  
            char c0 = URL[++i];  
            int num = 0;  
            num += hexChar2dec(c1) * 16 + hexChar2dec(c0);  
            result += char(num);  
        }  
    }  
}  

void HttpParser::_parse(char* recv_buf, Request& request) {
    int len = _parse_desc(recv_buf, request);
    if (HTTP_PARSE_FAIL == len) {
        return ;
    }
    _parse_headers(recv_buf + len, request);
}

int HttpParser::_parse_desc(char* recv_buf, Request& request) {
    char *p = recv_buf;
    int i = 0;
    enum {METHOD, URL, VER} req_part = METHOD;
    int parse_len = 0;
    while (*p != '\r') {
        if (*p != ' ') {
            if (req_part == METHOD && (*p < 'A' || *p > 'Z')) {
                return HTTP_PARSE_FAIL;
            }
            parse_len ++;
        } else if (req_part == METHOD) {
            request.method.assign(recv_buf, 0, parse_len);
            parse_len = 0;
            req_part = URL;
        } else if (req_part == URL) {
            std::string url_info = "";
            url_info.assign(recv_buf, request.method.length() + 1, parse_len);
            std::vector<std::string> url_info_splits;
            StringUtil::split(url_info, "?", url_info_splits);
            if (url_info_splits.size() == 0) {
                return HTTP_PARSE_FAIL;
            }
            request.url = url_info_splits[0];
            if (url_info_splits.size() > 1) {
                std::vector<std::string> params;
                StringUtil::split(url_info_splits[1], "&", params);
                for (int i = 0; i < params.size(); i++) {
                    std::vector<std::string> key_value;
                    StringUtil::split(params[i], "=", key_value);
                    if (key_value.size() != 2) {
                        continue;
                    }
                    std::string decode_val = "";
                    decode(key_value[1], decode_val);
                    request.params[key_value[0]] = decode_val;
                }
            }
            parse_len = 0;
            req_part = VER;
        }
        p++;
        i++;
    }
    if (req_part == URL) {
        if (parse_len != 0) {
            request.url.assign(recv_buf, request.method.length() + 1, parse_len);
            parse_len = 0;
        } else {
            return HTTP_PARSE_FAIL;
        }
    }
    if (parse_len == 0) {
        request.version = "HTTP/1.1";
    } else {
        request.version.assign(recv_buf,
            request.method.length() + request.url.length() + 2, parse_len);
    }
    return i + 2;
}

int HttpParser::_parse_headers(char* recv_buf, Request& request) {
    char *p = recv_buf;
    std::string key = "";
    std::string value = "";
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
            key.assign(recv_buf, i - parse_len, parse_len);
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
            value.assign(recv_buf, i - parse_len, parse_len);
            parse_len = 0;
            p++;
            i++;
        }
        request.headers[key] = value;
        i++;
        p++;
    }
    return i + 2;

}

}



















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
