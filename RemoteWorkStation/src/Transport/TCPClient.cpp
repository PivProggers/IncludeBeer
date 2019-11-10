#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include "TCPClient.h"
#include <exception>
#include <iostream>

#include <iostream>
using namespace std;

TCPClient::TCPClient(const TCPClient & copy)
{
	this->s = copy.s;//êîïèðóåì ñîêåò
	this->iLinks = copy.iLinks;//òàê æå êîïèðóåì ñ÷¸ò÷èê ññûëîê íà ñîêåò
	this->bound = copy.bound;//è ôëàãè
	this->listening = copy.listening;
	this->connected = copy.connected;
	(*iLinks)++;//óâåëè÷èâàåì ñ÷¸ò÷èê ññûëîê íà îäíó ññûëêó
}

TCPClient::TCPClient(void)
{
	if (!this->init)//åñëè ñèñòåìà ñîêåòîâ íå èíèöèàëèçèðîâàíà, òî ïðîñèñõîäèò èíèöèàëèçàöèÿ
	{
		WSADATA wsaData;

		int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (iResult != NO_ERROR)
			throw std::exception("Error at WSAStartup()");
	}

	init++;//óâåëè÷èâàåì ñ÷¸ò÷èê ñîêåòîâ

	char buf[10];

	s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//ñîçäà¸ì ñîêåò

	if (INVALID_SOCKET == s)//åñëè íå óäàëîñü - áðîñàåì èñêëþ÷åíèå
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	iLinks = new int(1);//èíèöèàëèçèðóåì ñ÷¸ò÷èê ññûëîê íà ýòîò ñîêåò
	bound = listening = connected = false;//èíèöèàëèçèðóåì ôëàãè
}

TCPClient::~TCPClient(void)
{
	if (--(*iLinks) > 0)//åñëè ñ÷¸ò÷èê ññûëîê ïîäîø¸ë ê êîíöó, òî
		return;

	delete iLinks;//óäàëÿåì åãî

	char buf[10];

	if (0 != ::closesocket(s))//çàêðûâàåì ñîêåò
		;//throw std::exception(::itoa(::WSAGetLastError(), buf, 16));

	if (--this->init == 0)//åñëè ñ÷¸ò÷èê ñîêåòîâ ïîäîø¸ë ê êîíöó
		if (0 != ::WSACleanup());//òî î÷èùàåì ðàáîòó ñ ñîêåòàìè
			//throw std::exception("Error at WSACleanup()");
}

bool TCPClient::connect(const sockaddr_in & name)
{
	cout << "try to connect" << endl;
	if (this->bound || this->listening || this->connected)//åñëè ñîêåò óæå èñïîëüçóåòñÿ, òî êèäàåì èñêëþ÷åíèå
	{
		throw std::exception("Already in use");
		std::cout << "ÿ  4 \n";
	}
	std::cout << "ÿ  5 \n";
	connected = (0 == ::connect(s, (sockaddr*)&name, sizeof(name)));//ñîåäèíÿåìñÿ íà îñíîâå äàííûõ ïåðåìåííîé name
	std::cout << connected;
	return connected;

}

//bool TCPClient::connect(const std::string & addr, int port)//áîëåå äðóæåëþáíûé âàðèàíò ôóíêöèè
//{
//	sockaddr_in s_in = { 0 };//èíèöèàëèçàöèÿ ñòðóêòóðû íóëÿìè
//
//	s_in.sin_addr.S_un.S_addr = ::inet_addr(addr.c_str());//ïðåîáðàçóåì ñòðîêó â àäðåñ
//	s_in.sin_family = AF_INET;//ñåìåéñòâî ïðîòîêîëîâ
//	s_in.sin_port = ::htons(port);//ïðåîáðàçóåì ïîðÿäîê áàéò â ñëîâå äëÿ ôîðìàòà ñòåêà TCP/IP
//
//	if (s_in.sin_addr.S_un.S_addr == INADDR_NONE)
//		throw std::exception("Wrong ip address");
//
//	return connect(s_in);//ñîåäèíÿåìñÿ
//}
//

bool TCPClient::send(const TCPSocket::AChar & inbuf)
{
	if (!this->connected)                                                         
		throw std::exception("Must be connected first");                      

	return inbuf.size() == ::send(s, &inbuf[0], inbuf.size(), 0);//îòïðàâëÿåì äàííûå èç áóôåðà
}

TCPSocket::AChar TCPClient::receive()//ïðèíèìàåì äàííûå
{
	AChar rval;//ìàññèâ, â êîòîðûé áóäåì ñîõðàíÿòü äàííûå
	AChar::size_type size = 1;//íà÷àëüíûé ðàçìåð ìàññèâà - 1
	char buf[10];

	if (!this->connected)
		throw std::exception("Must be connected first");

	rval.resize(size);//óñòàíàâëèâàåì íà÷àëüíûé ðàçìåð

	int res = ::recv(s, &rval[0], size, MSG_PEEK);//ïðîáóåì ÷èòàòü äàííûå

	while (res && res != SOCKET_ERROR && size == res)//ïîêà äàííûå ÷èòàþòñÿ
	{
		rval.resize(++size);//óâåëè÷èâàåì ðàçìåð áóôåðà

		res = ::recv(s, &rval[0], size, MSG_PEEK);//ïðîáóåì ÷èòàòü äàííûå
	}

	if (res == SOCKET_ERROR)//åñëè âî âðåìÿ ÷òåíèÿ ïðîèçîøëà îøèáêà, êèäàåì èñêëþ÷åíèå
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	if (res == 0)//åñëè ïîäêëþ÷åíèå áûëî çàêðûòî, òî âîçâðàùàåì ïóñòîé áóôåð
	{
		rval.resize(0);

		return rval;
	}

	rval.resize(--size);//êîððåêòèðóåì ðàçìåð áóôåðà
	::recv(s, &rval[0], size, 0);//÷èòàåì äàííûå

	return rval;//âîçâðàùàåì áóôåð
}