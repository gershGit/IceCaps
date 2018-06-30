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
#define DEFAULT_BUF_LEN 128

struct Game {
	std::string name;
	std::string pcName;
	sockaddr_in server_addr;
	std::string address_as_string;
};

class NetClient
{
private:
	unsigned short Default_Port = 27015;
	unsigned short Default_Port_In = 27017;
	std::string addressTemp = "10.0.0.17";
	sockaddr_in * serverAddress;
	WSAData wsaData;
	std::vector<std::string> *messageInList;
	std::vector<std::string> *messageOutList;
	std::mutex *messageIn_mutex;
	std::mutex *messageOut_mutex;
	std::vector<Game> * gamesList;

public:
	SOCKET SendSocket = INVALID_SOCKET;
	SOCKET ReceiveSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	NetClient(std::vector<std::string> *mInList, std::vector<std::string> *mOutList, std::mutex *mIn_mutex, std::mutex *mOut_mutex);
	~NetClient();

	void linkGameList(std::vector<Game> * gListPtr);
	void linkServerAddress(sockaddr_in * serverPtr);
	int Initialize();
	int receiveLoop();
	int join_group_client(int sd, unsigned int grpaddr, unsigned int iaddr);
	int leave_group_client(int sd, unsigned int grpaddr, unsigned int iaddr);
	int AddGame(Game game_in);
	int FindGame(std::string playerName);
	int JoinGame(Game game_to_join);
	int ConnectSocket();
	int SendData();
	int SendTest(const char * data);
	int KillSend();
	int Kill();
};
