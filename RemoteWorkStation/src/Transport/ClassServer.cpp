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
	int len = 1024;
	while (true) {
	tryout:
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		cout<< "\tAccepting new connection...";

		SetConsoleTextAttribute(hConsole, 106);
		cout << "\!_OPEN CLIENT_!\t" << endl;
		SetConsoleTextAttribute(hConsole, 2);

		while (true)
		{
			//���������� ��� ���������� ������� �������
			client = TCPSocket::accept(port);
		acception:
			// ������ ���������� �������� ������
			bufrec.resize(len);
			bufrec = client.receive();
			
			//������ ����, ���� ������ ���������� ������ ���������� ���� �����
			if (bufrec.size() != 0)
				break;

			// ���������� ������� ���������� �� ���� �� ������
			if (!client.send(bufrec)) {
				SetConsoleTextAttribute(hConsole, 12);
				cout << "Client closed connection" << endl;
				SetConsoleTextAttribute(hConsole, 2);
				goto tryout;
			}
		}

		//����� ��������� �������
		SetConsoleTextAttribute(hConsole, 12);
		cout << "\tCLIENT MESSAGE:\t" << endl;

		//_________����� ����������� ������__________//
		std::stringstream ss(std::string(bufrec.begin(), bufrec.end()));

		//����� ����������� xml
		//c������� ������ ������ � ��������� ����� �� ������
		Command com;

		//deserialize block
		{
			cereal::XMLInputArchive ar(ss);
			//������ ���� Command
			ar(cereal::make_nvp("Command", com));
			//����� ����������
			SetConsoleTextAttribute(hConsole, 11);
			cout << "NAME OF COM: " << com.GetName() << " PARAMS: " << com.GetParameters() << endl;
		}

		RunApplication command(com.GetName(), com.GetParameters());
		command.Run();
		// Display
		/*std::cout << ss.str() << std::endl;
		for (int i = 0; i < bufrec.size(); ++i)
			cout << bufrec[i];*/

		printf("\n");

		//������� ������
		bufrec.clear();

		SetConsoleTextAttribute(hConsole, 2);
		if (this->connected) goto tryout;
		else goto acception;


	/*		//����� ����������� xml
		//c������� ������ ������ � ��������� ����� �� ������ �����
		Command com;
		ifstream file("out.xml");
			if (!file.is_open()) {
				cout << "Oops!" << endl;
				return -1;
			}
		//deserialize block
		{
			cereal::XMLInputArchive ar(file);
			//������ ���� Command
			ar(cereal::make_nvp("Command", com));
			//����� ����������
			cout <<"\tNAME OF COM: " << com._name << " PARAMS: " << com._parameters << endl;
		}
		*/

		//���������� ������� ������
		/*printf("\tContinue using server? (Y/N)\t  ");
		char ch;
		cin >> ch;
		if (ch == 'N' || ch == 'n' || ch == 'NO'  || ch == 'no' || ch == 'No') {
			CloseHandle(hConsole);
			return server.CloseServer();
		}
		else {
			printf("\t__________________________________\t\n\n");
		}*/
	}
}

int Server::CloseServer() {
	cout << "\tShutdown SERVER...\t" << endl;
	return 0;
}