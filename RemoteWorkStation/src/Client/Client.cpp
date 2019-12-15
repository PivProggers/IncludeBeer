#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\ClassClient.h"
#include "..\Commands\Command.h"
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
	name = "192.168.1.34"; //"192.168.1.48";
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
				RunApplication* comObj = new RunApplication(command, parameters);
				break;
			}
		case 2:
			{
				ShowFillField(" Input name of file, please: ", " Input directory, please: ", command, parameters);
		//		SendFile* comObj = new SendFile(command, parameters);
				break;
			}
		case 3:
			{
				ShowFillField(" Input  name of file, please: ", " Input directory, please: ", command, parameters);
		//		RecieveFile* comObj = new RecieveFile(command, parameters);
				break;
			}
		case 4:
			{
				ShowFillField(" Input  name of file, please: ", " Input directory, please: ", command, parameters);
		//		DelFile* comObj = new DelFile(command, parameters);
				
				/*string s = "del"; string com;
				ShowFillField(" Input  name of file, please: ", " Input directory, please: ", com, parameters);
				command = s +" "+ parameters + "\\" + com;
				parameters = "";
				cout << command << endl;
				cout << endl;*/
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

			//создаем объект класса из полученных данных и открываем поток на запись файла
			Command comObj(command, parameters);
			//std::ofstream file("out.xml");
			stringstream ss;

			//serialize block
			{
				cereal::XMLOutputArchive archive(ss);
				//записываем данные в узел Command
				archive(cereal::make_nvp("Command", comObj));
			}
			//записываем
			string str = ss.str();
			//file.flush();

			TCPSocket::AChar buf;
			//std::string str("Executing command");
			buf.assign(str.begin(), str.end());//заполняем буфер для передачи

			Sleep(1000);
			client.SendDataToServer(buf);//отправляем данные

			TCPSocket::AChar recBuf = client.receive();

			buf.clear();//очистка вектора
			delete &comObj;
			system("pause"); // задерживаем выполнение, чтобы пользователь мог увидеть результат выполнения выбранного пункта

			system("cls");
		}
		/*else
		{

			client.CloseClient();
			return 0;
		}*/
	} while (true);

	return 0;
}

void Ofcskaya(int functia) //cashback na vse i na eto in na eto i na eto i na to
{
	//		1/0			    2/1/0				1/0			  4/3/2/1			/object of the class/	/size of file/		?/ count of parts of the file/		/file/
	//	 whosends	containtsfile/authori	  success		numberOfCommand										
	TCPSocket::AChar buf = {};
}