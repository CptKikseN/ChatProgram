#include <winsock2.h>
#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <Ws2tcpip.h>
#include <thread>
#include <vector>

using namespace std;

struct addrinfo *results;
struct addrinfo serverParameters;
WSADATA wsaData;

SOCKET listening;
SOCKET ClientSocket;
SOCKET sockets[5];

char recvbuf[512];
int infoFromAddr, sendRecievedBytes, recievedBytes, bindResult;
int recvbuflen = 512;
int wsaDataError;
int j = 0;

void clientMsgHandler(SOCKET clientSckt) {
	
	SOCKET connectingClient = clientSckt;

	do
	{
		// Recieve bytes from client
		recievedBytes = recv(connectingClient, recvbuf, recvbuflen, 0);

		// If there's a message, eccho message to all clients
		if (recievedBytes > 0) {
			for (int i = 0; i < 2; ++i) {

				cout << string(recvbuf, 0, recievedBytes) << endl;

				// Echo the message back to the sender(s)
				sendRecievedBytes = send(sockets[i], recvbuf, recievedBytes, 0);
				if (sendRecievedBytes == SOCKET_ERROR) {
					cerr << "Soccket error with message, quitting" << endl;
				}
			}
		}
		else if (recievedBytes == 0)
			cerr << "Closing connection" << endl;
		else {
			cerr << "Message error, quitting" << endl;
		}

	} while (recievedBytes > 0);
}

void ClientHandler() {
	wsaDataError = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (wsaDataError != 0) {
		cerr << "WSA Quitting" << endl;
	}

	memset(&serverParameters, 0, sizeof(serverParameters));

	serverParameters.ai_family = AF_INET; // IPv4
	serverParameters.ai_socktype = SOCK_STREAM; // TCP
	serverParameters.ai_flags = AI_PASSIVE; // Use my IP

	infoFromAddr = getaddrinfo(NULL, "33000", &serverParameters, &results);
	if (infoFromAddr != 0) {
		cerr << "getaddrinfo Quitting" << endl;
	}

	// Create socket
	listening = socket(results->ai_family, results->ai_socktype, results->ai_flags); // Make socket, IPv6, TCP, auto-fill IP

	if (listening == INVALID_SOCKET) {
		cerr << "Socket error, quitting" << endl;
	}

	// Bind socket to a port/IP
	bindResult = ::bind(listening, results->ai_addr, (int)results->ai_addrlen);
	if (bindResult == SOCKET_ERROR) {
		cerr << "Bind error, quitting" << endl;
	}

	// Listen for clients on 'newSocket' socket, with max 5 clients
	if (listen(listening, 5) == SOCKET_ERROR) {
		cerr << "Listen error, quitting" << endl;
	}


	while (true)
	{
		// Accept incoming client
		ClientSocket = accept(listening, NULL, NULL);

		if (ClientSocket == INVALID_SOCKET) {
			cerr << "Accept error, quitting" << endl;
		}
		// Add client sockets to array
		else {
			sockets[j] = ClientSocket;
			j++;

		}
		// Start thread for every client
		thread msgHandler(clientMsgHandler, ClientSocket);
		msgHandler.detach();

	}

}


int main() {

	thread startClientHandler(ClientHandler);
	startClientHandler.join();

}