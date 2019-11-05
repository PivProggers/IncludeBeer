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
	name = "127.0.0.1";
	
	client.connect(name, 65042);
	char arr[] = { 'A','B', 'C', 'D', 'F' };
	TCPSocket::AChar buf;
	buf.insert(buf.end(), arr, arr + 5);
	client.send(buf);

	TCPSocket::AChar bufrec;
	// Читаем данные от сервера
	bufrec = client.receive();

	return 0;
}

