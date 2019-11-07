#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include "TCPServer.h"
#include <exception>



TCPServer::TCPServer(void)
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

TCPServer::~TCPServer(void)
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



bool TCPServer::bind(int port, const sockaddr_in * name)
{
	if (this->connected || this->bound || this->listening)//нельзя биндить сокет, который уже был присоединён
		throw std::exception("Already used");

	if (name)//если была передана структура sockaddr_in
		return bound = (0 == ::bind(s, (sockaddr *)name, sizeof(sockaddr_in)));//то биндим на основании её данных, иначе

	sockaddr_in s_in = { 0 };//заполняем структуру значениями по умолчанию

	s_in.sin_family = AF_INET;
	s_in.sin_addr.S_un.S_addr = INADDR_ANY;
	s_in.sin_port = ::htons(port);//кроме порта

	return bound = (0 == ::bind(s, (sockaddr *)&s_in, sizeof(sockaddr_in)));//и биндим на эту структуру
}

bool TCPServer::bind(const std::string & addr, int port)//более дружелюбная версия
{
	sockaddr_in s_in = { 0 };//инициализация структуры нулями

	s_in.sin_addr.S_un.S_addr = ::inet_addr(addr.c_str());//преобразуем строку в адрес
	s_in.sin_family = AF_INET;//семейство протоколов
	s_in.sin_port = ::htons(port);//преобразуем порядок байт в слове для формата стека TCP/IP

	if (s_in.sin_addr.S_un.S_addr == INADDR_NONE)
		throw std::exception("Wrong ip address");

	return bind(port, &s_in);
}

bool TCPServer::listen(int port, int backlog)
{
	char buf[10];

	if (!bound && !this->bind(port))//если сокет не сбинден, нужно сбиндить
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	return listening = (0 == ::listen(s, backlog));//переводим сокет в режим прослушки
}

 TCPServer TCPServer::accept(int port, sockaddr * addr, int * addrlen)
{
	TCPServer rs;
	SOCKET ts;
	char buf[10];

	if (this->connected)
		throw std::exception("Already in use");

	if (!this->listening && !this->listen(port))//если сокет ещё не переведён в режим прослушки, переводим его
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	ts = ::accept(s, addr, addrlen);//и ждём входящего соединения

	if (INVALID_SOCKET == rs.s)
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	::closesocket(rs.s);//если дождались, то заменяем сокет, созданый по умолчанию,

	rs.s = ts;//сокетом соедиения
	rs.connected = true;

	return rs;
}

 TCPSocket::AChar TCPServer::receive()//принимаем данные
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