#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include <exception>
#include <iostream>
#include <Windows.h>

#ifndef OS_WIN
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#include <stdexcept>
	#include <sys/socket.h> 
	#include <sys/types.h>
	#include <unistd.h> //for close
	#include <arpa/inet.h> //inet_addr
#endif

using namespace std;

int TCPSocket::init = 0;

#ifndef OS_WIN
	char* itoa(char* str, int num)
	{
		sprintf(str, "%d", num);
		return str;
	}
#endif

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

#ifdef OS_WIN
	if (0 != ::closesocket(this->s))//закрываем текущий сокет
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));
#else
	if (0 != close(this->s))//закрываем текущий сокет
		throw runtime_error(itoa(buf, errno));
#endif
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
#ifdef OS_WIN
	if (!this->init)//если система сокетов не инициализирована, то просисходит инициализация
	{
		WSADATA wsaData;

		int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (iResult != NO_ERROR)
			throw std::exception("Error at WSAStartup()");
	}
#endif
	init++;//увеличиваем счётчик сокетов

	char buf[10];

	s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//создаём сокет

	if (INVALID_SOCKET == s)//если не удалось - бросаем исключение
		#ifdef OS_WIN
			throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));
		#else
			throw runtime_error(itoa(buf, errno));
		#endif
	iLinks = new int(1);//инициализируем счётчик ссылок на этот сокет
	bound = listening = connected = false;//инициализируем флаги
}

TCPSocket::~TCPSocket(void)
{
	if (--(*iLinks) > 0)//если счётчик ссылок подошёл к концу, то
		return;

	delete iLinks;//удаляем его

	char buf[10];
	#ifdef OS_WIN
		if (0 != ::closesocket(s))//закрываем сокет
			throw std::exception(::itoa(::WSAGetLastError(), buf, 16));
		if (--this->init == 0)//если счётчик сокетов подошёл к концу
			if (0 != ::WSACleanup())//то очищаем работу с сокетами
				throw std::exception("Error at WSACleanup()");
	#else
		if (0 != ::close(s))//закрываем сокет
			throw runtime_error(itoa(buf, errno));
	#endif
	
}

bool TCPSocket::bind(int port, const sockaddr_in * name)
{
	if (this->connected || this->bound || this->listening)//нельзя биндить сокет, который уже был присоединён
		#ifdef OS_WIN
			throw std::exception("Already used");
		#else
			throw runtime_error("Already used");
		#endif

	if (name) {//если была передана структура sockaddr_in
		cout << "\tSERVER has been started...\t" << endl;

		return bound = (0 == ::bind(s, (sockaddr *)name, sizeof(sockaddr_in)));//то биндим на основании её данных, иначе
	}

	sockaddr_in s_in = { 0 };//заполняем структуру значениями по умолчанию

	s_in.sin_family = AF_INET;
#ifdef OS_WIN
	s_in.sin_addr.S_un.S_addr = INADDR_ANY;
#else
	s_in.sin_addr.s_addr = INADDR_ANY;
#endif
	s_in.sin_port = ::htons(port);//кроме порта

	cout << "\tSERVER has been started...\t" << endl;

	return bound = (0 == ::bind(s, (sockaddr *)&s_in, sizeof(sockaddr_in)));//и биндим на эту структуру
}

bool TCPSocket::bind(const std::string & addr, int port)//более дружелюбная версия
{
	sockaddr_in s_in = { 0 };//инициализация структуры нулями
#ifdef OS_WIN
	s_in.sin_addr.S_un.S_addr = ::inet_addr(addr.c_str());//преобразуем строку в адрес
#else
	s_in.sin_addr.s_addr = ::inet_addr(addr.c_str());//преобразуем строку в адрес
#endif

	s_in.sin_family = AF_INET;//семейство протоколов
	s_in.sin_port = ::htons(port);//преобразуем порядок байт в слове для формата стека TCP/IP
#ifdef OS_WIN
	if (s_in.sin_addr.S_un.S_addr == INADDR_NONE)
		throw std::exception("Wrong ip address");
#else
	if (s_in.sin_addr.s_addr == INADDR_NONE)
		throw runtime_error("Wrong ip address");
#endif

	return bind(port, &s_in);
}

bool TCPSocket::listen(int port, int backlog)
{
	char buf[10];

	if (!bound && !this->bind(port))//если сокет не сбинден, нужно сбиндить
		#ifdef OS_WIN
			throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));
		#else 
			throw runtime_error(itoa(buf, errno));
		#endif
	cout << "\tSERVER is waiting for connection...\t" << endl;


	return listening = (0 == ::listen(s, backlog));//переводим сокет в режим прослушки
}

