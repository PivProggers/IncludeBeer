#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include <exception>
#include <iostream>
using namespace std;

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
		throw std::exception(::itoa(::WSAGetLastError(), buf, 16));

	if (--this->init == 0)//если счётчик сокетов подошёл к концу
		if (0 != ::WSACleanup())//то очищаем работу с сокетами
			throw std::exception("Error at WSACleanup()");
}

bool TCPSocket::bind(int port, const sockaddr_in * name)
{
	if (this->connected || this->bound || this->listening)//нельзя биндить сокет, который уже был присоединён
		throw std::exception("Already used");

	if (name)//если была передана структура sockaddr_in
		return bound = (0 == ::bind(s, (sockaddr *)name, sizeof(sockaddr_in)));//то биндим на основании её данных, иначе

	printf("\tSERVER has been started...\t\n");

	sockaddr_in s_in = { 0 };//заполняем структуру значениями по умолчанию

	s_in.sin_family = AF_INET;
	s_in.sin_addr.S_un.S_addr = INADDR_ANY;
	s_in.sin_port = ::htons(port);//кроме порта

	return bound = (0 == ::bind(s, (sockaddr *)&s_in, sizeof(sockaddr_in)));//и биндим на эту структуру
}

bool TCPSocket::bind(const std::string & addr, int port)//более дружелюбная версия
{
	sockaddr_in s_in = { 0 };//инициализация структуры нулями

	s_in.sin_addr.S_un.S_addr = ::inet_addr(addr.c_str());//преобразуем строку в адрес
	s_in.sin_family = AF_INET;//семейство протоколов
	s_in.sin_port = ::htons(port);//преобразуем порядок байт в слове для формата стека TCP/IP

	if (s_in.sin_addr.S_un.S_addr == INADDR_NONE)
		throw std::exception("Wrong ip address");

	return bind(port, &s_in);
}

bool TCPSocket::listen(int port, int backlog)
{
	char buf[10];

	if (!bound && !this->bind(port))//если сокет не сбинден, нужно сбиндить
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	printf("\tSERVER is waiting for connection...\t\n");


	return listening = (0 == ::listen(s, backlog));//переводим сокет в режим прослушки
}

TCPSocket TCPSocket::accept(int port, sockaddr * addr, int * addrlen)
{
	TCPSocket rs;
	SOCKET ts;
	char buf[10];

	if (this->connected)
		throw std::exception("Already in use");

	if (!this->listening && !this->listen(port))//если сокет ещё не переведён в режим прослушки, переводим его
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	ts = ::accept(s, addr, addrlen);//и ждём входящего соединения

	if (INVALID_SOCKET == rs.s || INVALID_SOCKET == ts)
	{
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));
		printf("invalid socket");
	}
	::closesocket(rs.s);//если дождались, то заменяем сокет, созданый по умолчанию,

	rs.s = ts;//сокетом соедиения
	rs.connected = true;
	printf(rs.connected ? "\tSERVER has new connection: TRUE\t\n" : "\tSERVER has new connection: FALSE\t\n");
	return rs;
}

bool TCPSocket::connect(const sockaddr_in & name)
{
	printf("\tCLIENT is trying to connect...\t\n");
	if (this->bound || this->listening || this->connected)//занят - исключение
	{
		throw std::exception("Already in use");
	}
	connected = (0 == ::connect(s, (sockaddr*)&name, sizeof(name)));//подключаемся со структурой name
	printf(connected ? "\tCLIENT is connected: TRUE...\t\n" : "\tCLIENT is connected: FALSE...\t\n");
	return connected;

}

bool TCPSocket::connect(const std::string & addr, int port)//более дружелюбная версия
{
	sockaddr_in s_in = { 0 };//èíèöèàëèçàöèÿ ñòðóêòóðû íóëÿìè

	s_in.sin_addr.S_un.S_addr = ::inet_addr(addr.c_str());//ïðåîáðàçóåì ñòðîêó â àäðåñ
	s_in.sin_family = AF_INET;//ñåìåéñòâî ïðîòîêîëîâ
	s_in.sin_port = ::htons(port);//ïðåîáðàçóåì ïîðÿäîê áàéò â ñëîâå äëÿ ôîðìàòà ñòåêà TCP/IP

	if (s_in.sin_addr.S_un.S_addr == INADDR_NONE)
		throw std::exception("Wrong ip address");

	return connect(s_in);//ñîåäèíÿåìñÿ
}


bool TCPSocket::send(const TCPSocket::AChar & inbuf)
{
	if (!this->connected)
		throw std::exception("Must be connected first");
	printf("\tCLIENT is sending data...\t\n");
	printf("\tData has been sent...\t\n");

	return inbuf.size() == ::send(s, &inbuf[0], inbuf.size(), 0);//отправляем данные
}

TCPSocket::AChar TCPSocket::receive()//принимаем данные
{
	AChar rval;//массив, в который будем сохранять данные
	AChar::size_type size = 1;//начальный размер массива - 1
	char buf[10];

	printf("\tReceiving data is started...\t\n");

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

	printf("\tAll data received successfully\t\n");

	return rval;//возвращаем буфер
}