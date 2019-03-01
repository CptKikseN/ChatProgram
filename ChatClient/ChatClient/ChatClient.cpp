#include "pch.h"
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")


using namespace std; 

void main()
{
	WSADATA dataStructur;
	WORD winsockVersion = MAKEWORD(2, 2);									//Used to decide which socket verison to use
	int errorChecker = WSAStartup(winsockVersion, &dataStructur);			//If the function is succesful it returns zero, if not it returns an error

	if (errorChecker != 0)
	{

		cerr << "#" << errorChecker << endl;								//Displays an error, if socket intialization failed 

		return;
	}

	string ipAddress = "127.0.0.1";											//Input server ip address here
	SOCKET socketIdentifier = socket(AF_UNSPEC, SOCK_STREAM, 0);			//Creates a socket

	if (socketIdentifier == INVALID_SOCKET)									//Checks if the socket is valid
	{
		cerr << "#" << WSAGetLastError << endl;								//Displays an error, if the socket is invalid
		return;
	}

	int portNumber = 30000;													//Input server port number here

	sockaddr_in supportInformation;
	supportInformation.sin_family = AF_UNSPEC;
	supportInformation.sin_port = htons(portNumber);
	inet_pton(AF_UNSPEC, ipAddress.c_str(), &supportInformation.sin_addr);	//The inet_pton() function converts an Internet address in its standard text format into its numeric binary form.

	int resultOfConnection = connect(socketIdentifier, (sockaddr*)&supportInformation, sizeof(supportInformation)); //Connects to socket 

	if (resultOfConnection == SOCKET_ERROR)
	{
		cerr << "#" << WSAGetLastError << endl;
	}

	char buffer[512];														//Max input size
	string inputFromUser;

	do
	{
		cout << "Whats on your mind?: ";
		getline(cin, inputFromUser);

		if (inputFromUser.size() > 0)
		{
			int textSender = send(socketIdentifier, inputFromUser.c_str(), inputFromUser.size() + 1, 0);

			if (textSender != SOCKET_ERROR)
			{
				int textReceiver = recv(socketIdentifier, buffer, 512, 0);

				if (textReceiver)
				{
					cerr << "#" << string(buffer, 0, textReceiver) << endl;
				}
			}
		}
	} while (inputFromUser.size() > 0);

	closesocket(socketIdentifier);
	WSACleanup();														//Return value is zero if operation was successful. Otherwise, the value SOCKET_ERROR is returned
}
