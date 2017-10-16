#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "func_ErrorMsgText.h"
#include "Winsock2.h"


void main()
{
	setlocale(0, "");

	SOCKET sS;       // ���������� ������ �������
	WSADATA wsaData;

	string action;

	try
	{
		/********************************* 1 *********************************/
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup:", WSAGetLastError());


		/********************************* 2 *********************************/
		if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv;				   // ��������� ������ sS
		serv.sin_family = AF_INET;		   // ������������ IP-���������
		serv.sin_port = htons(2000);	   // ���� 2000
		serv.sin_addr.s_addr = INADDR_ANY; // ����� ����������� IP-�����

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("bind:", WSAGetLastError());


		/********************************* 3 *********************************/
		if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
			throw SetErrorMsgText("listen:", WSAGetLastError());

		SOCKET cS;						// ����� ��� ������ ������� � ��������
		SOCKADDR_IN clnt;				// ��������� ������ �������
		memset(&clnt, 0, sizeof(clnt)); // �������� ������
		int lclnt = sizeof(clnt);		// ������ SOCKADDR_IN


		cout << "������� \"y\", ����� ���������� ������" << endl;
		cout << "����: "; cin >> action;
		while (action == "y") 
		{
			if ((cS = accept(sS, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
				throw SetErrorMsgText("accept:", WSAGetLastError());


		/********************************* 4 *********************************/
			char bfrom[50]; // ����� �����
			int lobuf = 0;  // ���������� �������� ����
			int libuf = 0;  // ���������� ������������ ����

			cout << "���� �������:  " << clnt.sin_port << endl;
			cout << "����� �������: " << inet_ntoa(clnt.sin_addr) << endl << endl;

			do
			{
				if ((lobuf = recv(cS, bfrom, sizeof(bfrom), NULL)) == SOCKET_ERROR)
					throw SetErrorMsgText("recv:", WSAGetLastError());


		/********************************* 5 *********************************/
				if ((libuf = send(cS, bfrom, sizeof(bfrom), NULL)) == SOCKET_ERROR)
					throw SetErrorMsgText("send:", WSAGetLastError());

			} while (lobuf != 0);

			cout << "���������� ������?" << endl;
			cout << "����: "; cin >> action;
		}
		/********************************* 6 *********************************/
		if (closesocket(sS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText << endl;
	}
	_getch();
}