#include "server.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

int serverOpen(SOCKET * ClientSocket, sockaddr_in * client, int size, int port, string address)
{
	WSADATA wsData;//initialize winsock
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		cout << "fail to setup" << endl;
		return 1;
	}
	//ceate socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cout << "no socket" << endl;
		return 1;
	}


	//bind socket
	sockaddr_in ipport;
	ipport.sin_family = AF_INET;
	ipport.sin_port = htons(port);
	inet_pton(AF_INET, address.c_str(), &(ipport.sin_addr.S_un.S_addr));


	bind(listening, (sockaddr*)&ipport, sizeof(ipport));

	cout << "wait for connection" << endl;
	listen(listening, SOMAXCONN);

	*ClientSocket = accept(listening, (sockaddr*)&client, &size);
	if (*ClientSocket == INVALID_SOCKET) {
		cout << "failed connecting socket" << endl;
		return 1;
	}
	cout << "connected" << endl;
	closesocket(listening);

	return 0;
}

void server()
{
	WSADATA wsData;//initialize winsock
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0) {
		cout << "fail to setup" << endl;
		return;
	}
	//ceate socket
	SOCKET listening = socket(AF_INET,SOCK_STREAM,0);
	if (listening == INVALID_SOCKET) {
		cout << "no socket" << endl;
		return;
	}
	//get addres information

	int port = 80;
	char address[] = "203.188.81.76";
	


	//bind socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, address, &(hint.sin_addr.S_un.S_addr));
	bind(listening, (sockaddr*)&hint, sizeof(hint));


	//setlistent

	listen(listening, SOMAXCONN);

	//wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	cout << "wait for connect" << endl;
	SOCKET ClientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	if (ClientSocket == INVALID_SOCKET) {
		cout << "failed connecting socket" << endl;
		return;
	}
	//print out partner information

	char host[NI_MAXHOST]; // name
	char service[NI_MAXHOST]; // port

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		cout << host << " connected on port " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}
	//close listening 
	closesocket(listening);
	//while loop: talk program
	char buf[4096];
	cout << "enter loop" << endl;
	while (true) {
		ZeroMemory(buf, 4096);
		cout << "loop" << endl;
		int bytesRecived = recv(ClientSocket, buf, 4096, 0);
		if (bytesRecived == SOCKET_ERROR) {
			cout << "error" << endl;
			break;
		}
		if(bytesRecived==0){
			cout << "end" << endl;
			break;
		}

		send(ClientSocket, buf, bytesRecived + 1, 0);

	}


	//close sock
	closesocket(ClientSocket);
	//shutdown winsock
	WSACleanup();

}
