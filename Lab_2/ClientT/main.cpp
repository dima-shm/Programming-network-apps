#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "func_ErrorMsgText.h"
#include "Winsock2.h"
#include "consoleColors.h"


void prepareString(string& str, int i)
{
	str.erase(18);
	if (i < 10)
	{
		str.append("00");
		str.append(to_string(i));
	}
	else if (i < 100)
	{
		str.append("0");
		str.append(to_string(i));
	}
	else
		str.append(to_string(i));
}


void main()
{
	setlocale(0, "");

	SOCKET cC;       // дескриптор сокета клиента
	WSADATA wsaData;

	int numMsg;

	try
	{
		/********************************* 1 *********************************/
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup:", WSAGetLastError());


		/********************************* 2 *********************************/
		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket:", WSAGetLastError());


		/********************************* 3 *********************************/
		SOCKADDR_IN serv;							   // параметры сокета сервера
		serv.sin_family = AF_INET;					   // используется IP-адресация
		serv.sin_port = htons(2000);				   // TCP-порт 2000
		serv.sin_addr.s_addr = inet_addr("127.0.0.1"); // адрес сервера

		if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw SetErrorMsgText("connect:", WSAGetLastError());


		cout << "Введите количество передаваемых сообщений: "; cin >> numMsg; cout << endl;


		/********************************* 4 *********************************/
		char buf[50] = "Hello from Client 000"; // буфер ввода
		string bufStr;
		int libuf = 0;                          // количество отправленных байт
		int lobuf = 0;                          // количество принятых байт

		for (int i = 0; i < numMsg; i++)
		{
			if ((libuf = send(cC, buf, sizeof(buf), NULL)) == SOCKET_ERROR)
				throw SetErrorMsgText("send:", WSAGetLastError());

			setConsoleClrBlue();
			cout << "    Количество отправленый байт сообщения: " << libuf << endl;
			cout << "    Текст сообшения:                       " << buf << endl;
			setConsoleClrWhite();


		/********************************* 5 *********************************/
			if ((lobuf = recv(cC, buf, sizeof(buf), NULL)) == SOCKET_ERROR)
				throw SetErrorMsgText("recv:", WSAGetLastError());

			setConsoleClrGreen();
			cout << "Количество полученых байт сообщения:   " << lobuf << endl;
			cout << "Текст сообшения:                       " << buf << endl;
			setConsoleClrWhite();

			bufStr = (string)buf;
			prepareString(bufStr, i + 1);
			strcpy(buf, bufStr.c_str());
		}


		/********************************* 6 *********************************/
		if (closesocket(cC) == SOCKET_ERROR)
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