#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "func_ErrorMsgText.h"
#include "Winsock2.h"

WSADATA wsaData;
SOCKET cS;
SOCKADDR_IN serv;

bool GetServerByName(char* name, char* call, struct sockaddr* from, int* flen)
{
	int countOfSendedBytes = 0;
	int countOfReceivedBytes = 0;
	char msgFromServer[50];
	memset(&msgFromServer, 0, sizeof(msgFromServer));

	hostent *servInfo;
	servInfo = gethostbyname(name);

	in_addr *addr_list;
	addr_list = (in_addr*)*(servInfo->h_addr_list);

	serv.sin_addr = *addr_list;

	if ((countOfSendedBytes = sendto(cS, call, strlen(call), NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto: ", WSAGetLastError());

	if ((countOfReceivedBytes = recvfrom(cS, msgFromServer, sizeof(msgFromServer), NULL, from, flen)) == SOCKET_ERROR)
		throw SetErrorMsgText("recvfrom: ", WSAGetLastError());

	cout << "����� �� �������: " << msgFromServer << endl << endl;

	return (strcmp(name, msgFromServer) == 0);
}

void main()
{
	setlocale(0, "");

	int sizeOfServ = sizeof(serv);
	short servPort = 2000;
	char* servName = "dima-shm";

	serv.sin_family = AF_INET;	 // ������������ IP-���������
	serv.sin_port = htons(2000); // ���� 2000

	try 
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("WSAStartup: ", WSAGetLastError());

		if ((cS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());

		int optval = 1;
		if (setsockopt(cS, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
			throw SetErrorMsgText("setsockopt: ", WSAGetLastError());

		if (!GetServerByName("DIMA", servName, (sockaddr*)&serv, &sizeOfServ))
		{
			cout << "IP-����� �������: " << inet_ntoa(serv.sin_addr) << endl;
			cout << "����:             " << serv.sin_port << endl;
		}
		else
		{
			cout << "������ � ����� ������ �� ������" << endl;
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