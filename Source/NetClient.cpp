#include "..\Headers\NetClient.h"



NetClient::NetClient()
{
}


NetClient::~NetClient()
{
}

int NetClient::Initialize() {
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult = getaddrinfo(addressTemp, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("Get address info failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
}

int NetClient::ConnectSocket() {
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
	return 0;
}

int NetClient::SendData(const char *data) {
	int recvbuflen = DEFAULT_BUF_LEN;
	char recvbuf[DEFAULT_BUF_LEN];

	int iResult = send(ConnectingSocket, data, (int)strlen(data), 0);
	if (iResult == SOCKET_ERROR) {
		printf("Send failed: %d\n", WSAGetLastError());
		closesocket(ConnectingSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld", iResult);
	do {
		iResult = recv(ConnectingSocket, recvbuf, recvbuflen, 0);
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