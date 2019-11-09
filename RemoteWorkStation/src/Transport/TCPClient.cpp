#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include "TCPClient.h"
#include <exception>

#include <iostream>
using namespace std;

TCPClient::TCPClient(const TCPClient & copy)
{
	this->s = copy.s;//�������� �����
	this->iLinks = copy.iLinks;//��� �� �������� ������� ������ �� �����
	this->bound = copy.bound;//� �����
	this->listening = copy.listening;
	this->connected = copy.connected;
	(*iLinks)++;//����������� ������� ������ �� ���� ������
}

TCPClient::TCPClient(void)
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

TCPClient::~TCPClient(void)
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

bool TCPClient::connect(const sockaddr_in & name)
{
	cout << "try to connect" << endl;
	if (this->bound || this->listening || this->connected)//���� ����� ��� ������������, �� ������ ����������
		throw std::exception("Already in use");
	if (::connect(s, (sockaddr *)&name, sizeof(sockaddr_in)) != 0)
		printf("%d", WSAGetLastError());
	return connected = (0 == ::connect(s, (sockaddr *)&name, sizeof(sockaddr_in)));//����������� �� ������ ������ ���������� name
}

bool TCPClient::connect(const std::string & addr, int port)//����� ����������� ������� �������
{
	sockaddr_in s_in = { 0 };//������������� ��������� ������

	s_in.sin_addr.S_un.S_addr = ::inet_addr(addr.c_str());//����������� ������ � �����
	s_in.sin_family = AF_INET;//��������� ����������
	s_in.sin_port = ::htons(port);//����������� ������� ���� � ����� ��� ������� ����� TCP/IP

	if (s_in.sin_addr.S_un.S_addr == INADDR_NONE)
		throw std::exception("Wrong ip address");

	return connect(s_in);//�����������
}

bool TCPClient::send(const TCPSocket::AChar & inbuf)
{
	if (!this->connected)                                                         
		throw std::exception("Must be connected first");                      

	return inbuf.size() == ::send(s, &inbuf[0], inbuf.size(), 0);//���������� ������ �� ������
}

TCPSocket::AChar TCPClient::receive()//��������� ������
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