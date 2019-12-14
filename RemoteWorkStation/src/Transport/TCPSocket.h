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
	SOCKET s;
public:
	TRANSPORT_API TCPSocket(void);
	TRANSPORT_API TCPSocket(const TCPSocket & copy);
	TRANSPORT_API ~TCPSocket(void);
	TRANSPORT_API bool bind(int port, const sockaddr_in * name = 0);//server
	TRANSPORT_API bool bind(const std::string & addr, int port);//server
	TRANSPORT_API bool listen(int port, int backlog = SOMAXCONN);//server
	TRANSPORT_API TCPSocket accept(int port, __out sockaddr * addr = 0, __out int * addrlen = 0);//server
	TRANSPORT_API bool connect(const sockaddr_in & name);//client
	TRANSPORT_API bool connect(const std::string & addr, int port);//client
	TRANSPORT_API bool send(const AChar & inbuf);//client ? server
	TRANSPORT_API AChar receive();//client ? server
	TRANSPORT_API TCPSocket & operator=(const TCPSocket & copy);

	
	
	
};

#endif

