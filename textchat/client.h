#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

int clinetJoin(SOCKET *partner, sockaddr_in *partneradd, int size);
void client();