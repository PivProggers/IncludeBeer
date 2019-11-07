#pragma once
#include "stdafx.h"
#include "TransportApi.h"
#include "TCPSocket.h"

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef TCPSERVER_H
#define TCPSERVER_H

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

class TCPServer : TCPSocket {
protected : 


public:
	TRANSPORT_API TCPServer(void);
	TRANSPORT_API TCPServer(const TCPSocket & copy);
	TRANSPORT_API ~TCPServer(void);

	TRANSPORT_API bool bind(int port, const sockaddr_in * name = 0);
	TRANSPORT_API bool bind(const std::string & addr, int port);
	TRANSPORT_API bool listen(int port, int backlog = SOMAXCONN);
	TRANSPORT_API TCPServer accept(int port, __out sockaddr * addr = 0, __out int * addrlen = 0);
	TRANSPORT_API AChar receive();


	//сделать метод отправки отчета
};

#endif