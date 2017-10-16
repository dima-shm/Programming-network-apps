#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "func_ErrorMsgText.h"
#include "Winsock2.h"


void main()
{
	setlocale(0, "");

	SOCKET sS;       // дескриптор сокета сервера
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

		SOCKADDR_IN serv;				   // параметры сокета sS
		serv.sin_family = AF_INET;		   // используется IP-адресация
		serv.sin_port = htons(2000);	   // порт 2000
		serv.sin_addr.s_addr = INADDR_ANY; // любой собственный IP-адрес

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("bind:", WSAGetLastError());


		/********************************* 3 *********************************/
		if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
			throw SetErrorMsgText("listen:", WSAGetLastError());

		SOCKET cS;						// сокет для обмена данными с клиентом
		SOCKADDR_IN clnt;				// параметры сокета клиента
		memset(&clnt, 0, sizeof(clnt)); // обнулить память
		int lclnt = sizeof(clnt);		// размер SOCKADDR_IN


		cout << "Введите \"y\", чтобы продолжить работу" << endl;
		cout << "Ввод: "; cin >> action;
		while (action == "y") 
		{
			if ((cS = accept(sS, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
				throw SetErrorMsgText("accept:", WSAGetLastError());


		/********************************* 4 *********************************/
			char bfrom[50]; // буфер ввода
			int lobuf = 0;  // количество принятых байт
			int libuf = 0;  // количество отправленных байт

			cout << "Порт клиента:  " << clnt.sin_port << endl;
			cout << "Адрес клиента: " << inet_ntoa(clnt.sin_addr) << endl << endl;

			do
			{
				if ((lobuf = recv(cS, bfrom, sizeof(bfrom), NULL)) == SOCKET_ERROR)
					throw SetErrorMsgText("recv:", WSAGetLastError());


		/********************************* 5 *********************************/
				if ((libuf = send(cS, bfrom, sizeof(bfrom), NULL)) == SOCKET_ERROR)
					throw SetErrorMsgText("send:", WSAGetLastError());

			} while (lobuf != 0);

			cout << "Продолжить работу?" << endl;
			cout << "Ввод: "; cin >> action;
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