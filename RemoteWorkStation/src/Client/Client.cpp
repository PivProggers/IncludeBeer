#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "..\Transport\TCPSocket.h"
#include <iostream>

using namespace std;

int main()
{
	TCPSocket client;

	// Присоединяемся к серверу
	client.connect("localhost", 65042);
	char arr[] = { 'A','B', 'C', 'D', 'F' };
	TCPSocket::AChar buf;
	buf.insert(buf.end(), arr, arr + 5);
	client.send(buf);
	
	TCPSocket::AChar bufrec;
	// Читаем данные от сервера
	bufrec = client.receive();
	
	return 0;
}

