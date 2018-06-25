#include "..\Headers\NetServer.h"

int NetServer::AddClient(sockaddr_in client_in) {
	clients->push_back(client_in);
	return 0;
}

int NetServer::ReceiveLoop(SOCKET receiveSocket, std::vector<std::string> *messageInList, std::mutex *messageIn_mutex) {
	printf("Starting receive on thread: %d\n", std::this_thread::get_id());
	char recvbuf[DEFAULT_BUF_LEN];
	int iResult, iSendResult;
	int recvbuflen = DEFAULT_BUF_LEN;
	struct sockaddr_in client;
	int client_length = (int)sizeof(client);

	//Infinite recieve
	do {
		iResult = recvfrom(receiveSocket, recvbuf, recvbuflen, 0, (SOCKADDR *)&client, &client_length);
		if (iResult > 0) {
			//printf("Bytes received: %d\n", iResult);
			std::lock_guard<std::mutex> lock(*messageIn_mutex);
			std::string str = std::string(recvbuf, iResult);
			if (str[0] == 'I') {
				AddClient(client);
			}
			messageInList->push_back(str);
		}
		else if (iResult == 0) {
			printf("Connection closing...\n");
		}
		else {
			printf("Receive failed: %d\n", WSAGetLastError());
			closesocket(receiveSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);
}

NetServer::NetServer(std::vector<std::string> * mInList, std::mutex * mInMutex, std::vector<sockaddr_in> * clients_in)
{
	messageInList = mInList;
	messageInMutex = mInMutex;
	clients = clients_in;
}

NetServer::~NetServer()
{
}

int NetServer::initialize()
{
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSA Startup failed: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;
	return 0;
}

int NetServer::AddSocket()
{
	ListenSockets.push_back(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
	if (ListenSockets.back() == INVALID_SOCKET) {
		printf("Error at socket creation: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	sockaddr_in ReceiveAddress;
	ReceiveAddress.sin_family = AF_INET;
	ReceiveAddress.sin_port = htons(Default_Port);
	ReceiveAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	ULONG blockingMode = 0;
	int iResult = ioctlsocket(ListenSockets.back(), FIONBIO, &blockingMode);

	iResult = bind(ListenSockets.back(), (SOCKADDR *)& ReceiveAddress, sizeof(ReceiveAddress));
	if (iResult == SOCKET_ERROR) {
		printf("Bind failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSockets.back());
		WSACleanup();
		return 1;
	}

	return 0;
}

int NetServer::StartListen() {
	//std::thread(ReceiveLoop, ListenSockets.back(), messageInList, messageInMutex);
	ReceiveLoop(ListenSockets.back(), messageInList, messageInMutex);
	
	return 0;
}

int NetServer::KillAll() {
	int iResult;
	for (SOCKET to_kill : ListenSockets) {
		iResult = shutdown(to_kill, SD_BOTH);
		if (iResult == SOCKET_ERROR) {
			printf("Shutdown failed: %d\n", WSAGetLastError());
			closesocket(to_kill);
			WSACleanup();
			return 1;
		}
		closesocket(to_kill);
	}
	return 0;
}

int NetServer::KillSocket(SOCKET to_kill) {
	int iResult = shutdown(to_kill, SD_BOTH);
	if (iResult == SOCKET_ERROR) {
		printf("Shutdown failed: %d\n", WSAGetLastError());
		closesocket(to_kill);
		WSACleanup();
		return 1;
	}

	closesocket(to_kill);
	return 0;
}

NetSender::NetSender(std::vector<std::string>* mOutList, std::mutex * mOutMutex, std::vector<sockaddr_in>* clients_in)
{
	messageOutList = mOutList;
	messageOutMutex = mOutMutex;
	clients = clients_in;
}

NetSender::~NetSender()
{
}

int NetSender::initialize()
{
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSA Startup failed: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;
	return 0;
}

int NetSender::AddSocket(sockaddr_in clientinfo)
{
	printf("Adding socket");
	SendSockets.push_back(socket(clientinfo.sin_family, SOCK_DGRAM, 0));

	/*
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	sockaddr_in ReceiveAddress;
	ReceiveAddress.sin_family = AF_INET;
	ReceiveAddress.sin_port = htons(Default_Port);
	ReceiveAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int iResult = bind(SendSockets.back(), (SOCKADDR *)& ReceiveAddress, sizeof(ReceiveAddress));
	if (iResult == SOCKET_ERROR) {
		printf("Bind failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(SendSockets.back());
		WSACleanup();
		return 1;
	}*/
	return 0;
}

int NetSender::SendAll()
{
	if (clients->size() > lastSize) {
		for (int i = lastSize; i < clients->size(); i++) {
			AddSocket(clients->at(i));
		}
		lastSize = clients->size();
	}
	std::lock_guard<std::mutex> lock(*messageOutMutex);
	sockaddr_in sendInfo;
	sendInfo.sin_family = AF_INET;
	sendInfo.sin_port = htons(Default_Port);
	InetPton(AF_INET, "10.0.0.7", &sendInfo.sin_addr.S_un.S_addr);


	
	for (int i = 0; i < clients->size(); i++) {
		for (std::string message : *messageOutList) {
			//TODO figure out why client info isn't correct
			int iResult = sendto(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP), message.c_str(), (int)strlen(message.c_str()) + 1, 0, (SOCKADDR*)&sendInfo, sizeof(sendInfo));

			//int iResult = sendto(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP), message.c_str(), (int)strlen(message.c_str()) + 1, 0, (SOCKADDR *)&clients[i], sizeof(clients[i]));
			if (iResult == SOCKET_ERROR) {
				printf("Send failed: %d\n", WSAGetLastError());
				closesocket(SendSockets[i]);
				WSACleanup();
				return 1;
			}
			printf("Bytes Sent: %ld\n", iResult);
		}
	}
	return 0;
}

int NetSender::KillAll()
{
	int iResult;
	for (SOCKET to_kill : SendSockets) {
		iResult = shutdown(to_kill, SD_BOTH);
		if (iResult == SOCKET_ERROR) {
			printf("Shutdown failed: %d\n", WSAGetLastError());
			closesocket(to_kill);
			WSACleanup();
			return 1;
		}
		closesocket(to_kill);
	}
	return 0;
}

int NetSender::KillSocket(SOCKET to_kill)
{
	int iResult = shutdown(to_kill, SD_BOTH);
	if (iResult == SOCKET_ERROR) {
		printf("Shutdown failed: %d\n", WSAGetLastError());
		closesocket(to_kill);
		WSACleanup();
		return 1;
	}

	closesocket(to_kill);
	return 0;
}
