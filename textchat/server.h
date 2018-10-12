#pragma once
#include <WS2tcpip.h>
#include <string>
using namespace std;
int serverOpen(SOCKET *partner, sockaddr_in *partneradd, int size, int port, string address);
void server();
