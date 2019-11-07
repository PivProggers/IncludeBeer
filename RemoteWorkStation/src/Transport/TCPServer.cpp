#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include "TCPServer.h"
#include <exception>



TCPServer::TCPServer(void)
{
	if (!this->init)//���� ������� ������� �� ����������������, �� ����������� �������������
	{
		WSADATA wsaData;

		int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (iResult != NO_ERROR)
			throw std::exception("Error at WSAStartup()");
	}

	init++;//����������� ������� �������

	char buf[10];

	s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//������ �����

	if (INVALID_SOCKET == s)//���� �� ������� - ������� ����������
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	iLinks = new int(1);//�������������� ������� ������ �� ���� �����
	bound = listening = connected = false;//�������������� �����
}

TCPServer::~TCPServer(void)
{
	if (--(*iLinks) > 0)//���� ������� ������ ������� � �����, ��
		return;

	delete iLinks;//������� ���

	char buf[10];

	if (0 != ::closesocket(s))//��������� �����
		;//throw std::exception(::itoa(::WSAGetLastError(), buf, 16));

	if (--this->init == 0)//���� ������� ������� ������� � �����
		if (0 != ::WSACleanup());//�� ������� ������ � ��������
			//throw std::exception("Error at WSACleanup()");
}



bool TCPServer::bind(int port, const sockaddr_in * name)
{
	if (this->connected || this->bound || this->listening)//������ ������� �����, ������� ��� ��� ����������
		throw std::exception("Already used");

	if (name)//���� ���� �������� ��������� sockaddr_in
		return bound = (0 == ::bind(s, (sockaddr *)name, sizeof(sockaddr_in)));//�� ������ �� ��������� � ������, �����

	sockaddr_in s_in = { 0 };//��������� ��������� ���������� �� ���������

	s_in.sin_family = AF_INET;
	s_in.sin_addr.S_un.S_addr = INADDR_ANY;
	s_in.sin_port = ::htons(port);//����� �����

	return bound = (0 == ::bind(s, (sockaddr *)&s_in, sizeof(sockaddr_in)));//� ������ �� ��� ���������
}

bool TCPServer::bind(const std::string & addr, int port)//����� ����������� ������
{
	sockaddr_in s_in = { 0 };//������������� ��������� ������

	s_in.sin_addr.S_un.S_addr = ::inet_addr(addr.c_str());//����������� ������ � �����
	s_in.sin_family = AF_INET;//��������� ����������
	s_in.sin_port = ::htons(port);//����������� ������� ���� � ����� ��� ������� ����� TCP/IP

	if (s_in.sin_addr.S_un.S_addr == INADDR_NONE)
		throw std::exception("Wrong ip address");

	return bind(port, &s_in);
}

bool TCPServer::listen(int port, int backlog)
{
	char buf[10];

	if (!bound && !this->bind(port))//���� ����� �� �������, ����� ��������
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	return listening = (0 == ::listen(s, backlog));//��������� ����� � ����� ���������
}

 TCPServer TCPServer::accept(int port, sockaddr * addr, int * addrlen)
{
	TCPServer rs;
	SOCKET ts;
	char buf[10];

	if (this->connected)
		throw std::exception("Already in use");

	if (!this->listening && !this->listen(port))//���� ����� ��� �� �������� � ����� ���������, ��������� ���
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	ts = ::accept(s, addr, addrlen);//� ��� ��������� ����������

	if (INVALID_SOCKET == rs.s)
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	::closesocket(rs.s);//���� ���������, �� �������� �����, �������� �� ���������,

	rs.s = ts;//������� ���������
	rs.connected = true;

	return rs;
}

 TCPSocket::AChar TCPServer::receive()//��������� ������
 {
	 AChar rval;//������, � ������� ����� ��������� ������
	 AChar::size_type size = 1;//��������� ������ ������� - 1
	 char buf[10];

	 if (!this->connected)
		 throw std::exception("Must be connected first");

	 rval.resize(size);//������������� ��������� ������

	 int res = ::recv(s, &rval[0], size, MSG_PEEK);//������� ������ ������

	 while (res && res != SOCKET_ERROR && size == res)//���� ������ ��������
	 {
		 rval.resize(++size);//����������� ������ ������

		 res = ::recv(s, &rval[0], size, MSG_PEEK);//������� ������ ������
	 }

	 if (res == SOCKET_ERROR)//���� �� ����� ������ ��������� ������, ������ ����������
		 throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	 if (res == 0)//���� ����������� ���� �������, �� ���������� ������ �����
	 {
		 rval.resize(0);

		 return rval;
	 }

	 rval.resize(--size);//������������ ������ ������
	 ::recv(s, &rval[0], size, 0);//������ ������

	 return rval;//���������� �����
 }