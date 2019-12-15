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
		name = "10.211.55.1";
		int port = 65041;

		server.InitServer(name, port);
		Sleep(1000);
	
		if (!server.ReceiveDataFromClient(port, server, client)) {
			break;
		}

	}

	return 0;

}

//ЭТО РАБОЧАЯ ВЕРСИЯ ТОЛЬКО С КЛАССОМ TCPSocket
/*int main()
{
	system("COLOR 02");
	while (true) {
		TCPSocket server, client;

		// Присоединяемся к серверу
		string name;
		name = "127.0.0.1";
		int port = 65041;

		server.bind(name, port);
		server.listen(port);


		TCPSocket::AChar bufrec;
		int len = 1024;

		while (true)
		{
			client = server.accept(port);
			// Читаем переданных клиентом данные
			
			Sleep(1000);
			bufrec.resize(len);
			bufrec = client.receive();

			if (bufrec.size() != 0)
}