#pragma once
#include "Headers.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "8081"
#define DEFAULT_BUF_LEN 128

class NetServer
{
private:
	unsigned short Default_Port = 27015;
	unsigned short Default_Port_Out = 27017;
	std::vector<sockaddr_in> * clients;
	int client_nums = 0;
public:
	std::vector<SOCKET> ListenSockets = {INVALID_SOCKET};
	std::vector<std::string> * messageInList;
	std::mutex * messageInMutex;

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	bool isServer = false;
	WSADATA wsaData;

	int AddClient(sockaddr_in clientInfo);

	int ReceiveLoop(SOCKET receiveSocket, std::vector<std::string>* messageInList, std::mutex * messageIn_mutex);

	NetServer(std::vector<std::string>* mInList, std::mutex * mInMutex, std::vector<sockaddr_in> * clients_in);
	~NetServer();

	int initialize();
	int AddSocket();
	int OpenGame();
	int StartListen();
	int KillAll();
	int KillSocket(SOCKET to_kill);
};

class NetSender
{
private:
	unsigned short Default_Port = 27017;
	std::vector<sockaddr_in> * clients;
public:
	std::vector<SOCKET> SendSockets;
	std::vector<std::string> * messageOutList;
	std::mutex * messageOutMutex;
	int lastSize = 0;

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	WSADATA wsaData;

	NetSender(std::vector<std::string>* mOutList, std::mutex * mOutMutex, std::vector<sockaddr_in> * clients_in);
	~NetSender();

	int initialize();
	int AddSocket(sockaddr_in clientInfo);
	int SendAll();
	int KillAll();
	int KillSocket(SOCKET to_kill);
};