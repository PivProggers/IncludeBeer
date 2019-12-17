#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\ClassClient.h"
#include "..\Commands\Command.h"
#include "TestFunctionDeclaration.h"

using namespace std;

#define countOfConnectionAttempts 5

void ShowFillField(const char* sOutName, const char* sOutParameters,  string & command, string & parameters) {
	cout << sOutName;
	getline(cin, command);//для примера "help"
	cout << sOutParameters;
	getline(cin, parameters);
}

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
//	name = "192.168.1.34"; //"192.168.1.48";
	name = "127.0.0.1";//"192.168.1.36";// "127.0.0.1"; //"192.168.1.48";
	int port = 65041;
	
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
	
	//здесь представлена сериализация команды в XML "out.xml" в debug'e
	//ввод содержимого в xml
	string command, parameters;


	//После вывода на экране предложения, ввести через enter например help dir -- это выведет справку о команде dir в shell'e
	int variant;
	//Command commandGranny;
	do {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 10);

		print_menu(); // выводим меню на экран
		
		variant = get_variant(5); // получаем номер выбранного пункта меню

		switch (variant) {
		case 1 :
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

			//	if (client.IsConnected())
					client.SendDataToServer(buf);//отправляем данные
				buf.clear();//очистка вектора
				break;
			}
		case 2:
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
				string str = ss.str()+"com2"+a;// + "[::]" + a;
				TCPSocket::AChar buf;
				buf.assign(str.begin(), str.end());//заполняем буфер для передачи
				Sleep(1000);

				client.SendDataToServer(buf);//отправляем данные
				buf.clear();//очистка вектора
				break;
			}
		case 3:
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
				buf.assign(str.begin(), str.end());//заполняем буфер для передачи
				Sleep(1000);

				client.SendDataToServer(buf);//отправляем данные
				buf.clear();//очистка вектора
				TCPSocket::AChar recievebuf;
				while (true)
				{
					// Читаем переданных клиентом данные
					recievebuf = client.receive();

					//крутим цикл, пока размер принятного буфера перестанет быть нулем
					if (recievebuf.size() != 0)
						break;
				}

				string filestr;
				filestr.assign(recievebuf.begin(), recievebuf.end());
				comObj.RecieveFile(filestr.c_str(), parameters.c_str());
				filestr.clear();
				recievebuf.clear();//очистка вектора

				break;
			}
		case 4:
			{
				ShowFillField("Input filename: ", " Input full path to file, please: ", command, parameters);
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
		default: //при закрытии клиента сразу после вызова через нажатие 5 вылетает исключение
			{
				client.CloseClient();
				return 0;
			}

		}
		if (variant !=5)
		{ 
			SetConsoleTextAttribute(hConsole, 12);

			////////////////////////////////отчет о работе///////////////////////////////////////////////////////////////
			TCPSocket::AChar bufReport;
			while (true)
			{
				// Читаем переданных клиентом данные
				bufReport = client.receive();

				//крутим цикл, пока размер принятного буфера перестанет быть нулем
				if (bufReport.size() != 0)
				{
					cout << reinterpret_cast<char*>(bufReport.data()) << endl;
					break;
				}
				// Отправляем клиенту полученную от него же строку
				if (!client.send(bufReport)) {
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Server closed connection" << endl;
					SetConsoleTextAttribute(hConsole, 2);
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////

			system("pause"); // задерживаем выполнение, чтобы пользователь мог увидеть результат выполнения выбранного пункта
			system("cls");
		}
	} while (true);

	return 0;
}

//void Ofcskaya(int functia) //cashback na vse i na eto in na eto i na eto i na to
//{
//	//		1/0			    2/1/0				1/0			  4/3/2/1			/object of the class/	/size of file/		?/ count of parts of the file/		/file/
//	//	 whosends	containtsfile/authori	  success		numberOfCommand										
//	TCPSocket::AChar buf = {};
//}


