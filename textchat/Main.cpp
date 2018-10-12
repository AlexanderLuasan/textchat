#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "server.h"
#include "client.h"
#include "chatfunc.h"

using namespace std;
int main() {
	cout << "Text chat start" << endl;

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
		serverOpen(&partner,&partnerdata,size);
		if (getnameinfo((sockaddr*)&partnerdata, sizeof(partnerdata), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
			cout << host << " connected on port " << service << endl;
		}
		else {
			inet_ntop(AF_INET, &partnerdata.sin_addr, host, NI_MAXHOST);
			cout << host << " connected on port " << ntohs(partnerdata.sin_port) << endl;
		}

	}
	else {
		clinetJoin(&partner, &partnerdata, size);
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
		cout << ">>" << data << endl;
		if (data[0] == 'q') {
			break;
		}
		cout << "<<" << flush;
		mess = getUser();
		send(partner, mess.c_str(), mess.length(), 0);
	}
	


	system("pause");
	return 0;
}