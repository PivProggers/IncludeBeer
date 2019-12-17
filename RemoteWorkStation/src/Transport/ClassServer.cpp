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
#include <algorithm>
using namespace std;

const char* NameOfCommands[] = { "com1", "com2", "com3", "com4" };

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


int Server::WorkWithClient(int port, Server& server, TCPSocket& client) {

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
			//���������� ��� ���������� ������� �������
			client = TCPSocket::accept(port);
		acception:
			// ������ ���������� �������� ������
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
		auto it = bufrec.begin();
		const char* comname;

		//����� �������
		for (auto& cm : NameOfCommands) {
			 it = std::search(bufrec.begin(), bufrec.end(), cm, cm + strlen(cm));
			 if (it != bufrec.end()) {
				 comname = cm;
				 break;
			 }
		}
	
		//����� ����������� xml
		//c������� ������ ������ � ��������� ����� �� ������
		Command com;
		std::stringstream ss(std::string(bufrec.begin(), it));
		//deserialize block
		{
			cereal::XMLInputArchive ar(ss);
			//������ ���� Command
			ar(cereal::make_nvp("Command", com));
			//����� ����������
			SetConsoleTextAttribute(hConsole, 11);
			cout << "NAME: " << com.GetName() << " PARAMS: " << com.GetParameters();
		}
		cout << " IDofCom: "<< comname << endl;

		if (comname == "com1") {
			RunApplication command(com.GetName(), com.GetParameters());
			command.Run();

			//�����
			string report = command.MakeReport();
			TCPSocket::AChar sendbuf;

			sendbuf.assign(report.begin(), report.end());
			if (IsConnected())
				client.send(sendbuf);

			sendbuf.clear();
			cout << report;
			report.clear();
		}
		else if (comname == "com2") {
			FileHandler command(com.GetName(), com.GetParameters());
			string a,b;
			a.assign(it + strlen(comname), bufrec.end());
			b = command.RecieveFile(a.c_str(), com.GetParameters().c_str());
			cout << b.c_str() << endl;
			b.clear();
			a.clear();

			//�����
			string report = command.MakeReport();
			TCPSocket::AChar sendbuf;
			sendbuf.assign(report.begin(), report.end());
			client.send(sendbuf);
			sendbuf.clear();
			report.clear();
			cout << report;

		}
		else if (comname == "com3") 
		{
			FileHandler command(com.GetName(), com.GetParameters());
			string a = command.SendFile(com.GetName().c_str());
			TCPSocket::AChar sendbuf;
			sendbuf.assign(a.begin(), a.end());
			client.send(sendbuf);
			sendbuf.clear();
			a.clear();

			//�����
			string report = command.MakeReport();
			cout << report;
		}
		else if(comname == "com4") {
			/*Delete*/
			DelFile command(com.GetName(), com.GetParameters());
			string a = command.Run();
			cout << a << endl;
			a.clear();

			//�����
			string report = command.MakeReport();
			TCPSocket::AChar sendbuf;
			sendbuf.assign(report.begin(), report.end());
			client.send(sendbuf);
			sendbuf.clear();
			report.clear();
			cout << report;
		}

		//������� ������
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