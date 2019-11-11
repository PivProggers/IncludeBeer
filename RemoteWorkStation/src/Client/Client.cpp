#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	system("COLOR 1C");

	TCPSocket client;

	// Присоединяемся к серверу
	string name;
	//name = "localhost";
	name = "127.0.0.1";
	int port = 65041;

	sockaddr_in s_in = { 0 };//инициализация структуры нулями

	s_in.sin_addr.S_un.S_addr = ::inet_addr(name.c_str());//преобразуем строку в адрес
	s_in.sin_family = AF_INET;//семейство протоколов
	s_in.sin_port = ::htons(port);//преобразуем порядок байт в слове для формата стека TCP/IP


	client.connect(s_in);
	TCPSocket::AChar buf;
	
	std::string str("Test string");

	buf.assign(str.begin(), str.end());//заполняем буфер для передачи

	Sleep(1000);
	client.send((const TCPSocket::AChar)buf);//отправляем данные

	//очистка вектора
	buf.clear();

	system("pause");
	return 0;
}

