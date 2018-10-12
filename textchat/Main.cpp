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



	if (choice == 0) {
		server2();
	}
	else {
		client();
	}

	system("pause");
	return 0;
}