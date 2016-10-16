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
	send(_clt_sock_fd, "Hello World!", strlen("Hello World"), 0);
	std::cout << "receiver from server:" ;
	
	recv(_clt_sock_fd, buf, 1024, 0);
	std::cout << buf << std::endl;
}

int main() {

	Client* client = new Client();
	client->_init_svr_param("127.0.0.1", 8888);
	client->_connect_svr();

	return 0;
}
