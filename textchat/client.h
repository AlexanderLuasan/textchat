#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include<string>
using namespace std;
int clinetJoin(SOCKET *partner, sockaddr_in *partneradd, int size, int port, string address);
