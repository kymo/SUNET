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

    char http_body[1024];
    std::string http_str = "";
    http_str += "GET /?q=南京长江大桥给你 HTTP/1.0\r\n";
    http_str += "Host: 127.0.0.1\r\n";
    http_str += "User-Agetn: client\r\n";
    http_str += "\r\n\r\n";

	//send(_clt_sock_fd, "Hello World!", strlen("Hello World!"), 0);
	send(_clt_sock_fd, http_str.c_str(), strlen(http_str.c_str()), 0);
    std::cout << "receiver from server:" ;
	//while (1);
	recv(_clt_sock_fd, buf, 1024, 0);
	std::cout << buf << std::endl;
	if (strcmp(buf, "Hello from svr!") == 0) {
		std::cout << "WRITE!" << std::endl;
	} else {
		std::cout << "WRONG!" << std::endl;
	}
}


class ClientHandler : public SubThreadHandler {
public:
	int _thread_proc_handler(void *args) {
		Client* client = new Client();
        std::cout << "fuck " << std::endl;
		client->_init_svr_param("127.0.0.1", 9000);
		client->_connect_svr();
	}
};

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Usag ./client [thread counts]" << std::endl;
		exit(1);
	}
	int thread_num = atoi(argv[1]);
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
	
	for (int i = 0; i < thread_num; i++) {
	    threads[i]->_run();
		threads[i]->_join();
	}
	return 0;
}
