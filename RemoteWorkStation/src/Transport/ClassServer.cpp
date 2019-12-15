#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include "ClassServer.h"
#include "..\Commands\Command.h"
#include <fstream>
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>
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
	while (true) {
	tryout:
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		cout<< "\tAccepting new connection...";

		SetConsoleTextAttribute(hConsole, 106);
		cout << "\!_OPEN CLIENT_!\t" << endl;
		SetConsoleTextAttribute(hConsole, 2);

		while (true)
		{
			//переменная для управления выводом сервера
			client = TCPSocket::accept(port);
		acception:
			// Читаем переданных клиентом данные
			bufrec = client.receive();
			
			//крутим цикл, пока размер принятного буфера перестанет быть нулем
			if (bufrec.size() != 0)
				break;

			// Отправляем клиенту полученную от него же строку
			if (!client.send(bufrec)) {
				SetConsoleTextAttribute(hConsole, 12);
				cout << "Client closed connection" << endl;
				SetConsoleTextAttribute(hConsole, 2);
				goto tryout;
			}
		}

		//вывод сообщения клиента
		SetConsoleTextAttribute(hConsole, 12);
		cout << "\tCLIENT MESSAGE:\t" << endl;

		//_________вывод содержимого буфера__________//
		std::stringstream ss(std::string(bufrec.begin(), bufrec.end()));

		//вывод содержимого xml
		//cсоздаем объект класса и открываем поток на чтение
		Command com;

		//deserialize block
		{
			cereal::XMLInputArchive ar(ss);
			//чтение узла Command
			ar(cereal::make_nvp("Command", com));
			//вывод параметров
			SetConsoleTextAttribute(hConsole, 11);
			cout << "NAME OF COM: " << com.GetName() << " PARAMS: " << com.GetParameters() << endl;
		}

		RunApplication command(com.GetName(), com.GetParameters());
		command.Run();
		cout << endl;

		//очистка буфера
		bufrec.clear();

		SetConsoleTextAttribute(hConsole, 2);
		if (this->connected) goto tryout;
		else goto acception;
	}
}

int Server::CloseServer() {
	cout << "\tShutdown SERVER...\t" << endl;
	return 0;
}