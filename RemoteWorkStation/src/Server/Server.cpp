#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
using namespace std;

int main()
{
	system("COLOR 02");
	while (true) {
		TCPSocket server, client;

		// Присоединяемся к серверу
		string name;
		name = "192.168.1.48";
		int port = 65041;

		//sockaddr_in s_in = { 0 };//заполняем структуру значениями по умолчанию
		//s_in.sin_family = AF_INET;
		//s_in.sin_addr.S_un.S_addr = INADDR_ANY;
		//s_in.sin_port = ::htons(port);//кроме порта


		server.bind(name, port);
		//server.bind(port, &(const sockaddr_in)s_in);
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
				break;

			// Отправляем клиенту полученную от него же строку
			client.send(bufrec);
		}

		//вывод сообщения клиента
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		printf("\tCLIENT MESSAGE:\t");

		for (int i = 0; i < bufrec.size(); ++i)
			cout << bufrec[i];
		
		printf("\n");
		SetConsoleTextAttribute(hConsole, 2);

		//очистка буфера
		bufrec.clear();

		//Предлагаем закрыть сервер
		printf("\tContinue using server? (Y/N)\t\n");
		char ch; 
		cin >> ch;
		if (ch == 'N' || ch == 'n') {
			CloseHandle(hConsole);
			return 0;
		}
		else printf("\t__________________________________\t\n\n");
	}

	return 0;
}

