#include <winsock2.h>
#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <Ws2tcpip.h>

using namespace std;

struct addrinfo serverParameters;
struct addrinfo *results;
int placeHolder;
SOCKET newSocket;
SOCKET ClientSocket;


int main()
{ 
	memset(&serverParameters, 0, sizeof(serverParameters));

	serverParameters.ai_family = AF_UNSPEC; // IPv4 or IPv6
	serverParameters.ai_socktype = SOCK_STREAM; // TCP
	serverParameters.ai_flags = AI_PASSIVE; // Automatically fills out the IP

	placeHolder = getaddrinfo(NULL, "30000", &serverParameters, &results);

	newSocket = socket(results->ai_family, results->ai_socktype, results->ai_flags); // Make socket, IPv6, TCP, auto-fill IP

	bind(newSocket, results->ai_addr, results->ai_addrlen); // Bind socket to a port/IP

	freeaddrinfo(results); // Free addrinfo from memory

	listen(newSocket, 5); // Listen for clients on 'newSocket' socket, with max 5 clients

	ClientSocket = accept(newSocket, NULL, NULL); // Accept incoming client

	if (ClientSocket == INVALID_SOCKET) {
		cout << "Error connecting client";

	}
	else
	{
		cout << "Client has connected!";
	}


}