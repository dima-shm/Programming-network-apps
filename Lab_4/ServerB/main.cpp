#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "func_ErrorMsgText.h"
#include "Winsock2.h"

WSADATA wsaData;
SOCKET sS;
SOCKADDR_IN serv;
SOCKADDR_IN clnt;

bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen)
{
	char msgFromClnt[50];
	int countOfRecievedBytes = 0;

	memset(&msgFromClnt, 0, sizeof(msgFromClnt));

	if ((countOfRecievedBytes = recvfrom(sS, msgFromClnt, sizeof(msgFromClnt), NULL, from, flen)) == SOCKET_ERROR)
		throw SetErrorMsgText("recvfrom: ", WSAGetLastError());

	return (strcmp(name, msgFromClnt) == 0);
}

bool PutAnswerToClient(char* name, struct sockaddr* to, int *lto)
{
	int countOfSendedBytes = 0;

	if ((countOfSendedBytes = sendto(sS, name, strlen(name), NULL, to, *lto)) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto: ", WSAGetLastError());

	return countOfSendedBytes > 0;
}

void FindTheSameServer(char* name)
{
	SOCKET bS;
	SOCKADDR_IN broadcast;
	SOCKADDR_IN sameServ;
	int sameServSizeOf = sizeof(sameServ);

	broadcast.sin_family = AF_INET;
	broadcast.sin_port = htons(2000);
	broadcast.sin_addr.s_addr = INADDR_BROADCAST;

	char msgFromServ[50];
	memset(&msgFromServ, 0, sizeof(msgFromServ));

	if ((bS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		throw SetErrorMsgText("socket: ", WSAGetLastError());

	int optval = 1;
	if (setsockopt(bS, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
		throw SetErrorMsgText("setsockopt: ", WSAGetLastError());

	int timeout = 1000;
	if (setsockopt(bS, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
		throw SetErrorMsgText("setsockopt: ", WSAGetLastError());


	if (sendto(bS, name, strlen(name), NULL, (sockaddr*)&broadcast, sizeof(broadcast)) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto: ", WSAGetLastError());

	if (recvfrom(bS, msgFromServ, sizeof(msgFromServ), NULL, (sockaddr*)&sameServ, &sameServSizeOf) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
		{
			cout << "Сервер с таким же позывным не найден" << endl << endl;
			return;
		}
		throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
	}

	if (strcmp(msgFromServ, name) == 0)
	{
		cout << "Найден серевер с таким же позывным" << endl;
		cout << "IP-адрес: " << inet_ntoa(sameServ.sin_addr) << endl << endl;
	}

	if (closesocket(bS) == SOCKET_ERROR)
		throw SetErrorMsgText("closesocket: ", WSAGetLastError());
}

void main()
{
	setlocale(0, "");

	int sizeOfСlnt = sizeof(clnt);
	short port = 2000;
	char* servName = "dima-shm";

	serv.sin_family = AF_INET;		   // используется IP-адресация
	serv.sin_port = htons(port);	   // порт 2000
	serv.sin_addr.s_addr = INADDR_ANY; // любой IP-адрес

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("WSAStartup: ", WSAGetLastError());

		// Проверка наличия в локальной сети сервера с таким же позывным 
		FindTheSameServer(servName);

		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("bind: ", WSAGetLastError());

		while (true) 
		{
			// Ожидание запроса клиентской программы
			while (!GetRequestFromClient(servName, port, (sockaddr*)&clnt, &sizeOfСlnt))
			{ }
			// Пересылка позывного сервера программе клиента
			PutAnswerToClient(servName, (sockaddr*)&clnt, &sizeOfСlnt);

			cout << "IP-адрес клиента: " << inet_ntoa(clnt.sin_addr) << endl;
			cout << "Порт:             " << clnt.sin_port << endl;
			cout << endl;
		}

		if (closesocket(sS) == SOCKET_ERROR)
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