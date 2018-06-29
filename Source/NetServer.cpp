#include "..\Headers\NetServer.h"

int NetServer::AddClient(clientInfo client_in) {
	std::cout << "Adding client at " << client_in.client_sockaddr.sin_addr.S_un.S_addr << std::endl;
	for (clientInfo client : *clients) {
		if (client.client_sockaddr.sin_addr.S_un.S_addr == client_in.client_sockaddr.sin_addr.S_un.S_addr) {
			return 1;
		}
	}
	clients->push_back(client_in);
	return 0;
}

int NetServer::DropClient(clientInfo client_in) {
	std::cout << "Adding client at " << client_in.client_sockaddr.sin_addr.S_un.S_addr << std::endl;
	std::vector<clientInfo> myClients = *clients;
	for (int i = 0; i < clients->size(); i++) {
		if (myClients[i].client_sockaddr.sin_addr.S_un.S_addr == client_in.client_sockaddr.sin_addr.S_un.S_addr) {
			clients->erase(clients->begin() + i);
			return 0;
		}
	}
	return 1;
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
				clientInfo newClient;
				newClient.name = "TODOFIXTHISNAME";
				newClient.clientID = -1;
				newClient.client_sockaddr = client;
				AddClient(newClient);
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

NetServer::NetServer(std::vector<std::string> * mInList, std::mutex * mInMutex, std::vector<clientInfo> * clients_in)
{
	messageInList = mInList;
	messageInMutex = mInMutex;
	clients = clients_in;
}

NetServer::~NetServer()
{
}

void NetServer::setGameStarted(bool * ref)
{
	gameStarted = ref;
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

int join_group(int sd, unsigned int grpaddr, unsigned int iaddr) {
	struct ip_mreq imr;
	imr.imr_multiaddr.S_un.S_addr = grpaddr;
	imr.imr_interface.S_un.S_addr = iaddr;
	return setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&imr, sizeof(imr));
}

int leave_group(int sd, unsigned int grpaddr, unsigned int iaddr) {
	struct ip_mreq imr;
	imr.imr_multiaddr.S_un.S_addr = grpaddr;
	imr.imr_interface.S_un.S_addr = iaddr;
	return setsockopt(sd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&imr, sizeof(imr));
}

int NetServer::ReceiveClients(SOCKET multiSocket, std::string myName, sockaddr_in sendAddress, std::string broadcastServerMessage) {
	printf("Now Receiving Clients to open game\n");
	char recvbuf[DEFAULT_BUF_LEN];
	int iResult, iSendResult;
	int recvbuflen = DEFAULT_BUF_LEN;
	struct sockaddr_in client;
	int client_length = (int)sizeof(client);

	//Infinite recieve
	do {
		iResult = recvfrom(multiSocket, recvbuf, recvbuflen, 0, (SOCKADDR *)&client, &client_length);
		if (iResult > 0) {
			std::string str = std::string(recvbuf, iResult);
			std::size_t found = str.find(myName);
			if (str[0] == 'J' && found!=std::string::npos) {
				clientInfo newClient;
				newClient.name = "TODOFIXTHISNAME";
				newClient.clientID = -1;
				newClient.client_sockaddr = client;
				AddClient(newClient);
			}
			else if (str[0] == 'L' && found != std::string::npos) {
				clientInfo newClient;
				newClient.client_sockaddr = client;
				AddClient(newClient);
				DropClient(newClient);
			}
			else if (str[0] == 'Q') {
				iResult = sendto(multiSocket, broadcastServerMessage.c_str(), (int)strlen(broadcastServerMessage.c_str()) + 1, 0, (SOCKADDR*)&sendAddress, sizeof(sendAddress));
				if (iResult == SOCKET_ERROR) {
					printf("Send server broadcast failed: %ld\n", WSAGetLastError());
					freeaddrinfo(result);
					closesocket(ListenSockets.back());
					WSACleanup();
					return 1;
				}
			}
			messageInList->push_back(str);
		}
		else if (iResult == 0) {
			printf("Connection closing...\n");
		}
		else {
			printf("Receive failed: %d\n", WSAGetLastError());
			closesocket(multiSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0 && !gameStarted);
	leave_group(multiSocket, sendAddress.sin_addr.S_un.S_addr, INADDR_ANY);
	return 0;
}

int NetServer::OpenGame(std::string myGameName) {
	SOCKET tempServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (tempServerSocket == INVALID_SOCKET) {
		printf("Error at socket creation: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	unsigned int grpaddr;
	InetPton(AF_INET, "239.0.0.5", &grpaddr);
	sockaddr_in multiAddress;
	multiAddress.sin_family = AF_INET;
	multiAddress.sin_port = htons(Default_Port);
	multiAddress.sin_addr.S_un.S_addr = grpaddr;

	int iResult = join_group(tempServerSocket, grpaddr, INADDR_ANY);
	if (iResult == SOCKET_ERROR) {
		printf("Join group failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(tempServerSocket);
		WSACleanup();
		return 1;
	}

	char myBuffer[64];
	if (gethostname(myBuffer, sizeof(myBuffer)) == SOCKET_ERROR) {
		printf("Failed getting local IP. Check connection. Error: %ld\n", WSAGetLastError());
		return 1;
	}

	struct hostent *phe = gethostbyname(myBuffer);
	if (phe == 0) {
		printf("Error on host lookup. Ensure your pc exists: %ld\n", WSAGetLastError());
		return 1;
	}

	int iter = 0;
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		iter++;
	}

	struct in_addr addr;
	memcpy(&addr, phe->h_addr_list[iter-1], sizeof(struct in_addr));
	std::string broadcastServerMessage = std::string("Server: ") + myBuffer + " " + myGameName + " @: " + std::string(inet_ntoa(addr));
	std::cout << broadcastServerMessage.c_str() << std::endl;
	iResult = sendto(tempServerSocket, broadcastServerMessage.c_str(), (int)strlen(broadcastServerMessage.c_str()) + 1, 0, (SOCKADDR*)&multiAddress, sizeof(multiAddress));
	if (iResult == SOCKET_ERROR) {
		printf("Send server broadcast failed: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(tempServerSocket);
		WSACleanup();
		return 1;
	}	

	ReceiveClients(tempServerSocket, myGameName, multiAddress, broadcastServerMessage);
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

NetSender::NetSender(std::vector<std::string>* mOutList, std::mutex * mOutMutex, std::vector<clientInfo>* clients_in)
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

int NetSender::AddSocket(clientInfo clientinfo)
{
	printf("--Adding socket\n");
	SendSockets.push_back(socket(clientinfo.client_sockaddr.sin_family, SOCK_DGRAM, 0));

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
	InetPton(AF_INET, "10.0.0.17", &sendInfo.sin_addr.S_un.S_addr);


	
	for (int i = 0; i < clients->size(); i++) {
		for (std::string message : *messageOutList) {
			
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
