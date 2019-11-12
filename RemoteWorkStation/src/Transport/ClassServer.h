#pragma once
#include "stdafx.h"
#include "TransportApi.h"
#include "TCPSocket.h"
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef CLASSSERVER_H
#define CLASSSERVER_H

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

class Server : public TCPSocket {
public:
	TRANSPORT_API Server(void);
	TRANSPORT_API ~Server(void);
	TRANSPORT_API Server(const Server& copy);

	TRANSPORT_API bool InitServer(const std::string & addr, int port);
	TRANSPORT_API int ReceiveDataFromClient(int port, Server& server, TCPSocket& client);
	TRANSPORT_API int CloseServer();
};



#endif