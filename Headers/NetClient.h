#include "Headers.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#define DEFAULT_PORT "8081"
#define DEFAULT_BUF_LEN 512

class NetClient
{
private:
	const char* addressTemp = "10.0.0.17";
public:
	SOCKET ConnectingSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	NetClient();
	~NetClient();
	int Initialize();
	int ConnectSocket();
	int SendData(const char * data);
	int KillSend();
	int Kill();
};
