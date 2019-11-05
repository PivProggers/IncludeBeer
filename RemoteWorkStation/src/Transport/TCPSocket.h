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
	bool bound, listening, connected;
	int * iLinks;
	SOCKET s;

public:
	TRANSPORT_API TCPSocket(void);
	TRANSPORT_API TCPSocket(const TCPSocket & copy);
	TRANSPORT_API ~TCPSocket(void);

	TRANSPORT_API TCPSocket & operator=(const TCPSocket & copy);

	
	
	
};

#endif

