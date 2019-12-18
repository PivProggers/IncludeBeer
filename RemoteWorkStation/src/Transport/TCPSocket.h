#pragma once
#include "stdafx.h"
#include "TransportApi.h"
#include <string>

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#ifdef _WIN32
#define OS_WIN
#endif

#ifndef OS_WIN
	#include <netinet/in.h>
	#include <sys/socket.h> 
	#include <vector>
	#include <string>
	#undef TRANSPORT_API
#else
	#include <Winsock2.h>
	#include <vector>
	#pragma comment( lib, "Ws2_32.lib" )
#endif


class TCPSocket
{
public:
	typedef std::vector<char> AChar;

protected:
	static int init;
	bool bound, listening, connected;
	int * iLinks;
	#ifdef OS_WIN
		SOCKET s;
	#else 
		int s; //unix socket
	#endif
public:
	TRANSPORT_API bool IsConnected() { return connected; };
	TRANSPORT_API TCPSocket(void);
	TRANSPORT_API TCPSocket(const TCPSocket & copy);
	TRANSPORT_API ~TCPSocket(void);
	TRANSPORT_API bool bind(int port, const sockaddr_in * name = 0);//server
	TRANSPORT_API bool bind(const std::string & addr, int port);//server
	TRANSPORT_API bool listen(int port, int backlog = SOMAXCONN);//server
	#ifdef OS_WIN
		TCPSocket accept(int port, __out sockaddr* addr = 0, __out int* addrlen = 0);//server
	#else
		TCPSocket accept(int port, sockaddr* addr = 0, int* addrlen = 0);//server
	#endif   	
	TRANSPORT_API bool connect(const sockaddr_in & name);//client
	TRANSPORT_API bool connect(const std::string & addr, int port);//client
	TRANSPORT_API bool send(const AChar & inbuf);//client ? server
	TRANSPORT_API AChar receive();//client ? server
	TRANSPORT_API TCPSocket & operator=(const TCPSocket & copy);

	
	
	
};

#endif

