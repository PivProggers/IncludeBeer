#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\ClassClient.h"
#include "..\Commands\Command.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

//ЭТО РАБОЧАЯ ВЕРСИЯ ТОЛЬКО С КЛАССОМ TCPSocket

//int main() 
//{
//	system("COLOR 1C");
//
//	TCPSocket client;
//
//	// Присоединяемся к серверу
//	string name;
//	//name = "localhost";
//	name = "127.0.0.1";//"192.168.1.48";
//	int port = 65041;
//
//	client.connect(name, port);
//	TCPSocket::AChar buf;
//	
//	std::string str("Test string");
//
//	buf.assign(str.begin(), str.end());//заполняем буфер для передачи
//
//	Sleep(1000);
//	client.send((const TCPSocket::AChar)buf);//отправляем данные
//
//	//очистка вектора
//	buf.clear();
//
//	system("pause");
//	return 0;
//}

/*_______________________________________________________________*/


//ЭТО ВЕРСИЯ С КЛАССОМ Client

int main() 
{
	system("COLOR 0C");

	Client client;
	// Присоединяемся к серверу
	string name;
	//name = "localhost";
	name = "127.0.0.1"; //"192.168.1.48";
	int port = 65041;



	client.InitClient(name, port);


		//здесь представлена сериализация команды в XML "out.xml" в debug'e
	//ввод содержимого в xml
	string command, parameters;


	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);

	cout << " Input your command: first string -- name of command, second string -- name of parameters" << endl;
	cout << " Input name of command, please: "; 
	cin >> command; //для примера "help"
	cout << endl;
	cout << " Input parameters, please: ";
	cin >> parameters; //для примера "dir"
	cout << endl;

	SetConsoleTextAttribute(hConsole, 12);

	//cсоздаем объект класса из полученных данных и открываем поток на запись файла
	Command com(command, parameters);
	//std::ofstream file("out.xml");
	stringstream ss;
	//serialize block
	{
		cereal::XMLOutputArchive archive(ss);
		//записываем данные в узел Command
		archive(cereal::make_nvp("Command", com));
	}
	//записываем
	string str = ss.str();
	//file.flush();

	TCPSocket::AChar buf;
	//std::string str("Executing command");
	buf.assign(str.begin(), str.end());//заполняем буфер для передачи

	//Нужно придумать логику передачи на сервер и исполнения там
		//На данном этапе исполнение происходит непосредственно на клиенте или сервере, в зависимости от того, куда следующий код вставить
		//После вывода на экране предложения, ввести через enter например help dir -- это выведет справку о команде dir в shell'e
										/*string one, two;
										cout << "\tInput command then params" << endl;
										cin >> one; cin >> two;
										RunAppliсation command(one.c_str(), two.c_str());*/
	



	Sleep(1000);
	client.SendDataToServer(buf);//отправляем данные

	//очистка вектора
	buf.clear();

	system("pause");

	client.CloseClient();
}

