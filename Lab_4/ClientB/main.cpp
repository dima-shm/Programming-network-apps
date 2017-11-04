#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "func_ErrorMsgText.h"
#include "Winsock2.h"

WSADATA wsaData;
SOCKET cS;
SOCKADDR_IN serv;

bool GetServer(char* name, short port, struct sockaddr* from, int* flen)
{
	int countOfSendedBytes = 0;
	int countOfReceivedBytes = 0;
	char msgFromServer[100];

	memset(&msgFromServer, 0, sizeof(msgFromServer));

	if ((countOfSendedBytes = sendto(cS, name, strlen(name), NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto: ", WSAGetLastError());

	if ((countOfReceivedBytes = recvfrom(cS, msgFromServer, sizeof(msgFromServer), NULL, from, flen)) == SOCKET_ERROR)
		throw SetErrorMsgText("recvfrom: ", WSAGetLastError());

	cout << "ќтвет от сервера: " <<  msgFromServer << endl << endl;

	return (strcmp(name, msgFromServer) == 0);
}

void main()
{
	setlocale(0, "");

	int sizeOfServ = sizeof(serv);
	short servPort = 2000;
	char* servName = "dima-shm";

	serv.sin_family = AF_INET;				 // используетс€ IP-адресаци€
	serv.sin_port = htons(2000);		     // порт 2000
	serv.sin_addr.s_addr = INADDR_BROADCAST; // широковещание

	try 
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("WSAStartup: ", WSAGetLastError());

		if ((cS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());

		int optval = 1;
		if (setsockopt(cS, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
			throw SetErrorMsgText("setsockopt: ", WSAGetLastError());

		// ќтправка широковещательного запроса с позывным сервера и прием ответа
		if (GetServer(servName, servPort, (sockaddr*)&serv, &sizeOfServ))
		{
			cout << "IP-адрес сервера: " << inet_ntoa(serv.sin_addr) << endl;
			cout << "ѕорт:             " << serv.sin_port << endl;
		}

		if (closesocket(cS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket: ", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("WSACleanup: ", WSAGetLastError());
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}
	_getch();
}