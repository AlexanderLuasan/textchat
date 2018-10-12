#pragma once
#include <WS2tcpip.h>


int serverOpen(SOCKET *partner, sockaddr_in *partneradd, int size);
void server();

void server2();