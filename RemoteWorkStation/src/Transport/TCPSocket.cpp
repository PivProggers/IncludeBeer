#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "TCPSocket.h"
#include <exception>

int TCPSocket::init = 0;

TCPSocket::TCPSocket(const TCPSocket & copy)
{
	this->s = copy.s;//�������� �����
	this->iLinks = copy.iLinks;//��� �� �������� ������� ������ �� �����
	this->bound = copy.bound;//� �����
	this->listening = copy.listening;
	this->connected = copy.connected;
	(*iLinks)++;//����������� ������� ������ �� ���� ������
}

TCPSocket & TCPSocket::operator=(const TCPSocket & copy)
{
	if (this->s == copy.s)//���� ����������� ����� ������ ����, �� ������ ������ �� ����
		return *this;

	char buf[10];

	if (0 != ::closesocket(this->s))//��������� ������� �����
		throw std::exception(::_itoa(::WSAGetLastError(), buf, 16));

	delete this->iLinks;//������� ������� ������ �� ������� �����

	this->s = copy.s;//� �������� ��� �����
	this->iLinks = copy.iLinks;//��� �� �������� ������� ������ �� �����
	this->bound = copy.bound;//� �����
	this->listening = copy.listening;
	this->connected = copy.connected;
	(*iLinks)++;//����������� ������� ������ �� ���� ������

	return *this;
}

TCPSocket::TCPSocket(void)
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

TCPSocket::~TCPSocket(void)
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



