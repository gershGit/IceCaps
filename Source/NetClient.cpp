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

	iResult = getaddrinfo(addressTemp, DEFAULT_PORT, &hints, &result);
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
	InetPton(AF_INET, "10.0.0.17", &ReceiveAddress.sin_addr.S_un.S_addr);

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

int NetClient::FindGame()
{
	return 0;
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
	InetPton(AF_INET, "10.0.0.17", &server.sin_addr.S_un.S_addr);

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
	InetPton(AF_INET, "10.0.0.17", &server.sin_addr.S_un.S_addr);

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