//string ClientTest(int variant, string Command, string parameters)
//{
//	Client client;
//	string name;
//	name = "127.0.0.1";
//	int port = 65041;
//
//	for (int attempt = 1; attempt <= countOfConnectionAttempts; attempt++) {
//		if (client.InitClient(name, port)) {
//			break;
//		}
//		if (attempt == 5) {
//			client.CloseClient();
//			return "Failed with the connection attempts";
//		}
//	}
//	string command, parameters;
//
//	do {
//		switch (variant) {
//		case 1:
//		{
//			RunApplication comObj(command, parameters);
//			stringstream ss;
//			//serialize block
//			{
//				cereal::XMLOutputArchive archive(ss);
//				//записываем данные в узел Command
//				archive(cereal::make_nvp("Command", comObj));
//			}
//
//			//записываем
//			string str = ss.str() + "com1";
//			TCPSocket::AChar buf;
//			buf.assign(str.begin(), str.end());//заполняем буфер для передачи
//			Sleep(1000);
//
//			//	if (client.IsConnected())
//			client.SendDataToServer(buf);//отправляем данные
//			buf.clear();//очистка вектора
//			break;
//		}
//		case 2:
//		{
//			FileHandler comObj(command, parameters);
//
//			stringstream ss;
//			//serialize block
//			{
//				cereal::XMLOutputArchive archive(ss);
//				//записываем данные в узел Command
//				archive(cereal::make_nvp("Command", comObj));
//			}
//			string a = comObj.SendFile(command.c_str());
//
//			//записываем
//			string str = ss.str() + "com2" + a;// + "[::]" + a;
//			TCPSocket::AChar buf;
//			buf.assign(str.begin(), str.end());//заполняем буфер для передачи
//			Sleep(1000);
//
//			client.SendDataToServer(buf);//отправляем данные
//			buf.clear();//очистка вектора
//			break;
//		}
//		case 3:
//		{
//			FileHandler comObj(command, parameters);
//
//			stringstream ss;
//			//serialize block
//			{
//				cereal::XMLOutputArchive archive(ss);
//				archive(cereal::make_nvp("Command", comObj));
//			}
//
//			//записываем
//			string str = ss.str() + "com3";
//			TCPSocket::AChar buf;
//			buf.assign(str.begin(), str.end());//заполняем буфер для передачи
//			Sleep(1000);
//
//			client.SendDataToServer(buf);//отправляем данные
//			buf.clear();//очистка вектора
//			TCPSocket::AChar recievebuf;
//			while (true)
//			{
//				// Читаем переданных клиентом данные
//				recievebuf = client.receive();
//
//				//крутим цикл, пока размер принятного буфера перестанет быть нулем
//				if (recievebuf.size() != 0)
//					break;
//			}
//
//			string filestr;
//			filestr.assign(recievebuf.begin(), recievebuf.end());
//			comObj.RecieveFile(filestr.c_str(), parameters.c_str());
//			filestr.clear();
//			recievebuf.clear();//очистка вектора
//
//			break;
//		}
//		case 4:
//		{
//			DelFile comObj(command, parameters);
//			stringstream ss;
//			//serialize block
//			{
//				cereal::XMLOutputArchive archive(ss);
//				//записываем данные в узел Command
//				archive(cereal::make_nvp("Command", comObj));
//			}
//
//			//записываем
//			string str = ss.str() + "com4";
//			TCPSocket::AChar buf;
//			buf.assign(str.begin(), str.end());//заполняем буфер для передачи
//			Sleep(1000);
//
//			client.SendDataToServer(buf);//отправляем данные
//			buf.clear();//очистка вектор
//			break;
//		}
//		default: //при закрытии клиента сразу после вызова через нажатие 5 вылетает исключение
//		{
//			client.CloseClient();
//			return 0;
//		}
//
//		}
//		if (variant != 5)
//		{
//
//		}
//	} while (true);
//
//	return "Ènded";
//}