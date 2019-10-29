#pragma once
#include "stdafx.h"
#include "TransportApi.h"

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

	int * iLinks;
	SOCKET s;
	bool bound, listening, connected;

public:
	TRANSPORT_API TCPSocket(void);
	TRANSPORT_API TCPSocket(const TCPSocket & copy);
	TRANSPORT_API ~TCPSocket(void);

	TRANSPORT_API TCPSocket & operator=(const TCPSocket & copy);

	TRANSPORT_API bool bind(int port, const sockaddr_in * name = 0);
	TRANSPORT_API bool bind(const std::string & addr, int port);
	TRANSPORT_API TCPSocket accept(int port, __out sockaddr * addr = 0, __out int * addrlen = 0);
	TRANSPORT_API bool listen(int port, int backlog = SOMAXCONN);
	TRANSPORT_API bool connect(const sockaddr_in & name);
	TRANSPORT_API bool connect(const std::string & addr, int port);
	TRANSPORT_API AChar receive();
	TRANSPORT_API bool send(const AChar & inbuf);
};

#endif

