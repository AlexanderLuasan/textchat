#include "client.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include "chatfunc.h"
#pragma comment(lib,"ws2_32.lib")

using namespace std;
void client()
{
	//setup
	WSADATA wsData;//initialize winsock
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		cout << "fail to setup" << endl;
		return;
	}


	SOCKET reciving = socket(AF_INET, SOCK_STREAM, 0);
	if (reciving == INVALID_SOCKET) {
		cout << "no socket" << endl;
		return;
	}

	// where to connect 
	cout << "Enter your target's port information." << endl;
	int port = getPort();
	string address = getAddress();

	//make address 
	sockaddr_in host;
	host.sin_family = AF_INET;
	host.sin_port = htons(port);
	inet_pton(AF_INET, address.c_str(), &(host.sin_addr.S_un.S_addr));

	//my socket

	if (connect(reciving, (sockaddr*)&host, sizeof(host)) != 0) {
		cout << "connection error" << endl;
		return;
	}
	cout << "connected" << endl;


	char data[4096];
	string mess;
	while (true) {
		cout << "what would you like to send: " << flush;
		mess = getUser();
		send(reciving, mess.c_str(), mess.length(), 0);
		ZeroMemory(data, 4096);
		recv(reciving, data, 4096, 0);
		cout << data << endl;


	}

}