TCPSocket TCPSocket::accept(int port, sockaddr * addr, int * addrlen)
{
	TCPSocket rs;
	#ifdef OS_WIN
		SOCKET ts;
	#else
		int ts; //linux socket 
	#endif 
	char buf[10];

	if (this->connected)
#ifdef OS_WIN
		throw std::exception("Already in use");
#else
		throw runtime_error("Already in use");
#endif

	if (!this->listening && !this->listen(port))//если сокет ещё не переведён в режим прослушки, переводим его
#ifdef OS_WIN
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));
#else
		throw runtime_error(itoa(buf, errno));
#endif
	ts = ::accept(s, addr, addrlen);//и ждём входящего соединения

	if (INVALID_SOCKET == rs.s || INVALID_SOCKET == ts)
	{
#ifdef OS_WIN
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));
		printf("invalid socket");
#else
		throw runtime_error(itoa(buf, errno));
		printf("invalid socket");
#endif
	}
#ifdef OS_WIN
	::closesocket(rs.s);//если дождались, то заменяем сокет, созданый по умолчанию,
#else
	::close(rs.s);//если дождались, то заменяем сокет, созданый по умолчанию,
#endif
	rs.s = ts;//сокетом соедиения
	rs.connected = true;
	printf(rs.connected ? "\tSERVER has new connection: TRUE\t\n" : "\tSERVER has new connection: FALSE\t\n");
	return rs;
}

bool TCPSocket::connect(const sockaddr_in & name)
{
	cout << "\tCLIENT is trying to connect...\t" << endl;
	if (this->bound || this->listening || this->connected)//занят - исключение
	{
#ifdef OS_WIN
		throw std::exception("Already in use");
#else
		throw runtime_error("Already in use");
#endif
	}
	connected = (0 == ::connect(s, (sockaddr*)&name, sizeof(name)));//подключаемся со структурой name
	
	if (connected) {
		#ifdef OS_WIN
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		#endif
		cout << "\tCLIENT is connected: TRUE...\t" << endl;
	}
	else {
	#ifdef OS_WIN
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	#endif
		cout << "\tCLIENT is connected: FALSE...\t" << endl;
	}
#ifdef OS_WIN
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#endif
	return connected;

}

bool TCPSocket::connect(const std::string & addr, int port)//более дружелюбная версия
{
	sockaddr_in s_in = { 0 };
#ifdef OS_WIN
	s_in.sin_addr.S_un.S_addr = ::inet_addr(addr.c_str());
#else 
	s_in.sin_addr.s_addr = ::inet_addr(addr.c_str());//ïðåîáðàçóåì ñòðîêó â àäðåñ
#endif
	s_in.sin_family = AF_INET;
	s_in.sin_port = ::htons(port);

	if (s_in.sin_addr.S_un.S_addr == INADDR_NONE)
#ifdef OS_WIN
		throw std::exception("Wrong ip address");
#else
		throw runtime_error("Wrong ip address");
#endif

	return connect(s_in);//ñîåäèíÿåìñÿ
}


bool TCPSocket::send(const TCPSocket::AChar & inbuf)
{
	if (!this->connected)
#ifdef OS_WIN
		throw std::exception("Must be connected first");
#else
		throw runtime_error("Must be connected first");
#endif
	if (inbuf.size() != 0) {
		return inbuf.size() == ::send(s, &inbuf[0], inbuf.size(), 0);//отправляем данные
	}
	else return false;
}

TCPSocket::AChar TCPSocket::receive()//принимаем данные
{
	AChar rval;//массив, в который будем сохранять данные
	AChar::size_type size = 1;//начальный размер массива - 1
	char buf[10];

	cout << "\tReceiving data is started...\t" << endl;
	if (!this->connected)
#ifdef OS_WIN
		throw std::exception("Must be connected first");
#else
		throw runtime_error("Must be connected first");
#endif
	rval.resize(size);//устанавливаем начальный размер

	int res = ::recv(s, &rval[0], size, MSG_PEEK);//пробуем читать данные

	while (res && res != SOCKET_ERROR && size == res)//пока данные читаются
	{
		rval.resize(++size);//увеличиваем размер буфера

		res = ::recv(s, &rval[0], size, MSG_PEEK);//пробуем читать данные
	}
#ifdef OS_WIN
	try {
		if (res == SOCKET_ERROR)//если во время чтения произошла ошибка, кидаем исключение
			throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));
	}
	catch (exception ex) {
		cout << "Connection fault...Closed: " << ex.what()<<endl;
		return rval;
	}
#else
	if (res == SOCKET_ERROR)//если во время чтения произошла ошибка, кидаем исключение
		throw runtime_error(itoa(buf, errno));
#endif

	if (res == 0)//если подключение было закрыто, то возвращаем пустой буфер
	{
		rval.resize(0);

		return rval;
	}

	rval.resize(--size);//корректируем размер буфера
	::recv(s, &rval[0], size, 0);//читаем данные
#ifdef OS_WIN
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
#endif
	cout << "\tAll data received successfully\t" << endl;
#ifdef OS_WIN
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#endif

	return rval;//возвращаем буфер
}