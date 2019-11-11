#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\ClassClient.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//ЭТО РАБОЧАЯ ВЕРСИЯ ТОЛЬКО С КЛАССОМ TCPSocket

//int main() 
//{
//	system("COLOR 1C");
//
//	TCPSocket client;
//
//	// Присоединяемся к серверу
//	string name;
//	//name = "localhost";
//	name = "127.0.0.1";//"192.168.1.48";
//	int port = 65041;
//
//	client.connect(name, port);
//	TCPSocket::AChar buf;
//	
//	std::string str("Test string");
//
//	buf.assign(str.begin(), str.end());//заполняем буфер для передачи
//
//	Sleep(1000);
//	client.send((const TCPSocket::AChar)buf);//отправляем данные
//
//	//очистка вектора
//	buf.clear();
//
//	system("pause");
//	return 0;
//}

/*_______________________________________________________________*/


//ЭТО ВЕРСИЯ С КЛАССОМ Client

int main() 
{
	system("COLOR 1C");

	Client client;
	// Присоединяемся к серверу
	string name;
	//name = "localhost";
	name = "127.0.0.1"; //"192.168.1.48";
	int port = 65041;



	client.InitClient(name, port);


	TCPSocket::AChar buf;
	std::string str("Test string");
	buf.assign(str.begin(), str.end());//заполняем буфер для передачи

	Sleep(1000);
	client.SendDataToServer(buf);//отправляем данные

	//очистка вектора
	buf.clear();

	system("pause");

	client.CloseClient();
}

