/**
 * @file test_http.cpp
 * @date 2017/04/13 13:06:03
 * @brief 
 *  
 **/

#include "http_parse.h"

int main() {
    
    sub_framework::Request* request = new sub_framework::Request();

    sub_framework::HttpParser* http_parser = new sub_framework::HttpParser();

    char *recv_buf = "";

    http_parser->_parse(recv_buf, *request);

    return 0;
}




















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
