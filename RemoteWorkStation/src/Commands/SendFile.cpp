#include "pch.h"
#include "Command.h"
using namespace std;

string SendFile::Run()
{
#ifdef _WIN32
	string line;
	ifstream file(_parameters.c_str()); // �������� ���� ��� ������
	if (file.is_open())
	{	
		stringstream buf;
		while (getline(file, line))
		{
			buf << line;
		}
	}
	file.close();     // ��������� ����
#endif
}