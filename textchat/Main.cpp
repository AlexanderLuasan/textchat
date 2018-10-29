#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "server.h"
#include "client.h"
#include "chatfunc.h"

using namespace std;
int main() {
	cout << "Text chat start" << endl;
	bool chating = true;
	while (chating) {
		cout << "server  or client" << endl;
		int choice = stoi(getUser());


		char host[NI_MAXHOST]; // name
		char service[NI_MAXHOST]; // port
		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(service, NI_MAXHOST);
		SOCKET partner;
		sockaddr_in partnerdata;
		int size = sizeof(partnerdata);


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
			string temp;
			cout << "<<" << flush;
			temp = getUser();
			send(partner, temp.c_str(), temp.length(), 0);
		}



		int bitsrecved;
		char data[4096];
		string mess;
		while (true) {
			ZeroMemory(data, 4096);
			bitsrecved = recv(partner, data, 4096, 0);
			if (bitsrecved == 0) {
				cout << "quitting" << endl;
				break;
			}
			else if (data[0] == '!'&& data[1] == 'q') {
				cout << "requesting quit" << endl;
				char qd[3] = "!q";
				send(partner, qd, 3, 0);
				mess = getUser();
				if (mess.compare("yes") == 0 || mess.compare("y") == 0) {
					chating = false;
					break;
				}
				else {
					break;
				}
			}
			else {
				cout << ">>" << data << endl;
			}
			cout << "<<" << flush;
			mess = getUser();

			if (mess.compare("quit") == 0) {
				char qd[3] = "!q";
				send(partner, qd, 3, 0);
			}
			else {
				send(partner, mess.c_str(), mess.length(), 0);
			}

		}

		closesocket(partner);
	}
	system("pause");
	return 0;
}