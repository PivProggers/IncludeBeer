#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include "TCPClient.h"
#include <exception>

#include <iostream>
using namespace std;

TCPClient::TCPClient(const TCPClient & copy)
{
	this->s = copy.s;//копируем сокет
	this->iLinks = copy.iLinks;//так же копируем счётчик ссылок на сокет
	this->bound = copy.bound;//и флаги
	this->listening = copy.listening;
	this->connected = copy.connected;
	(*iLinks)++;//увеличиваем счётчик ссылок на одну ссылку
}

TCPClient::TCPClient(void)
{
	if (!this->init)//если система сокетов не инициализирована, то просисходит инициализация
	{
		WSADATA wsaData;

		int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (iResult != NO_ERROR)
			throw std::exception("Error at WSAStartup()");
	}

	init++;//увеличиваем счётчик сокетов

	char buf[10];

	s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//создаём сокет

	if (INVALID_SOCKET == s)//если не удалось - бросаем исключение
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	iLinks = new int(1);//инициализируем счётчик ссылок на этот сокет
	bound = listening = connected = false;//инициализируем флаги
}

TCPClient::~TCPClient(void)
{
	if (--(*iLinks) > 0)//если счётчик ссылок подошёл к концу, то
		return;

	delete iLinks;//удаляем его

	char buf[10];

	if (0 != ::closesocket(s))//закрываем сокет
		;//throw std::exception(::itoa(::WSAGetLastError(), buf, 16));

	if (--this->init == 0)//если счётчик сокетов подошёл к концу
		if (0 != ::WSACleanup());//то очищаем работу с сокетами
			//throw std::exception("Error at WSACleanup()");
}

bool TCPClient::connect(const sockaddr_in & name)
{
	cout << "try to connect" << endl;
	if (this->bound || this->listening || this->connected)//если сокет уже используется, то кидаем исключение
		throw std::exception("Already in use");
	if (::connect(s, (sockaddr *)&name, sizeof(sockaddr_in)) != 0)
		printf("%d", WSAGetLastError());
	return connected = (0 == ::connect(s, (sockaddr *)&name, sizeof(sockaddr_in)));//соединяемся на основе данных переменной name
}

bool TCPClient::connect(const std::string & addr, int port)//более дружелюбный вариант функции
{
	sockaddr_in s_in = { 0 };//инициализация структуры нулями

	s_in.sin_addr.S_un.S_addr = ::inet_addr(addr.c_str());//преобразуем строку в адрес
	s_in.sin_family = AF_INET;//семейство протоколов
	s_in.sin_port = ::htons(port);//преобразуем порядок байт в слове для формата стека TCP/IP

	if (s_in.sin_addr.S_un.S_addr == INADDR_NONE)
		throw std::exception("Wrong ip address");

	return connect(s_in);//соединяемся
}

bool TCPClient::send(const TCPSocket::AChar & inbuf)
{
	if (!this->connected)                                                         
		throw std::exception("Must be connected first");                      

	return inbuf.size() == ::send(s, &inbuf[0], inbuf.size(), 0);//отправляем данные из буфера
}

TCPSocket::AChar TCPClient::receive()//принимаем данные
{
	AChar rval;//массив, в который будем сохранять данные
	AChar::size_type size = 1;//начальный размер массива - 1
	char buf[10];

	if (!this->connected)
		throw std::exception("Must be connected first");

	rval.resize(size);//устанавливаем начальный размер

	int res = ::recv(s, &rval[0], size, MSG_PEEK);//пробуем читать данные

	while (res && res != SOCKET_ERROR && size == res)//пока данные читаются
	{
		rval.resize(++size);//увеличиваем размер буфера

		res = ::recv(s, &rval[0], size, MSG_PEEK);//пробуем читать данные
	}

	if (res == SOCKET_ERROR)//если во время чтения произошла ошибка, кидаем исключение
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	if (res == 0)//если подключение было закрыто, то возвращаем пустой буфер
	{
		rval.resize(0);

		return rval;
	}

	rval.resize(--size);//корректируем размер буфера
	::recv(s, &rval[0], size, 0);//читаем данные

	return rval;//возвращаем буфер
}