 ///
 /// @file    Acceptor.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-05-07 15:25:43
 ///

#include "../include/Acceptor.h"
#include <sys/types.h>
#include <sys/socket.h>

namespace wd
{

Acceptor::Acceptor(unsigned short port)
: _addr(port)
, _listensock()
{}

Acceptor::Acceptor(const string & ip, unsigned short port)
: _addr(ip, port)
, _listensock()
{}

void Acceptor::ready()
{
	setReuseAddr(true);
	setReusePort(true);
	bind();
	listen();
}

void Acceptor::setReuseAddr(bool on)
{
	int one = on;
	if(setsockopt(_listensock.fd(), 
		SOL_SOCKET, 
		SO_REUSEADDR, 
		&one, 
		sizeof(one)) < 0) {
		perror("setsockopt");	
	}
}

void Acceptor::setReusePort(bool on)
{
	int one = on;
	if(setsockopt(_listensock.fd(), 
		SOL_SOCKET, 
		SO_REUSEPORT, 
		&one, 
		sizeof(one)) < 0) {
		perror("setsockopt");	
	}
}

void Acceptor::bind()
{
	int ret = ::bind(_listensock.fd(), 
		(struct sockaddr *)_addr.getInetAddressPtr(),
		sizeof(struct sockaddr));
	if(-1 == ret) {
		perror("bind");
	}
}

void Acceptor::listen()
{
	int ret = ::listen(_listensock.fd(), 10);
	if(-1 == ret) {
		perror("listen");
	}
}

int Acceptor::accept()
{
	int peerfd = ::accept(_listensock.fd(), NULL, NULL);
	if(peerfd == -1) {
		perror("accept");
	}
	return peerfd;
}

}//end of namespace wd
