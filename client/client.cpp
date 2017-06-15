#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#include "thread.h"

using namespace sub_framework;

int port;
class Client {

private:
    struct sockaddr_in _srv_addr;
    struct sockaddr_in _clt_addr;
    int _clt_sock_fd;

public:
    void _init_svr_param(const std::string& ip, int port);
    void _connect_svr();
    void _send();
    void _recv();
};

void Client::_init_svr_param(const std::string& ip, int port) {
    _clt_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&_srv_addr, 0, sizeof(_srv_addr));
    _srv_addr.sin_family = AF_INET;
    _srv_addr.sin_port = htons(port);
    _srv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

void Client::_connect_svr() {
    
    if (-1 == _clt_sock_fd) {
        return ;
    }
    std::cout << "connect server" << std::endl;
    if (connect(_clt_sock_fd, (struct sockaddr*)&_srv_addr, sizeof(_srv_addr)) < 0) {
        return ;
    }
    char buf[1024];
    std::cout << "send data from client!" << std::endl;

    std::string http_str = "";
    http_str += "GET /?q=南京长江大桥给你 HTTP/1.0\r\n";
    http_str += "Host: 127.0.0.1\r\n";
    http_str += "User-Agetn: client\r\n";
    http_str += "Cookie: bdshare_firstime=1494745796849; Hm_lvt_5a0f15523b0100a6979e2772e0573347=1494745762; read_log=%7B%22tag%22%3A%221%22%2C%22id%22%3A%22243404%22%2C%22title%22%3A%22%E7%89%B9%E5%B7%A5%E7%9A%87%E5%A6%83%E6%A5%9A%E4%B9%94%E4%BC%A0%22%2C%22time%22%3A%222017-06-14%2010%3A56%3A46%22%2C%22chapter_title%22%3A%22%E7%AC%AC192%E7%AB%A0.%E5%A4%A9%E4%B8%8B%E5%8F%96%E8%88%8D(%E5%A4%A7%E7%BB%93%E5%B1%80)%22%7D%2C%7B%22tag%22%3A%221%22%2C%22id%22%3A%22231933%22%2C%22title%22%3A%22%E5%88%80%E8%AF%95%E5%A4%A9%E4%B8%8B%22%2C%22time%22%3A%222017-05-31%2014%3A44%3A47%22%2C%22chapter_title%22%3A%22%E7%AC%AC%E4%BA%94%E5%8D%81%E4%BA%8C%E7%AB%A0%20%E5%89%8D%E8%B7%AF%5B%E5%8D%93%E9%9B%85%E5%B1%85%E5%85%A8%E6%96%87%E5%AD%97%E7%A7%92%E6%9B%B4%5D%22%7D%2C%7B%22tag%22%3A%221%22%2C%22id%22%3A%22239973%22%2C%22title%22%3A%22%E5%B1%A0%E9%BE%99%E9%80%94%22%2C%22time%22%3A%222017-05-31%2014%3A44%3A40%22%2C%22chapter_title%22%3A%22%E7%AC%AC52%E7%AB%A0%20%E6%83%85%E6%8C%9A%E6%95%85%E4%BA%8B%22%7D%2C%7B%22tag%22%3A%221%22%2C%22id%22%3A%22232272%22%2C%22title%22%3A%22%E5%BD%92%E6%9D%A5%E4%B9%8B%E6%9C%80%E5%BC%BA%E5%94%90%E5%83%A7%22%2C%22time%22%3A%222017-05-31%2014%3A44%3A33%22%2C%22chapter_title%22%3A%22%E7%AC%AC47%E7%AB%A0%E7%9B%B8%E9%81%87%22%7D%2C%7B%22tag%22%3A%221%22%2C%22id%22%3A%22121451%22%2C%22title%22%3A%22%E6%AD%A5%E6%AD%A5%E6%83%8A%E5%BF%83%22%2C%22time%22%3A%222017-05-31%2014%3A44%3A24%22%2C%22chapter_title%22%3A%22%E7%AC%AC%E4%B8%80%E8%8A%82%5B%E5%8D%93%E9%9B%85%E5%B1%85%E5%85%A8%E6%96%87%E5%AD%97%E7%A7%92%E6%9B%B4%5D%22%7D%2C%7B%22tag%22%3A%221%22%2C%22id%22%3A%22220484%22%2C%22title%22%3A%22%E6%8B%A9%E5%A4%A9%E8%AE%B0%22%2C%22time%22%3A%222017-05-31%2014%3A44%3A20%22%2C%22chapter_title%22%3A%22%E7%AC%AC1182%E7%AB%A0%20%E9%BB%91%E8%A2%8D%E4%B9%8B%E6%AD%BB%22%7D%2C%7B%22tag%22%3A%221%22%2C%22id%22%3A%22187451%22%2C%22title%22%3A%22%E6%AC%A2%E4%B9%90%E9%A2%82%22%2C%22time%22%3A%222017-05-31%2014%3A44%3A27%22%2C%22chapter_title%22%3A%22%E7%AC%AC268%E7%AB%A0%22%7D%2C%7B%22tag%22%3A%221%22%2C%22id%22%3A%22239896%22%2C%22title%22%3A%22%E7%94%B7%E7%A5%9E%E4%B8%8D%E6%98%AF%E7%9C%81%E6%B2%B9%E7%81%AF%5B%E5%A8%B1%E4%B9%90%E5%9C%88%5D%22%2C%22time%22%3A%222017-05-31%2014%3A44%3A35%22%2C%22chapter_title%22%3A%2238.038%22%7D%2C%7B%22tag%22%3A%221%22%2C%22id%22%3A%22230802%22%2C%22title%22%3A%22%E9%AD%94%E7%89%A9%E5%BC%82%E7%95%8C%E7%94%9F%E5%AD%98%E6%8C%87%E5%8D%97%22%2C%22time%22%3A%222017-05-31%2014%3A44%3A43%22%2C%22chapter_title%22%3A%22%E7%AC%AC44%E7%AB%A0%20%E7%9F%AE%E4%BA%BA%E9%94%BB%E9%80%A0%22%7D%2C%7B%22tag%22%3A%221%22%2C%22id%22%3A%22121451%22%2C%22title%22%3A%22%E6%AD%A5%E6%AD%A5%E6%83%8A%E5%BF%83%22%2C%22time%22%3A%222017-05-31%2014%3A45%3A03%22%2C%22chapter_title%22%3A%22%E7%AC%AC%E4%B8%80%E8%8A%82%5B%E5%8D%93%E9%9B%85%E5%B1%85%E5%85%A8%E6%96%87%E5%AD%97%E7%A7%92%E6%9B%B4%5D%22%7D; csrftoken=nvVahtaPjazyCPfjlMcNQ9sYWfPtCgkl; Hm_lvt_7a67fa4088881ccceecdf7903967a3b3=1497184827,1497196885,1497318221,1497408054; Hm_lpvt_7a67fa4088881ccceecdf7903967a3b3=1497409074\r\n";

    http_str += "\r\n\r\n";
    for (int i = 0; i < 1000; i ++) {
        http_str += "fuck you";
    }

    //send(_clt_sock_fd, "Hello World!", strlen("Hello World!"), 0);
    int n = strlen(http_str.c_str());
    int tot_len = n;
    while (n > 0) {
        int nwrite = send(_clt_sock_fd, http_str.c_str() + tot_len - n, n, 0);
        if (nwrite < n) {
            break;
        }
        n -= nwrite;
    }
    std::cout << "receiver from server:" ;
    recv(_clt_sock_fd, buf, 1024, 0);
    
    std::cout << buf << std::endl;
    
    if (strcmp(buf, "Hello from svr!") == 0) {
        std::cout << "WRITE!" << std::endl;
    } else {
        std::cout << "WRONG!" << std::endl;
    }
    close(_clt_sock_fd);
}


class ClientHandler : public SubThreadHandler {
public:
    int _thread_proc_handler(void *args) {
        Client* client = new Client();
        std::cout << "fuck " << std::endl;
        client->_init_svr_param("127.0.0.1", port);
        client->_connect_svr();
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usag ./client [thread counts]" << std::endl;
        exit(1);
    }
    int thread_num = atoi(argv[1]);
    port = atoi(argv[2]);
    std::cout << argv[1] << std::endl;
    std::vector<SubThread*> threads;
    for (int i = 0; i < thread_num; i++) {
        std::cout << i << std::endl;
        ClientHandler*client_handler = new ClientHandler();
        SubThread* sub_thread = new SubThread(client_handler, NULL);
        sub_thread->_start();
        // sub_thread->_join();
        threads.push_back(sub_thread);
    }
    std::cout <<"nihao"<<std::endl;
    
    /*for (int i = 0; i < thread_num; i++) {
        threads[i]->_run();
    }*/

    for (int i = 0; i < thread_num; i++) {
        threads[i]->_join();
    }
    return 0;
}
