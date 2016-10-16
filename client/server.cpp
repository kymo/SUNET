#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
using namespace std;

#define SOCK_FD int
#define BUF_SIZE 1024
#define BACKLOG 10
class Net {

private:
	struct sockaddr_in _svr_addr;
	struct sockaddr_in _clt_addr;
	SOCK_FD _svr_sock_fd;
	SOCK_FD _clt_sock_fd;

	socklen_t sin_size;
	int _port;
	int _backlog;

public:
	Net() {}
	~Net() {
		close(_clt_sock_fd);
		close(_svr_sock_fd);
	}
	Net(int port, int backlog) : _port(port), _backlog(backlog) {}
	void _create_svr_proc();
	void _work();
};


void Net::_create_svr_proc() {
	_svr_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_svr_sock_fd == -1) {
		perror("[Error] socket!");
		exit(1);
	}
	int yes = 1;
	if (setsockopt(_svr_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
		perror("[Error] set socket option!");
		exit(1);
	}
	_svr_addr.sin_family = AF_INET;			// TCP/IP
	_svr_addr.sin_port = htons(_port);		// port
	_svr_addr.sin_addr.s_addr = INADDR_ANY; // fill by my ip!

	memset(_svr_addr.sin_zero, 0, sizeof(_svr_addr.sin_zero));

	if (bind(_svr_sock_fd, (struct sockaddr*)&_svr_addr, sizeof(_svr_addr)) == -1) {
		perror("[Error] bind");
		exit(1);
	}

	if (listen(_svr_sock_fd, BACKLOG) == -1) {
		perror("[Error] listen!");
		exit(1);
	}

	std::cout << "[DEBUG] begin listening on port !" << _port << std::endl;
}

void Net::_work() {	
	sin_size = sizeof(struct sockaddr);
	std::cout << "[DEBUG] begin to acccept from clients!" << std::endl;
	_clt_sock_fd = accept(_svr_sock_fd, (struct sockaddr*)&_clt_addr, &sin_size);
	if (-1 == _clt_sock_fd) {
		perror("accept error!");
		exit(1);
	}
	char buffer[1024];
	std::cout << "Server gets connection from " << inet_ntoa(_clt_addr.sin_addr) << std::endl;
	int nbytes = recv(_svr_sock_fd, buffer, 1024, 0);
	buffer[nbytes] = '\0';
	std::cout << "server received: " << buffer << std::endl;

}

int main() {

	Net *net = new Net(8888, 10);
	net->_create_svr_proc();
	net->_work();
	return 0;

}
