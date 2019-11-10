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
	TCPServer server ; TCPClient client1;

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
	
	TCPSocket::AChar bufrec;
	int len = 1024;
	cout << 1 << endl;
	while (true) 
	{
		cout << 1.5 << endl;
		TCPServer client( server.accept(port));
		cout << 1.9 << endl;
		// Читаем переданных клиентом данные
		bufrec = client.receive();
		cout << 2 << endl;
		if (bufrec.size() != 0)
			break;
		// Отправляем клиенту полученную от него же строку
		//client.send(bufrec);
	}
	cout << 3 << endl;
	for (int i = 0; i < sizeof(bufrec); ++i)
			cout << bufrec[i];
	//TCPSocket::AChar bufrec;
	//// Читаем данные от сервера
	//bufrec = server.receive();

	

	return 0;
}

