#include "pch.h"
#include "../TCPdll/TCP.hpp"
using namespace  std;

TEST(Really, Test)
{
	cout << TestFunc(1);
}

int main(int argc, char** argv)
{
	setlocale(0, "rus");
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}