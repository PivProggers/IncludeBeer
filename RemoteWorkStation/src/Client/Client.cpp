#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\ClassClient.h"
#include "..\Commands\Command.h"
#include "TestFunctionDeclaration.h"


using namespace std;

#define countOfConnectionAttempts 5

void ShowFillField(const char* sOutName, const char* sOutParameters,  string & command, string & parameters) {
	cout << sOutName;
	getline(cin, command);
	if (sOutParameters != "") {
		cout << sOutParameters;
		getline(cin, parameters);
	}
	else parameters = "";
}

void ShowFillFieldInitialization(const char* sOutName, const char* sOutPort, string& name_ip, int& port) {
	cout << sOutName;
	getline(cin, name_ip);
	cout << sOutPort;
	cin >> port;
	cin.ignore(32767, '\n');
	cout << endl;
}

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
	SetConsoleOutputCP(1251);
	refresh:
	string name;
	int port;
	ShowFillFieldInitialization("Input IP-address of server, please: ", "Input number of port, please: ", name, port);

	Client client;
	//int port = 65041;
	
	for(int attempt = 1; attempt <= countOfConnectionAttempts; attempt++) {
		cout << "Connection attempt #" << attempt << endl;
		if (client.InitClient(name, port)) {
			break;
		}
		if (attempt == 5) {
			cout << "Unable to connect to server!" << endl;
			return client.CloseClient();
		}
	}
	
	string command, parameters;
	int variant;

	do {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

		print_menu(); // выводим меню на экран
		
		variant = get_variant(5); // получаем номер выбранного пункта меню

		switch (variant) {
			
		case 1 : //Запуск приложения на удаленной системе
			{
				ShowFillField(" Input name of the application, please: ", " Input parameters, please: ", command, parameters);
				RunApplication comObj(command, parameters);
				stringstream ss;

				//serialize block
				{
					cereal::XMLOutputArchive archive(ss);
					//записываем данные в узел Command
					archive(cereal::make_nvp("Command", comObj));
				}

				//записываем
				string str = ss.str() + "com1";

				TCPSocket::AChar buf;
				buf.assign(str.begin(), str.end());//заполняем буфер для передачи

				Sleep(1000);
				client.SendDataToServer(buf);//отправляем данные
				buf.clear();//очистка вектора
				break;
			}
		case 2: //Выгрузка файла на удаленную систему
			{
				ShowFillField(" Input full path to file to upload, please: ", " Input full path to save, please: ", command, parameters);
				FileHandler comObj(command, parameters);
				
				stringstream ss;
				//serialize block
				{
					cereal::XMLOutputArchive archive(ss);
					//записываем данные в узел Command
					archive(cereal::make_nvp("Command", comObj));
				}
				string a = comObj.SendFile(command.c_str());
				//записываем
				string str = ss.str() + "com2" + a;
				TCPSocket::AChar buf;
				buf.assign(str.begin(), str.end());//заполняем буфер для передачи
				Sleep(1000);

				client.SendDataToServer(buf);//отправляем данные
				buf.clear();//очистка вектора
				break;
			}
		case 3: //Загрузка файла с удаленной системы
			{
				ShowFillField(" Input full path to file on server, please: ", " Input full path to save, please: ", command, parameters);
				FileHandler comObj(command, parameters);
				
				stringstream ss;
				//serialize block
				{
					cereal::XMLOutputArchive archive(ss);
					//записываем данные в узел Command
					archive(cereal::make_nvp("Command", comObj));
				}

				//записываем
				string str = ss.str() + "com3";
				TCPSocket::AChar buf;
				buf.assign(str.begin(), str.end()); //заполняем буфер для передачи
				Sleep(1000);

				client.SendDataToServer(buf); //отправляем данные
				buf.clear(); //очистка вектора
				TCPSocket::AChar recievebuf;
				while (true) 
				{
					// Читаем переданных клиентом данные
					recievebuf = client.receive();
					if (recievebuf.size() == 1) {
						system("pause");
						system("cls"); //очистка экрана
						goto refresh;
					}

					//крутим цикл, пока размер принятного буфера перестанет быть нулем
					if (recievebuf.size() != 0)
						break;
				}

				string filestr;											// записываем в строчку файл,
				filestr.assign(recievebuf.begin(), recievebuf.end());	//	полученный с сервера

				comObj.RecieveFile(filestr, parameters.c_str()); // запись из полученной строки в файл 

				filestr.clear(); //очистка строки
				recievebuf.clear(); //очистка вектора

				system("pause"); // задерживаем выполнение, чтобы пользователь мог увидеть результат выполнения выбранного пункта
				system("cls"); //очистка экрана
				break;
			}
		case 4:	//Удаление файла с удаленной системы
			{
				ShowFillField(" Input full path to file, please: ", "", command, parameters);

				DelFile comObj(command, parameters);
				stringstream ss;
				//serialize block
				{
					cereal::XMLOutputArchive archive(ss);
					//записываем данные в узел Command
					archive(cereal::make_nvp("Command", comObj));
				}

				//записываем
				string str = ss.str() + "com4";
				TCPSocket::AChar buf;
				buf.assign(str.begin(), str.end());//заполняем буфер для передачи
				Sleep(1000);

				client.SendDataToServer(buf);//отправляем данные
				buf.clear();//очистка вектор
				break;
			}
		default:
			{
				return client.CloseClient();
			}

		}
		if (variant !=5 && variant !=3)
		{ 
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

			////////////////////////////////отчет о работе////////////////////////////////
			TCPSocket::AChar bufReport;
			while (true)
			{
				// Читаем переданных клиентом данные
				bufReport = client.receive();
				if (bufReport.size() == 1) {
					system("pause");
					system("cls"); //очистка экрана
					goto refresh;
				}

				//крутим цикл, пока размер принятного буфера перестанет быть нулем
				if (bufReport.size() != 0)
				{
					cout << reinterpret_cast<char*>(bufReport.data()) << endl;
					break;
				}
				// Отправляем клиенту полученную от него же строку
				if (!client.send(bufReport)) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					cout << "Server closed connection" << endl;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////

			system("pause"); // задерживаем выполнение, чтобы пользователь мог увидеть результат выполнения выбранного пункта
			system("cls"); //очистка экрана
		}
	} while (true);
	
	return 0;
}