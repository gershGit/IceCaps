#include "..\Headers\NetClient.h"
#include <thread>


NetClient::NetClient(std::vector<const char *> *messageList)
{
	mList = messageList;
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
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo(addressTemp, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("Get address info failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
}

void receiveLoop(SOCKET sock) {
	std::cout << "Started listen thread" << std::endl;
	int iResult;
	int recvbuflen = DEFAULT_BUF_LEN;
	char recvbuf[DEFAULT_BUF_LEN];
	do {
		iResult = recv(sock, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
		}
		else if (iResult == 0) {
			printf("Connection Closed\n");
		}
		else {
			printf("Receive failed: %d\n", WSAGetLastError());
		}
	} while (iResult > 0);
}

int NetClient::ConnectSocket() {
	//Send socket
	ptr = result;

	ConnectingSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (ConnectingSocket == INVALID_SOCKET) {
		printf("Error connecting socket: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	int iResult = connect(ConnectingSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectingSocket);
		ConnectingSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectingSocket == INVALID_SOCKET) {
		printf("Unable to connect to server! \n");
		WSACleanup();
		return 1;
	}


	//Receive socket
	iResult = getaddrinfo(addressTemp, RECEIVE_PORT, &hints, &result);
	if (iResult != 0) {
		printf("Get address info failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	ptr = result;

	
	RecvSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (RecvSocket == INVALID_SOCKET) {
		printf("Error connecting socket: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = connect(RecvSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(RecvSocket);
		RecvSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (RecvSocket == INVALID_SOCKET) {
		printf("Unable to connect to server! \n");
		WSACleanup();
		return 1;
	}

	std::thread(receiveLoop, RecvSocket);
	return 0;
}

int NetClient::SendData(const char *data) {
	int iResult;
	for (const char * message : *mList) {
		iResult = send(ConnectingSocket, message, (int)strlen(message), 0);
		if (iResult == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError());
			closesocket(ConnectingSocket);
			WSACleanup();
			return 1;
		}
		printf("Bytes Sent: %ld\n", iResult);
	}	
	return 0;
}

int NetClient::KillSend() {
	int iResult = shutdown(ConnectingSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectingSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}

int NetClient::Kill() {
	int iResult = shutdown(ConnectingSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectingSocket);
		WSACleanup();
		return 1;
	}
	closesocket(ConnectingSocket);
	WSACleanup();

	return 0;
}