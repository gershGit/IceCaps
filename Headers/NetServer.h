#pragma once
#include "Headers.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "8081"
#define SEND_PORT "8082"
#define DEFAULT_BUF_LEN 512

class NetServer
{
public:
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET SendSocket = INVALID_SOCKET;
	std::vector<const char *> * mList;

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	bool isServer = false;
	WSADATA wsaData;

	NetServer(std::vector<const char*> * messageList);
	~NetServer();

	int initialize();
	int createServer();
	int startListen();
	int sendRecieve();
};
