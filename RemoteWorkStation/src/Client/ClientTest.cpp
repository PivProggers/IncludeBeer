#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\ClassClient.h"
#include "..\Commands\Command.h"
#include "TestFunctionDeclaration.h"



#ifdef TEST_
int port = 65041;
const char* nameIp = "127.0.0.1";

TEST(ClientConnect, MethodConnect)
{
	string command = "";
	string assertresult = "connected";
	string parameters = "";
	string res = ClientTesting(6, command, parameters, port, nameIp);
	ASSERT_EQ(res, assertresult);
}
TEST(ClientConnect, MethodCommandRun)
{
	string command = "calc.exe";
	string assertresult = "Succesfully runned file\n";
	string parameters = "";
	string res = ClientTesting(1, command, parameters, port, nameIp);
	ASSERT_EQ(res, assertresult);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif