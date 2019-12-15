#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\ClassServer.h"
#include "..\Commands\Command.h"
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <fstream>
using namespace std;

//ЭТО ВЕРСИЯ С КЛАССОМ Server

int main()
{
	system("COLOR 02");

	Server server; TCPSocket client;

	while (true) {
		// Присоединяемся к серверу
		string name;
		name = "127.0.0.1";//"192.168.1.34";//"127.0.0.1";
		int port = 65041;

		server.InitServer(name, port);
		Sleep(1000);
	
		if (!server.ReceiveDataFromClient(port, server, client)) {
			break;
		}

	}

	return 0;

}