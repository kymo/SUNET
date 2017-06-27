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
    char *recv_buf = "22%2C%22chapter_title%22%3A%22%E7%AC%AC52%E7%AB%A0%20%E6%83%85%E6%8C%9A%E6%95%85%E4%BA%8B%22%7D%2C%7B%22tag%22%3A%221%22%2C%22id%22%3A%22231933%22%2C%22title%22%3A%22%E5%88%80%E8%AF%95%E5%A4%A9%E4%B8%8B%22%2C%22time%22%3A%222017-05-31%2014%3A44%3A47%22%2C%22chapter_title%22%3A%22%E7%AC%AC%E4%BA%94%E5%8D%81%E4%BA%8C%E7%AB%A0%20%E5%89%8D%E8%B7%AF%5B%E5%8D%93%E9%9B%85%E5%B1%85%E5%85%A8%E6%96%87%E5%AD%97%E7%A7%92%E6%9B%B4%5D%22%7D; csrftoken=nvVahtaPjazyCPfjlMcNQ9sYWfPtCgkl; Hm_lvt_7a67fa4088881ccceecdf7903967a3b3=1497424500,1497424705,1497458204,1497539140; Hm_lpvt_7a67fa4088881ccceecdf7903967a3b3=1497594477";
    http_parser->_parse(recv_buf, *request);
    return 0;
}




















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
