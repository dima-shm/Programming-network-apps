#include "func_ErrorMsgText.h"
#include <time.h>

bool portIsCorrect(int port)
{
	return (port >= 0 && port <= 65535);
}

void main(int argc, char* argv[])
{
	setlocale(0, "");
	SetConsoleTitle(L"Client");

	WSADATA wsaData;
	SOCKET cS;
	SOCKADDR_IN serv;
	char* servName = "DIMA";
	int   port = 2000;

	if (argc > 1)
		cout << "Имя сервера: " << (servName = argv[1]) << endl;
	else
		cout << "Имя сервера: " << servName << "      (по умолчанию)" << endl;
	if (argc > 2)
	{
		if (portIsCorrect(atoi(argv[3])))
			cout << "Порт:        " << (port = atoi(argv[2])) << endl;
		else
			cout << "Некорректный порт" << endl;
	}
	else
		cout << "Порт:        " << port << "      (по умолчанию)" << endl;

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("startup: ", WSAGetLastError());

		if ((cS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());

		hostent* s = gethostbyname(servName);
		serv.sin_addr = *(struct in_addr *)s->h_addr_list[0];
		serv.sin_family = AF_INET;
		serv.sin_port = htons(port);

		if ((connect(cS, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw SetErrorMsgText("connect: ", WSAGetLastError());

		int cmd;
		char obuf[50],
			ibuf[50];

		cout << endl;
		cout << "Команды:" << endl;
		cout << "[1] – rand" << endl;
		cout << "[2] – time" << endl;
		cout << "[3] – echo" << endl;
		cout << "[4] – Выйти из Client" << endl;
		cout << "Ввод: ";  cin >> cmd; cout << endl;

		if ((cmd > 0) && (cmd < 4))
		{
			switch (cmd)
			{
			case 1: strcpy_s(obuf, "Rand"); break;
			case 2: strcpy_s(obuf, "Time"); break;
			case 3: strcpy_s(obuf, "Echo"); break;
			}

			if (send(cS, obuf, sizeof(obuf), NULL) == SOCKET_ERROR)
				throw SetErrorMsgText("send: ", WSAGetLastError());

			if (recv(cS, ibuf, sizeof(ibuf), NULL) == SOCKET_ERROR)
				throw SetErrorMsgText("recv: ", WSAGetLastError());

			cout << "Ответ от сервера: " << ibuf << endl;
		}
		else if (cmd > 4)
			cout << "Введена несуществующая команда [" << cmd << "]" << endl << endl;

		if (closesocket(cS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket: ", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("cleanup: ", WSAGetLastError());
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}

	_getch();
}