#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include <exception>

int TCPSocket::init = 0;

TCPSocket::TCPSocket(const TCPSocket & copy)
{
	this->s = copy.s;//копируем сокет
	this->iLinks = copy.iLinks;//так же копируем счётчик ссылок на сокет
	this->bound = copy.bound;//и флаги
	this->listening = copy.listening;
	this->connected = copy.connected;
	(*iLinks)++;//увеличиваем счётчик ссылок на одну ссылку
}

TCPSocket & TCPSocket::operator=(const TCPSocket & copy)
{
	if (this->s == copy.s)//если присваиваем сокет самому себе, то ничего делать не надо
		return *this;

	char buf[10];

	if (0 != ::closesocket(this->s))//закрываем текущий сокет
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	delete this->iLinks;//удаляем счётчик ссылок на текущий сокет

	this->s = copy.s;//и заменяем его новым
	this->iLinks = copy.iLinks;//так же заменяем счётчик ссылок на сокет
	this->bound = copy.bound;//и флаги
	this->listening = copy.listening;
	this->connected = copy.connected;
	(*iLinks)++;//увеличиваем счётчик ссылок на одну ссылку

	return *this;
}

TCPSocket::TCPSocket(void)
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

TCPSocket::~TCPSocket(void)
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



