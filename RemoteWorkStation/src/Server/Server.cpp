#define _CRT_SECURE_NO_WARNINGS

#include "pch.h"
#include "..\Transport\TCPSocket.h"
#include "..\Transport\ClassServer.h"
#include "..\Commands\Command.h"
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <fstream>
using namespace std;

#ifndef OS_WIN
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename), (mode)))==NULL
#endif

int main()
{
	system("COLOR 02");

	Server server; TCPSocket client;

	while (true) {

        string name;
        string prt;
        FILE* config;
        char ip[256];
        if (!fopen_s(&config, "..\\..\\..\\src\\Server\\ipport.cfg", "rb"))
            fgets(ip, 256, config);

        name = ip;
        name.erase(0, 3);  //delete "ip:"
        name.erase(name.end()); //delete '\n'
        fgets(ip, 256, config);
        prt = ip;
        prt.erase(0, 5);  //delete "port:"
        prt.erase(prt.end()); //delete '\n'
        int port = atoi(prt.c_str());
        cout << "ip: " << name << "port: " << port << endl;
      /*  string name = "127.0.0.1";
        int port = 65041;*/

		server.InitServer(name, port);

		if (!server.WorkWithClient(port, server, client)) {
			break;
		}

	}

	return 0;

}