// RemoteWorkStation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\Transport\TCPSocket.h"
#include <iostream>

int main()
{
	//TCPSocket s1, s2 = s1.accept(25);//ожидаем входящее соединения на 25 порт, хотя можно и вручную сначала bind, потом listen, а потом уже accept
	//TCPSocket::AChar buf;
	//std::string str("Test string");

	//buf.assign(str.begin(), str.end());//заполняем буфер для передачи
	//s2.send(buf);//отправляем данные
	//buf = s2.receive();

	/////////////////////////////////////////////////////////////////////
	TCPSocket s;

	s.connect("127.0.0.1", 8080);//инициируем исходящее соединение

    return 0;
}

