#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 5000

int upload_score(struct settings* set, char* player_name, int score) {
	FILE* connection_log = fopen("connection_log.dat", "a");

	SYSTEMTIME time;
	GetLocalTime(&time);

	fprintf(connection_log, "\nUpload process\n");
	fprintf(connection_log, "Time:\n");
	fprintf(connection_log, "%u.%02u.%02u %02u:%02u:%02u\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	int iResult;
	WSADATA wsaData;

	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in clientService;

	char send_score[10];
	_itoa(score, send_score, 10);

	int recvbuflen = DEFAULT_BUFLEN;
	char sendbuf[300] = "GET /save?name=";
	strcat(sendbuf, player_name);
	strcat(sendbuf, "&score=");
	strcat(sendbuf, send_score);
	strcat(sendbuf, " HTTP/1.1");

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		fwprintf(connection_log, L"WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		fwprintf(connection_log, L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(set->ip_address);
	clientService.sin_port = htons(DEFAULT_PORT);

	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		fwprintf(connection_log, L"connect failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		fwprintf(connection_log, L"send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	fprintf(connection_log, "Bytes Sent: %d\n", iResult);

	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		fwprintf(connection_log, L"shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	iResult = closesocket(ConnectSocket);
	if (iResult == SOCKET_ERROR) {
		fwprintf(connection_log, L"close failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	WSACleanup();
	fprintf(connection_log, "Success\n");
	return 0;
}

int download_score(struct settings* set) {
	FILE* connection_log = fopen("connection_log.dat", "a");

	SYSTEMTIME time;
	GetLocalTime(&time);

	fprintf(connection_log, "\nDownload process\n");
	fprintf(connection_log, "Time:\n");
	fprintf(connection_log, "%u.%02u.%02u %02u:%02u:%02u\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	//----------------------
	// Declare and initialize variables.
	int iResult;
	WSADATA wsaData;

	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in clientService;

	int recvbuflen = DEFAULT_BUFLEN;
	char sendbuf[] = "GET /score HTTP/1.1";
	char recvbuf[DEFAULT_BUFLEN] = "";

	//----------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		fwprintf(connection_log, L"WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	//----------------------
	// Create a SOCKET for connecting to server
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		fwprintf(connection_log, L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(set->ip_address);
	clientService.sin_port = htons(DEFAULT_PORT);

	//----------------------
	// Connect to server.
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		fwprintf(connection_log, L"connect failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	//----------------------
	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		fwprintf(connection_log, L"send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	fprintf(connection_log, "Bytes Sent: %d\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		fwprintf(connection_log, L"shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			fwprintf(connection_log, L"Bytes received: %d\n", iResult),printf(recvbuf);
		else if (iResult == 0)
			fwprintf(connection_log, L"Connection closed\n");
		else
			fwprintf(connection_log, L"recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);


	// close the socket
	iResult = closesocket(ConnectSocket);
	if (iResult == SOCKET_ERROR) {
		fwprintf(connection_log, L"close failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	WSACleanup();
	fprintf(connection_log, "Success\n");
	return 0;
}