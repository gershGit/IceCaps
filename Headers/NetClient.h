#include "Headers.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>

#define DEFAULT_PORT "8081"
#define RECEIVE_PORT "8082"
#define DEFAULT_BUF_LEN 512

class NetClient
{
private:
	const char* addressTemp = "10.0.0.17";
	WSAData wsaData;
	std::vector<const char *> *mList;
public:
	SOCKET ConnectingSocket = INVALID_SOCKET;
	SOCKET RecvSocket;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	NetClient(std::vector<const char *> *messageList);
	~NetClient();
	int Initialize();
	int ConnectSocket();
	int SendData(const char * data);
	int KillSend();
	int Kill();
};
