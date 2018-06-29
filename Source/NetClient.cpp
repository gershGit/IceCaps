#include "..\Headers\NetClient.h"
#include <thread>



NetClient::NetClient(std::vector<std::string> *mInList, std::vector<std::string> *mOutList, std::mutex *mIn_mutex, std::mutex *mOut_mutex)
{
	messageInList = mInList;
	messageOutList = mOutList;
	messageIn_mutex = mIn_mutex;
	messageOut_mutex = mOut_mutex;
}

NetClient::~NetClient()
{
}

void NetClient::linkGameList(std::vector<Game> * gListPtr)
{
	gamesList = gListPtr;
}

void NetClient::linkServerAddress(sockaddr_in * serverPtr)
{
	serverAddress = serverPtr;
}

int NetClient::Initialize() {
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

	iResult = getaddrinfo(addressTemp.c_str(), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("Get address info failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
}

int NetClient::receiveLoop() {
	std::cout << "Started receive loop" << std::endl;
	int iResult;
	int recvbuflen = DEFAULT_BUF_LEN;
	char recvbuf[DEFAULT_BUF_LEN];
	struct sockaddr_in server;
	int server_length = (int)sizeof(server);

	//Receive socket
	ReceiveSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sockaddr_in ReceiveAddress;
	ReceiveAddress.sin_family = AF_INET;
	ReceiveAddress.sin_port = htons(Default_Port_In);
	InetPton(AF_INET, addressTemp.c_str(), &ReceiveAddress.sin_addr.S_un.S_addr);

	ULONG blockingMode = 0;
	iResult = ioctlsocket(ReceiveSocket, FIONBIO, &blockingMode);

	iResult = bind(ReceiveSocket, (SOCKADDR *)& ReceiveAddress, sizeof(ReceiveAddress));
	if (iResult == SOCKET_ERROR) {
		printf("Bind failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ReceiveSocket);
		WSACleanup();
		return 1;
	}

	do {
		iResult = recvfrom(ReceiveSocket, recvbuf, recvbuflen, 0, (SOCKADDR *)&server, &server_length);
		if (iResult > 0) {
			std::lock_guard<std::mutex> lock(*messageIn_mutex);
			messageInList->push_back(recvbuf);
			printf("Bytes received: %d\n", iResult);
		}
		else if (iResult == 0) {
			printf("Connection Closed\n");
		}
		else {
			printf("Receive failed: %d\n", WSAGetLastError());
		}
	} while (iResult > 0);
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

int NetClient::AddGame(Game game_in) {
	for (Game possible_duplicate : *gamesList) {
		if (possible_duplicate.name == game_in.name) {
			return 1;
		}
	}
	gamesList->push_back(game_in);
	return 0;
}

int NetClient::FindGame(std::string playerName)
{
	SOCKET tempClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (tempClientSocket == INVALID_SOCKET) {
		printf("Error at socket creation: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;
	setsockopt(tempClientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&read_timeout, sizeof read_timeout);

	unsigned int grpaddr;
	InetPton(AF_INET, "239.0.0.5", &grpaddr);
	sockaddr_in multiAddress;
	multiAddress.sin_family = AF_INET;
	multiAddress.sin_port = htons(Default_Port);
	multiAddress.sin_addr.S_un.S_addr = grpaddr;

	int iResult = join_group(tempClientSocket, grpaddr, INADDR_ANY);
	if (iResult == SOCKET_ERROR) {
		printf("Join group failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(tempClientSocket);
		WSACleanup();
		return 1;
	}

	std::string queryMessage = std::string("Query Games") + playerName;
	std::cout << queryMessage.c_str() << std::endl;
	iResult = sendto(tempClientSocket, queryMessage.c_str(), (int)strlen(queryMessage.c_str()) + 1, 0, (SOCKADDR*)&multiAddress, sizeof(multiAddress));
	if (iResult == SOCKET_ERROR) {
		printf("Send query broadcast failed: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(tempClientSocket);
		WSACleanup();
		return 1;
	}

	printf("Now Receiving Game(s) Information\n");
	char recvbuf[DEFAULT_BUF_LEN];
	int iResult, iSendResult;
	int recvbuflen = DEFAULT_BUF_LEN;
	struct sockaddr_in server;
	int server_length = (int)sizeof(server);

	//Infinite recieve until servers are done
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		iResult = recvfrom(tempClientSocket, recvbuf, recvbuflen, 0, (SOCKADDR *)&server, &server_length);
		if (iResult > 0) {
			std::string str = std::string(recvbuf, iResult);
			if (str[0] == 'S') {
				Game newGame;
				char * pch;
				pch = strtok(recvbuf, " ");
				pch = strtok(NULL, " ");
				newGame.pcName = std::string(pch);
				pch = strtok(NULL, " ");
				newGame.name = std::string(pch);
				pch = strtok(NULL, " ");
				pch = strtok(NULL, " ");
				newGame.address_as_string = std::string(pch);
				newGame.server_addr = server;
				AddGame(newGame);
			}
		}
		else if (iResult == 0) {
			printf("Connection closing...\n");
		}
		else {
			printf("Receive failed: %d\n", WSAGetLastError());
			closesocket(tempClientSocket);
			WSACleanup();
			return 1;
		}
	}
	return 0;
}

int NetClient::JoinGame(Game game_to_join) {
	SOCKET tempClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	*serverAddress = game_to_join.server_addr;

	if (tempClientSocket == INVALID_SOCKET) {
		printf("Error at socket creation: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 10;
	setsockopt(tempClientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&read_timeout, sizeof read_timeout);

	unsigned int grpaddr;
	InetPton(AF_INET, "239.0.0.5", &grpaddr);
	sockaddr_in multiAddress;
	multiAddress.sin_family = AF_INET;
	multiAddress.sin_port = htons(Default_Port);
	multiAddress.sin_addr.S_un.S_addr = grpaddr;

	int iResult = join_group(tempClientSocket, grpaddr, INADDR_ANY);
	if (iResult == SOCKET_ERROR) {
		printf("Join group failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(tempClientSocket);
		WSACleanup();
		return 1;
	}

	std::string joinMessage = std::string("Join ") + game_to_join.name;
	std::cout << joinMessage.c_str() << std::endl;
	iResult = sendto(tempClientSocket, joinMessage.c_str(), (int)strlen(joinMessage.c_str()) + 1, 0, (SOCKADDR*)&multiAddress, sizeof(multiAddress));
	if (iResult == SOCKET_ERROR) {
		printf("Join broadcast failed: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(tempClientSocket);
		WSACleanup();
		return 1;
	}
}

int NetClient::ConnectSocket() {
	//Send socket
	ptr = result;

	SendSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (SendSocket == INVALID_SOCKET) {
		printf("Error connecting socket send: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	return 0;
}

int NetClient::SendData() {
	int iResult;
	sockaddr_in server;
	int server_length = sizeof(server);

	server.sin_family = AF_INET;
	server.sin_port = htons(Default_Port);
	InetPton(AF_INET, addressTemp.c_str(), &server.sin_addr.S_un.S_addr);

	for (std::string message : *messageOutList) {
		iResult = sendto(SendSocket, message.c_str(), (int)strlen(message.c_str()) + 1, 0, (SOCKADDR *)&server, server_length);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			closesocket(SendSocket);
			WSACleanup();
			return 1;
		}
		printf("Bytes Sent: %s\n", message.c_str());
	}	
	messageOutList->clear();
	return 0;
}

int NetClient::SendTest(const char *data) {
	std::cout << "Sending message of strlen " << (int)strlen(data) << std::endl;
	
	sockaddr_in server;
	int server_length = sizeof(server);

	server.sin_family = AF_INET;
	server.sin_port = htons(Default_Port);
	InetPton(AF_INET, addressTemp.c_str(), &server.sin_addr.S_un.S_addr);

	int iResult = sendto(SendSocket, data, (int)strlen(data)+1, 0, (SOCKADDR *) &server, server_length);
	if (iResult == SOCKET_ERROR) {
		printf("Send failed: %d\n", WSAGetLastError());
		closesocket(SendSocket);
		WSACleanup();
		return 1;
	}
	printf("Bytes Sent: %s\n", data);
	return 0;
}

int NetClient::KillSend() {
	int iResult = shutdown(SendSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(SendSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

int NetClient::Kill() {
	int iResult = shutdown(SendSocket, SD_BOTH);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(SendSocket);
		WSACleanup();
		return 1;
	}
	closesocket(SendSocket);

	iResult = shutdown(ReceiveSocket, SD_BOTH);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ReceiveSocket);
		WSACleanup();
		return 1;
	}
	closesocket(ReceiveSocket);
	WSACleanup();

	return 0;
}