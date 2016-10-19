 
/**
 * @file test_select.cpp
 * @author kymowind@gmail.com
 * @date 2016/10/15 21:32:27
 * @brief 
 *  
 **/


#include  <stdio.h>
#include  <iostream>
#include  <unistd.h>
#include  <fcntl.h>
#include  <errno.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <sys/epoll.h>
#include  <netinet/in.h>
#include  <arpa/inet.h>
#include "select.h"
#include "event.h"
#include <fcntl.h>
#include "define.h"
#include "server.h"

using namespace sub_framework;
int main() {
    SubServer*svr = SubServer::_get_instance();
    svr->_run();
    return 0;
}









/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
