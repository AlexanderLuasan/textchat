#include "client.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib,"ws2_32.lib")

using namespace std;
int clinetJoin(SOCKET * partner, sockaddr_in * partneradd, int size, int port, string address)
{
	//setup
	WSADATA wsData;//initialize winsock
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		cout << "fail to setup" << endl;
		return 1;
	}

	*partner = socket(AF_INET, SOCK_STREAM, 0);
	if (*partner == INVALID_SOCKET) {
		cout << "no socket" << endl;
		return 1;
	}

	

	
	partneradd->sin_family = AF_INET;
	partneradd->sin_port = htons(port);
	inet_pton(AF_INET, address.c_str(), &(partneradd->sin_addr.S_un.S_addr));
	if (connect(*partner, (sockaddr*)partneradd, size) != 0) {
		cout << "connection error" << endl;
		return 1;
	}
	cout << "connected" << endl;

	return 0;
}
