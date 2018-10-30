#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "server.h"
#include "client.h"
#include "chatfunc.h"
#include <thread>
#include <future>

using namespace std;
int sendfunc(fd_set sockets);
int recfunc(fd_set sockets);
int main() {
	cout << "Text chat start" << endl;
	bool chating = true;
	while (chating) {
		int choice;
		cout << "server  or client" << endl;
		if (getUser() == "s") {
			choice = 0;
		}
		else {
			choice = 1;
		}


		char host[NI_MAXHOST]; // name
		char service[NI_MAXHOST]; // port
		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(service, NI_MAXHOST);
		SOCKET partner;
		sockaddr_in partnerdata;
		int size = sizeof(partnerdata);

		timeval timeout;
		fd_set sockets;
		timeout.tv_sec=1;
		if (choice == 0) {
			//collect ports n stuff
			cout << "Enter your own port information." << endl;
			int port = getPort();
			string address = getAddress();
			serverOpen(&partner, &partnerdata, size, port, address);
			if (getnameinfo((sockaddr*)&partnerdata, sizeof(partnerdata), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
				cout << host << " connected on port " << service << endl;
			}
			else {
				inet_ntop(AF_INET, &partnerdata.sin_addr, host, NI_MAXHOST);
				cout << host << " connected on port " << ntohs(partnerdata.sin_port) << endl;
			}

		}
		else {
			cout << "Enter your target's port information." << endl;
			int port = getPort();
			string address = getAddress();
			clinetJoin(&partner, &partnerdata, size, port, address);
			if (getnameinfo((sockaddr*)&partnerdata, sizeof(partnerdata), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
				cout << host << " connected on port " << service << endl;
			}
			else {
				inet_ntop(AF_INET, &partnerdata.sin_addr, host, NI_MAXHOST);
				cout << host << " connected on port " << ntohs(partnerdata.sin_port) << endl;
			}
		}

		sockets.fd_count = 1;
		sockets.fd_array[0] = partner;


		int bitsrecved;
		char data[4096];
		string mess;

		future<int> ready = async(&sendfunc, sockets);
		future<int> ready2 = async(&recfunc, sockets);
		
		while (true) {
			
			
			auto status = ready.wait_for(0ms);
			
			if (status==future_status::ready) {
				ready = async(&sendfunc, sockets);
			}
			else {
				
			}
			auto status2 = ready2.wait_for(0ms);
			if (status2 == future_status::ready) {
				ready2 = async(&recfunc, sockets);
			}
			else {

			}
		}

		closesocket(partner);
	}
	system("pause");
	return 0;
}

int sendfunc(fd_set sockets)
{
	string mess;
	cout << "<<" << flush;
	mess = getUser();

	if (mess.compare("quit") == 0) {
		char qd[3] = "!q";
		send(sockets.fd_array[0], qd, 3, 0);
	}
	else {
		send(sockets.fd_array[0], mess.c_str(), mess.length(), 0);
	}
	return 0;
}
int recfunc(fd_set sockets) {
	char data[400];
	int bitsrecved;
	
	ZeroMemory(data, 400);
	bitsrecved = recv(sockets.fd_array[0], data, 4096, 0);
	if (bitsrecved == 0) {
		cout << "quitting" << endl;
		exit(1);
	}
	else {
		cout << endl << ">>" << data << endl;
	}
	return 1;
}
