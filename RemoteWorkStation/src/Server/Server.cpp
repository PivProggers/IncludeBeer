#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\TCPServer.h"
#include "..\Transport\TCPClient.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	TCPServer server; TCPClient client;

	// Присоединяемся к серверу
	string name;
	name = "localhost";
	int port = 65042;

	sockaddr_in s_in = { 0 };//заполняем структуру значениями по умолчанию

	s_in.sin_family = AF_INET;
	s_in.sin_addr.S_un.S_addr = INADDR_ANY;
	s_in.sin_port = ::htons(port);//кроме порта

	server.bind(port, &(const sockaddr_in)s_in);
	server.listen(port);
	server.accept(port);

	while (true) {
			TCPSocket::AChar bufrec;
			// Читаем переданных клиентом данные
			bufrec = client.receive();
			// Отправляем клиенту полученную от него же строку
			client.send(bufrec);
		}

	TCPSocket::AChar bufrec;
	// Читаем данные от сервера
	bufrec = server.receive();

	for (int i = 0; i < sizeof(bufrec); ++i)
			cout << bufrec[i];

	return 0;
}

