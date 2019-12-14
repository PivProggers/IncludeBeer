#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include "ClassClient.h"
#include <exception>
#include <iostream>
#include <string>
using namespace std;

Client::Client(void) {
	TCPSocket client;
}

Client::~Client(void) {}

Client::Client(const Client& copy) {
	Client client;
	client = copy;
}

bool Client::InitClient(const std::string & addr, int port) {

	TCPSocket::connect(addr, port);
	return connected;
}

bool Client::SendDataToServer(const AChar & inbuf){
	return TCPSocket::send(inbuf);
}


int Client::CloseClient() {

	printf("\tShutdown CLIENT...\t\n");
	return 0;
}
