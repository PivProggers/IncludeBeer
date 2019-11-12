#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include "ClassServer.h"
#include <exception>
#include <iostream>
using namespace std;

Server::Server(void) {
	TCPSocket server;
}

Server::~Server(void) {}

Server::Server(const Server& copy) {
	Server server;
	server = copy;
}

bool Server::InitServer(const std::string & addr, int port) {
	TCPSocket::bind(addr, port);
	TCPSocket::listen(port);

	return (bound && listening);
}


int Server::ReceiveDataFromClient(int port, Server& server, TCPSocket& client) {

	TCPSocket::AChar bufrec;
	int len = 1024;
	while (true) {

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		printf("\tAccepting new connection...");

		SetConsoleTextAttribute(hConsole, 106);
		printf("\!_OPEN CLIENT_!\t\n");
		SetConsoleTextAttribute(hConsole, 2);

		while (true)
		{
			//переменна€ дл€ управлени€ выводом сервера
			client = TCPSocket::accept(port);

			// „итаем переданных клиентом данные
			bufrec.resize(len);
			bufrec = client.receive();

			//крутим цикл, пока размер прин€тного буфера перестанет быть нулем
			if (bufrec.size() != 0)
				break;

			// ќтправл€ем клиенту полученную от него же строку
			client.send(bufrec);
		}

		//вывод сообщени€ клиента
		SetConsoleTextAttribute(hConsole, 12);
		printf("\tCLIENT MESSAGE:\t");

		//вывод содержимого буфера
		for (int i = 0; i < bufrec.size(); ++i)
			cout << bufrec[i];

		printf("\n");
		//очистка буфера
		bufrec.clear();

		SetConsoleTextAttribute(hConsole, 2);

	

		//ѕредлагаем закрыть сервер
		printf("\tContinue using server? (Y/N)\t\n");
		char ch;
		cin >> ch;
		if (ch == 'N' || ch == 'n') {
			CloseHandle(hConsole);
			return server.CloseServer();
		}
		else {
			printf("\t__________________________________\t\n\n");
		}
	}
}

int Server::CloseServer() {

	printf("\tShutdown SERVER...\t\n");
	return 0;
}