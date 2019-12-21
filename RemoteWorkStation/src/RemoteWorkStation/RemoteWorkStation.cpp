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
	
	ASSERT_ANY_THROW(s.InitServer("127.0.0.1", 65041));
}

TEST(Commands, RunAppReport_succeed) {
	string command = "calc.exe";
	string parameters = "";
	RunApplication app(command, parameters);
	app.Run();
	string a = app.MakeReport();
	string b = "Succesfully runned file\n";
	ASSERT_EQ(a, b);
}

TEST(Commands, RunAppReport_NotInitialized) {
	string command = "";
	string parameters = "calc.exe";
	RunApplication app(command, parameters);
	app.Run();
	string a = app.MakeReport();
	string b = "Name of application wasn't initialized...\n";
	ASSERT_EQ(a, b);
}

TEST(Commands, FileHandler_Send_succeed) {
	string command = "../../../temp/shpory.docx";
	string parameters = "";
	FileHandler sf(command, parameters);
	sf.SendFile(command.c_str());
	string a = sf.MakeReport();
	string b = "Succesfully read file\n";
	ASSERT_EQ(a, b);
}

TEST(Commands, FileHandler_Send_fault) {
	string command = "../../../temp/shpory1.docx";
	string parameters = "";
	FileHandler sf(command, parameters);
	sf.SendFile(command.c_str());
	string a = sf.MakeReport();
	string b = "Failed with reading the file\n";
	ASSERT_EQ(a, b);
}

TEST(Commands, FileHandler_Recieve_succeed) {
	string command = "../../../temp/shpory.docx";
	string parameters = "../../../temp/goodtest.docx";
	FileHandler sf(command, parameters);
	string buf = sf.SendFile(command.c_str());
	string a = sf.MakeReport();
	string b = "Succesfully read file\n";
	ASSERT_EQ(a, b);
	sf.RecieveFile(buf, parameters.c_str());
	 a = sf.MakeReport();
	b = "Succesfully writting file\n";
	ASSERT_EQ(a, b);
}

TEST(Commands, FileHandler_Recieve_fault) {
	string command = "../../../temp/shpory.docx";
	string parameters = "../../../temp/badtest/";
	FileHandler sf(command, parameters);
	string buf = sf.SendFile(command.c_str());
	sf.RecieveFile(buf, parameters.c_str());
	string a = sf.MakeReport();
	string b = "Failed with writting the file\n";
	ASSERT_EQ(a, b);
}

TEST(Commands, DelFile_succeed) {
	string command = "../../../temp/goodtest.docx";
	string parameters = "";
	DelFile df(command, parameters);
	df.Run();
	string a = df.MakeReport();
	string b = "successfully deleted\n";
	ASSERT_EQ(a, b);
}

TEST(Commands, DelFile_fault) {
	string command = "../../../temp/shpory112233.docx";
	string parameters = "";
	DelFile df(command, parameters);
	df.Run();
	string a = df.MakeReport();
	string b = "failed\n";
	ASSERT_EQ(a, b);
}

TEST(AllTest, pause) {
	system("pause");
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}