#include "Headers.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <mutex>

#define DEFAULT_PORT "8081"
#define DEFAULT_PORT_INT 8081
#define RECEIVE_PORT "8082"
#define DEFAULT_BUF_LEN 64

class NetClient
{
private:
	unsigned short Default_Port = 27015;
	unsigned short Default_Port_In = 27017;
	const char* addressTemp = "10.0.0.17";
	WSAData wsaData;
	std::vector<const char *> *messageInList;
	std::vector<const char *> *messageOutList;
	std::mutex *messageIn_mutex;
	std::mutex *messageOut_mutex;

public:
	SOCKET SendSocket = INVALID_SOCKET;
	SOCKET ReceiveSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	NetClient(std::vector<const char *> *mInList, std::vector<const char *> *mOutList, std::mutex *mIn_mutex, std::mutex *mOut_mutex);
	~NetClient();
	int Initialize();
	int receiveLoop();
	int ConnectSocket();
	int SendData();
	int SendTest(const char * data);
	int KillSend();
	int Kill();
};
