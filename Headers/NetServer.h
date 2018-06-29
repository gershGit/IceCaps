#pragma once
#include "Headers.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "8081"
#define DEFAULT_BUF_LEN 128

struct clientInfo {
	int clientID;
	std::string name;
	sockaddr_in client_sockaddr;
};

class NetServer
{
private:
	bool * gameStarted = false;
	unsigned short Default_Port = 27015;
	unsigned short Default_Port_Out = 27017;
	std::vector<clientInfo> * clients;
	int client_nums = 0;
public:
	std::vector<SOCKET> ListenSockets = {INVALID_SOCKET};
	std::vector<std::string> * messageInList;
	std::mutex * messageInMutex;

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	bool isServer = false;
	WSADATA wsaData;

	int AddClient(clientInfo clientInfo);

	int DropClient(clientInfo client_in);

	int ReceiveLoop(SOCKET receiveSocket, std::vector<std::string>* messageInList, std::mutex * messageIn_mutex);

	NetServer(std::vector<std::string>* mInList, std::mutex * mInMutex, std::vector<clientInfo> * clients_in);
	~NetServer();

	void setGameStarted(bool* ref);
	int initialize();
	int AddSocket();
	int ReceiveClients(SOCKET multiSocket, std::string myGame, sockaddr_in multiAddress, std::string broadcastServerMessage);
	int OpenGame(std::string myGameName);
	int StartListen();
	int KillAll();
	int KillSocket(SOCKET to_kill);
};

class NetSender
{
private:
	unsigned short Default_Port = 27017;
	std::vector<clientInfo> * clients;
public:
	std::vector<SOCKET> SendSockets;
	std::vector<std::string> * messageOutList;
	std::mutex * messageOutMutex;
	int lastSize = 0;

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	WSADATA wsaData;

	NetSender(std::vector<std::string>* mOutList, std::mutex * mOutMutex, std::vector<clientInfo> * clients_in);
	~NetSender();

	int initialize();
	int AddSocket(clientInfo clientInfo);
	int SendAll();
	int KillAll();
	int KillSocket(SOCKET to_kill);
};