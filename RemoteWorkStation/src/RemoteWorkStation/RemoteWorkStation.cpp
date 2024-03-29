// RemoteWorkStation.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\ClassClient.h"
#include "..\Transport\ClassServer.h"
#include "..\Commands\Command.h"
#include <iostream>
#include "gtest\gtest.h"
#include "..\Client\TestFunctionDeclaration.h"
#include <string>

using namespace std;


TEST(ClientTest, MethodConnectFalse) {
	Client c;
	ASSERT_EQ(c.InitClient("127.0.0.1", 65041), FALSE);
}

TEST(ClientTest, MethodConnectTrue) {
	Client c; Server s;
	s.InitServer("127.0.0.1", 65041);
	ASSERT_EQ(c.InitClient("127.0.0.1", 65041), TRUE);
}

TEST(ClientTest, MethodClose) {
	Client c; Server s;
	s.InitServer("127.0.0.1", 65041);
	c.InitClient("127.0.0.1", 65041);
	c.CloseClient();
	ASSERT_NO_THROW("Client closed connection");
}

TEST(ClientTest, Exception) {
	Client c; Server s;
	s.InitServer("127.0.0.1", 65041);
	c.InitClient("127.0.0.1", 65041);
	
	ASSERT_ANY_THROW(s.InitServer("127.0.0.1", 65041););
}



TEST(ClientTest, pause) {
	system("pause");
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}