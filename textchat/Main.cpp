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

		char host[NI_MAXHOST]; // name
		char service[NI_MAXHOST]; // port
		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(service, NI_MAXHOST);
		SOCKET partner;
		sockaddr_in partnerdata;
		int size = sizeof(partnerdata);

		timeval timeout;
		fd_set sockets;
		timeout.tv_sec = 0;

		int choice;
		cout << "server or client" << endl;
		string in= getUser();
		if (in.compare("s")==0) {
			choice = 0;
		}
		else if (in.compare("c") == 0){
			choice = 1;
		}
		else {
			continue;
		}

		if (choice == 0) {
			//collect ports n stuff
			cout << "Enter your own information." << endl;
			string address = getAddress();
			int port = getPort();
			serverOpen(&partner, &partnerdata, size, port, address);
		}
		else {
			cout << "Enter your target's information." << endl;
			string address = getAddress();
			int port = getPort();
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

		future<int> sending = async(&sendfunc, sockets);
		future<int> reciving = async(&recfunc, sockets);
		cout << flush;
		while (true) {
			
			
			auto status = sending.wait_for(0ms);
			if (status==future_status::ready) {
				sending = async(&sendfunc, sockets);
			}
			else {
				
			}
			auto status2 = reciving.wait_for(0ms);
			if (status2 == future_status::ready) {
				int out = reciving.get();
				if (out == -1) {
					send(sockets.fd_array[0], "!q", 3, 0);
					cout << "your partner has left. Enter to continue.";
					break;
				}
				else {
					reciving = async(&recfunc, sockets);
				}
			}
			else {

			}
		}
		
		closesocket(partner);
		sending.wait();
		cout << "Would you like to continue chating? y/(n): ";
		string exitc;
		exitc = getUser();
		if (exitc.compare("y")!=0) {
			chating = false;
		}

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
	bitsrecved = recv(sockets.fd_array[0], data, 400, 0);
	if (bitsrecved == 0) {
		cout << "quitting" << endl;
		return -1;
	}
	else if (data[0]=='!' && data[1] == 'q') {
		return -1;
	}
	else {
		cout << '\r' << ">>" << data << endl << "<<";
	}
	return 1;
}
