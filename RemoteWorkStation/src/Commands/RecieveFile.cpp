#include "pch.h"
#include "Command.h"

string RecieveFile::Run(TCPSocket::AChar buf = {0})
{
#ifdef _WIN32
	std::ofstream out;          // поток для записи
	out.open(_name); // окрываем файл для записи
	char*& result = {0};
	if (out.is_open())
	{
		std::copy(buf.begin(), buf.end(), result);
		out << result;
		_error_report = "1";
		return "1";
	}
	else
	{
		_error_report = "0" + GetLastError();
		return "0";
	}
#else
	string line;
	ifstream in(_name); // окрываем файл для чтения
	if (in.is_open())
	{
		while (getline(in, line))
		{
			std::cout << line << std::endl;
		}
		return "1";
	}
	else
	{
		_error_report = "0" + errno;
		return "0";
	}
	in.close();     // закрываем файл
	return 
#endif
	return "0";
}