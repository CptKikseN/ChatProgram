#include "pch.h"
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

using namespace std;

void main()
{
	WSADATA dataStructur;													//The WSADATA structure contains information about the Windows Sockets implementation.
	WORD winsockVersion = MAKEWORD(2, 2);									//Used to decide which socket verison to use, here version 2.2 is requested
	int errorChecker = WSAStartup(winsockVersion, &dataStructur);			//If the function is succesful it returns zero, if not it returns an error

	if (errorChecker != 0)
	{
		cerr << "Windows socket error, #" << errorChecker << endl;			//Displays an error, if socket intialization failed 
		return;
	}

	string ipAddress = "172.20.10.3";										//Input server ip address here
	SOCKET socketIdentifier = socket(AF_INET, SOCK_STREAM, 0);				//Creates a socket

	if (socketIdentifier == INVALID_SOCKET)									//Checks if the socket is valid
	{
		cerr << "Socket intializer error, #" << WSAGetLastError << endl;	//Displays an error, if the socket is invalid
		return;
	}

	int portNumber = 33000;													//Input server port number here

	sockaddr_in supportInformation;
	supportInformation.sin_family = AF_INET;								//The htons function returns the value in TCP/IP network byte order
	supportInformation.sin_port = htons(portNumber);
	inet_pton(AF_INET, ipAddress.c_str(), &supportInformation.sin_addr);	//The inet_pton() function converts an Internet address in its standard text format into its numeric binary form

	int resultOfConnection = connect(socketIdentifier, (sockaddr*)&supportInformation, sizeof(supportInformation)); //Connects to socket 

	if (resultOfConnection == SOCKET_ERROR)									//Checks if the socket connects
	{
		cerr << "Socket connection error, #" << WSAGetLastError << endl;
		closesocket(socketIdentifier);
		WSACleanup();														//Return value is zero if operation was successful. Otherwise, the value SOCKET_ERROR is returned
		return;
	}

	char buffer[512];														//Max input size
	string inputFromUser;

	//Chat
	do
	{
		cout << "Input: ";
		getline(cin, inputFromUser);

		if (inputFromUser.size() >= 0)									//Gets the input from the user, if there is any. It then sends it to the server
		{
			int textSender = send(socketIdentifier, inputFromUser.c_str(), inputFromUser.size() + 1, 0);

			if (textSender != SOCKET_ERROR)								//If the text sender is error free, a text reciever is created
			{
				ZeroMemory(buffer, 512);								//Fills a block of memory with zeros, that can manage a text that is lower than 512 bytes
				
				int textReceiver = recv(socketIdentifier, buffer, 512, 0);
				
				if (textReceiver >= 0)									//If the text being recieved has any input, it is send back to the client 
				{
					cout << "Output: " << string(buffer, 0, textReceiver) << endl;
				}
			}
		}
	} while (inputFromUser.size() > 0);									//inputFromUser.size() > 0

	closesocket(socketIdentifier);
}
