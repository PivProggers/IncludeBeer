#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\TCPServer.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	TCPServer server;

	// Присоединяемся к серверу
	string name;
	name = "localhost";
	server.bind(name, 65042);
	server.listen(65042);
	server.accept(65042);

	return 0;
}

