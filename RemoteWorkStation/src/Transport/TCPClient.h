#pragma once
#include "stdafx.h"
#include "TransportApi.h"
#include "TCPSocket.h"
#include "TCPServer.h"

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

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

class TCPClient : TCPSocket{
protected:


public:
	TRANSPORT_API TCPClient(void);
	TRANSPORT_API TCPClient(const TCPClient & copy);
	TRANSPORT_API ~TCPClient(void);

	TRANSPORT_API bool connect(const sockaddr_in & name);
	TRANSPORT_API bool connect(const std::string & addr, int port);
	TRANSPORT_API bool send(const AChar & inbuf);
	TRANSPORT_API AChar receive();
};

#endif