// RemoteWorkStation.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "..\Transport\TCPSocket.h"
#include <iostream>

using namespace std;

int main()
{
	//TCPServer server;
	//TCPClient client; // = s1.accept(25);//ожидаем входящее соединения на 25 порт, хотя можно и вручную сначала bind, потом listen, а потом уже accept
	//
	//// Связываем сокет с адресом localhost и портом 65042
	//server.bind("localhost", 65043);
	// //Начинаем слушать
	//server.listen(true);

	//while (true) {
	//	// Создаём переменную для управления соединения с клиентом
	//	client = server.accept(65043);
	//	TCPSocket::AChar bufrec;
	//	// Читаем переданных клиентом данные
	//	bufrec = client.receive();
	//	// Отправляем клиенту полученную от него же строку
	//	client.send(bufrec);
	//}

	//s2.connect("localhost", 65043);

	//TCPSocket::AChar buf;
	//std::string str("Test string");

	//buf.assign(str.begin(), str.end());//заполняем буфер для передачи
	//s2.send(buf);//отправляем данные
	//
	//

	//for (int i = 0; i < sizeof(buf); ++i)
	//	cout << bufrec[i];

	///////////////////////////////////////////////////////////
	//TCPSocket s;

	//s.connect("127.0.0.1", 8080);//инициируем исходящее соединение

    return 0;
}

