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
void print_menu() {
	cout << "What do you want to do?" << endl;
	cout << "1. Run application on the platform" << endl;
	cout << "2. Upload a file on the platform" << endl;
	cout << "3. Download a file from the platform" << endl;
	cout << "4. Delete a file from the platform" << endl;
	cout << "5. Exit" << endl;
	cout << ">";
}

int get_variant(int count) {
	int variant;
	string s; // строка для считывания введённых данных
	getline(cin, s); // считываем строку

					 // пока ввод некорректен, сообщаем об этом и просим повторить его
	while (sscanf(s.c_str(), "%d", &variant) != 1 || variant < 1 || variant > count) {
		cout << "Incorrect input. Try again: "; // выводим сообщение об ошибке
		getline(cin, s); // считываем строку повторно
	}

	return variant;
}

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

	//После вывода на экране предложения, ввести через enter например help dir -- это выведет справку о команде dir в shell'e
	
	int variant;

	//do {
		print_menu(); // выводим меню на экран

		variant = get_variant(5); // получаем номер выбранного пункта меню

		switch (variant) {
		case 1:
			cout << " Input name of the application, please: ";
			getline(cin, command);//для примера "help"
			cout << endl;
			cout << " Input parameters, please: ";
			getline(cin, parameters);
			cout << endl;

			break;
		
			//не реализовано
		case 2:
			cout << " Input name of file, please: ";
			getline(cin, command);//для примера "help"
			cout << endl;
			cout << " Input directory, please: ";
			getline(cin, parameters);
			cout << endl;
			break;
			
			//не реализовано
		case 3:
			cout << " Input  name of file, please: ";
			getline(cin, command);//для примера "help"
			cout << endl;
			cout << " Input directory, please: ";
			getline(cin, parameters);
			cout << endl;
			break;

			//не реализовано
		case 4:
			string s = "del"; string com;
			cout << " Input  name of file, please: ";
			getline(cin, com);
			cout << " Input directory, please: ";
			getline(cin, parameters);
			cout << endl;
			command = s +" "+ parameters + "\\" + com;
			parameters = "";
			cout << command << endl;
			cout << endl;
			break;
		}

	//	if (variant != 5) {

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

			Sleep(1000);
			client.SendDataToServer(buf);//отправляем данные

										 //очистка вектора
			buf.clear();
			system("pause"); // задерживаем выполнение, чтобы пользователь мог увидеть результат выполнения выбранного пункта
		//}

	//} while (variant != 5);

	
	client.CloseClient();
}

