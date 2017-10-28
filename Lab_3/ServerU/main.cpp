#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "func_ErrorMsgText.h"
#include "Winsock2.h"


void main()
{
	setlocale(0, "");

	SOCKET sS;       // дескриптор сокета сервера
	WSADATA wsaData;

	string action;

	SOCKADDR_IN serv;
	SOCKADDR_IN from;
	int sizeOfFrom = sizeof(from);

	try 
	{
		/********************************* 1 *********************************/
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("WSAStartup: ", WSAGetLastError());


		/********************************* 2 *********************************/
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());

		serv.sin_family = AF_INET;		   // используется IP-адресация
		serv.sin_port = htons(2000);	   // порт 2000
		serv.sin_addr.s_addr = INADDR_ANY; // любой собственный IP-адрес

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("bind:", WSAGetLastError());


		/********************************* 3 *********************************/
		char buf[50];  // буфер ввода
		int lobuf = 0; // количество принятых байт
		int libuf = 0; // количество отправленных байт


		cout << "Введите \"y\", чтобы продолжить работу" << endl;
		cout << "Ввод: "; cin >> action;


		while (action == "y")
		{
			// демонстрация потери пакетов
			/*while (true)
			{
				memset(&buf, 0, sizeof(buf));
			
				if ((lobuf = recvfrom(sS, buf, sizeof(buf), NULL, (sockaddr*)&from, &sizeOfFrom)) == SOCKET_ERROR)
					throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
			
				Sleep(6);
			
				if (strcmp(buf, "") == 0)
					break;
			
				cout << "Текст сообшения: " << buf << endl;			
			}*/
			do {
				memset(&buf, 0, sizeof(buf));
				from.sin_family = AF_INET;		   // используется IP-адресация
				from.sin_port = htons(2000);	   // порт 2000
				from.sin_addr.s_addr = INADDR_ANY; // любой собственный IP-адрес
			
				if ((lobuf = recvfrom(sS, buf, sizeof(buf), NULL, (sockaddr*)&from, &sizeOfFrom)) == SOCKET_ERROR)
					throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
			
				cout << "Количество полученых байт сообщения: " << lobuf << endl;
				cout << "Текст сообшения:                     " << buf << endl;
			
				if (lobuf == 0)
					break;
			
			
				/********************************* 4 *********************************/
				if (libuf = sendto(sS, buf, strlen(buf), NULL, (sockaddr*)&from, sizeof(from)) == SOCKET_ERROR)
					throw SetErrorMsgText("sendto: ", WSAGetLastError());
			} while (lobuf != 0);
		
			cout << "Продолжить работу?" << endl;
			cout << "Ввод: "; cin >> action;
		}


		/********************************* 5 *********************************/
		if (closesocket(sS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}
	_getch();
}