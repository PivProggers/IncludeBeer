// RemoteWorkStation.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "..\Transport\TCPSocket.h"
#include <iostream>
#include "gtest\gtest.h"
#include "..\Client\TestFunctionDeclaration.h"
#include <string>

using namespace std;

//TEST(ClientTest, MethodConnect) {
//	string name = "";
//	string com = "";
//	string report = ClientTesting(6, ' ', ' ');
//	ASSERT_EQ(report.c_str(), "connected");
//}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}