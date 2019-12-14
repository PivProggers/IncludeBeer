#pragma once
#include "stdafx.h"
#include "TransportApi.h"
#include "TCPSocket.h"
#include <string>
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifndef CLASSCLIENT_H
#define CLASSCLIENT_H

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

class Client : public TCPSocket {
public:
	TRANSPORT_API Client(void);
	TRANSPORT_API ~Client(void);
	TRANSPORT_API Client(const Client& copy);
	//TRANSPORT_API Client & operator=(const Client&);

	TRANSPORT_API bool InitClient(const std::string & addr, int port);
	TRANSPORT_API bool SendDataToServer(const AChar & inbuf);//client ? server
	TRANSPORT_API int CloseClient();
};



#endif