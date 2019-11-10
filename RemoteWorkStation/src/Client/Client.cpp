#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\TCPClient.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	TCPClient client;

	// Присоединяемся к серверу
	string name;
	//name = "localhost";
	name = "127.1.1.255";
	int port = 65041;

	sockaddr_in s_in = { 0 };//инициализация структуры нулями

	s_in.sin_addr.S_un.S_addr = ::inet_addr(name.c_str());//преобразуем строку в адрес
	s_in.sin_family = AF_INET;//семейство протоколов
	s_in.sin_port = ::htons(port);//преобразуем порядок байт в слове для формата стека TCP/IP


	cout << "then will be connection try" << endl;
	client.connect(s_in);
	cout << "я  1";
	TCPSocket::AChar buf;
	
	std::string str("Test string");

	buf.assign(str.begin(), str.end());//заполняем буфер для передачи


	cout << "я  2";

	client.send((const TCPSocket::AChar)buf);//отправляем данные
	cout << "я  3";
	

	return 0;
}

